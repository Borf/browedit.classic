#define __GRAPHICS_CPP__
#include "main.h"
#include "common.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <math.h>
#include <map>

#include "graphics.h"
#include "menu.h"

extern float			mousex, mousey;
extern eState			state;
extern cFileSystem		fs;
extern void				ChangeGrid();
extern string			message;
extern bool				showmessage;
extern cMenu*			menu;
extern map<long, string, less<long> >	idtomodel;
extern eMode			editmode;
float f = 0;
extern bool				lbuttondown;

double mouse3dx, mouse3dy, mouse3dz;

int cGraphics::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode

	if(state != OBJECTSELECT && state != OBJECTPROPS)
		world.draw();




	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,width,0,height,-10000,10000);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	if (showmessage)
		{
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glVertex2f( (width / 2) - (font->textlen(message)/2) - 50, height/2 - 50);
			glVertex2f( (width / 2) + (font->textlen(message)/2) + 50, height/2 - 50);
			glVertex2f( (width / 2) + (font->textlen(message)/2) + 50, height/2 + 50);
			glVertex2f( (width / 2) - (font->textlen(message)/2) - 50, height/2 + 50);
		glEnd();
		font->print(0,0,0,(width / 2) - (font->textlen(message)/2),height/2,"%s", message.c_str());
	}

	if (previewmodel != NULL)
	{
		if (previewcolor != 0)
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1,1,1,previewcolor > 20 ? 1 : previewcolor / 20.0f);
			glBegin(GL_QUADS);
				glVertex3f( 0,0,-1000);
				glVertex3f( 400,0,-1000);
				glVertex3f( 400,400,-1000);
				glVertex3f( 0,400,-1000);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			glColor4f(1,1,1,previewcolor > 10 ? 1 : previewcolor / 20.0f);
			previewmodel->pos = cVector3(40,-100,0);
			previewmodel->draw(false);
			if (previewcolor > -1)
				previewcolor--;
		}
		glColor4f(1,1,1,1);
		if (editmode == MODE_OBJECTS)
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBegin(GL_QUADS);
				glVertex2f( w()-0,	h()-0-32);
				glVertex2f( w()-256,h()-0-32);
				glVertex2f( w()-256,h()-256-32);
				glVertex2f( w()-0,	h()-256-32);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			previewmodel->pos = cVector3((w()/5)-25,-h()+32+192,1000);
			previewmodel->draw(false);
			previewmodel->rot.y+=3;
		}
	}
	if(world.loaded && editmode != MODE_OBJECTS)
	{
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		for(int i = 0; 288*i+256 < height; i++)
		{
			glBindTexture(GL_TEXTURE_2D, world.textures[i+texturestart]->texid());
			glBegin(GL_QUADS);
				glTexCoord2f(1,1);		glVertex2f( width, height-(32+288*i));
				glTexCoord2f(1,0);		glVertex2f( width, height-(32+288*i+256));
				glTexCoord2f(0,0);		glVertex2f( width-256, height-(32+288*i+256));
				glTexCoord2f(0,1);		glVertex2f( width-256, height-(32+288*i));
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

		glColor3f(1,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex2f( selectionstart.x, height-selectionstart.y);
			glVertex2f( selectionstart.x, height-selectionend.y);
			glVertex2f( selectionend.x, height-selectionend.y);
			glVertex2f( selectionend.x, height-selectionstart.y);
		glEnd();

		glColor3f(1,1,1);
	}
	menu->draw();


	

	
/*
glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
//	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix


*/

	return 1;
}


int cGraphics::init()
{
	width = 1024;
	height = 600;

	
	cameraheight = 123;
	camerarot = 0.0f;
	//camerapointer = cVector2(980,980);
	camerapointer = cVector2(-774,-963.5);
	cameraangle = 0;


	int flags = 0;
	const SDL_VideoInfo* info = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf( stderr, "Video initialization failed: %s\n",
				 SDL_GetError( ) );
		return 0;
	}
	SDL_EnableUNICODE(1);
	info = SDL_GetVideoInfo( );

	if( !info ) {
		fprintf( stderr, "Video query failed: %s\n",
				 SDL_GetError( ) );
		return -1;
	}

	int bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	flags = SDL_OPENGL;// | SDL_FULLSCREEN;
	if(fullscreen != 0)
	{
		flags = SDL_OPENGL | SDL_FULLSCREEN;
	}
	if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
		fprintf( stderr, "Video mode set failed: %s\n",
				 SDL_GetError( ) );
		return -1;
	}


//	SDL_WM_SetIcon(SDL_LoadBMP("data/domovsneko.bmp"), NULL);
	SDL_WM_SetCaption("Ragnarok Online Worldeditor 0.1", "RO Worldeditor");

	//SDL_ShowCursor(0);
	if (InitGL() == 0)							// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		Log(1,0,"Initialization Failed.");
		return 0;						// Return FALSE
	}


	glDisable(GL_LIGHTING);
	font = new cFont();
	font->load("data/fonts/font1.tga");
	mask = new cTexture();
	mask->Load("data/textures/mask.tga");
	bulb = new cTexture();
	mask->Load("data/textures/interface/bulb.tga");

	previewmodel = NULL;

	return 1;
}










int cGraphics::ReSizeGLScene(GLsizei w, GLsizei h)	// Resize And Initialize The GL Window
{
	if (h==0)										// Prevent A Divide By Zero By
	{
		h=1;										// Making Height Equal One
	}
	width = w;
	height = h;
	return 1;								// Return TRUE

}




int cGraphics::InitGL(void)										// All Setup For OpenGL Goes Here
{

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.01f);


	return 1;										// Initialization Went OK
}


void cGraphics::KillGLWindow(void)								// Properly Kill The Window
{
	SDL_ShowCursor(0);

	return;
}



extern cGraphics 		Graphics;

void cMenu::draw()
{
	int i;
	if(drawstyle == 0)
	{
		bool oneopened = false;
		for(i = 0; i < (int)items.size(); i++)
			if (items[i]->opened)
				oneopened = true;
		
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.7f,0.7f,0.7f,1);
		glBegin(GL_QUADS);
			glVertex2f(0, Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h());
			glVertex2f(0, Graphics.h());
		glEnd();
		glColor4f(0,0,0,1);
		for(i = 0; i < (int)items.size(); i++)
		{
			if (mousex >= x + items[i]->x && mousex < x + items[i]->x + items[i]->w && mousey < 20)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,1);
				glBegin(GL_QUADS);
					glVertex2f(x+items[i]->x, 		Graphics.h()-y-20);
					glVertex2f(x+items[i]->x+items[i]->w, 	Graphics.h()-y-20);
					glVertex2f(x+items[i]->x+items[i]->w, 	Graphics.h()-y);
					glVertex2f(x+items[i]->x, 		Graphics.h()-y);
					if (oneopened && !items[i]->opened)
						items[i]->opened = true;
				glEnd();
				glColor4f(0,0,0,1);
			}
			else if (oneopened && items[i]->opened && mousey < 20)
				items[i]->closemenu();

			Graphics.font->print(0,0,0,x+items[i]->x+3,Graphics.h()-y-18,"%s",items[i]->title.c_str());
			if(items[i]->opened)
				items[i]->draw();
		}
	}
	else if (drawstyle == 1)
	{
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.7f,0.7f,0.7f,1);
		glBegin(GL_QUADS);
			glVertex2f(x, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y);
			glVertex2f(x, Graphics.h()-y);
		glEnd();
		glColor4f(0,0,0,1);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y);
			glVertex2f(x, Graphics.h()-y);
		glEnd();
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x+1, Graphics.h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, Graphics.h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, Graphics.h()-y-1);
			glVertex2f(x+1, Graphics.h()-y-1);
		glEnd();
		
		glColor4f(0,0,0,1);
		for(i = 0; i < (int)items.size(); i++)
		{
			if (Graphics.font->textlen(items[i]->title.c_str()) > maxlen-50)
				maxlen = Graphics.font->textlen(items[i]->title.c_str())+50;
			
			float color = 0;
			if (mousex > x && mousex < x+maxlen && (mousey) > y+i*20 && (mousey) < y+i*20+20)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,1);
				glBegin(GL_QUADS);
					glVertex2f(x+3, Graphics.h()-y-20*i-18);
					glVertex2f(x+maxlen-3, Graphics.h()-y-20*i-18);
					glVertex2f(x+maxlen-3, Graphics.h()-y-20*i-2);
					glVertex2f(x+3, Graphics.h()-y-20*i-2);
				glEnd();
				glColor4f(0,0,0,1);
				color = 1;
			}
			
			if (items[i]->ticked)
				Graphics.font->print(color,color,color,x+5,Graphics.h()-y-20*i-18,"X");
			Graphics.font->print(color,color,color,x+23,Graphics.h()-y-20*i-18,"%s",items[i]->title.c_str());
			if(items[i]->opened)
				items[i]->draw();
		}
	}
}


cMenu* cMenu::inwindow(int xx, int yy)
{
	mouseover = false;
	if (xx > x && xx < x+w && Graphics.h()-yy > y && Graphics.h()-yy < y+h() && opened)
	{
		mouseover = true;
		return this;
	}
	if (opened)
	{
		int i;
		for(i = 0; i < (int)items.size(); i++)
		{
			cMenu* m = items[i]->inwindow(xx, yy);
			if (m != NULL)
				return m;
		}
	}
	return NULL;
}

void cMenu::click(int xx, int yy)
{
	int i;
	if (drawstyle == 0)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			if (mousex > x+items[i]->x && mousex < x+items[i]->x+items[i]->w)
			{
				items[i]->opened = !items[i]->opened;
			}
		}
	}
	else if (opened)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			if (mousex > x && mousex < x+w && (mousey) > y+20*i && (mousey) < y+20*i+20)
			{
				if(items[i]->item)
				{
					((cMenuItem*)items[i])->proc((cMenuItem*)items[i]);
					menu->closemenu();
					menu->opened = true;
					return;
				}
				else
					items[i]->opened = !items[i]->opened;
			}
		}
	}
}


bool cGraphics::is3dSelected(float x, float y, float z)
{
	if(Graphics.selectionstart3d.x < Graphics.selectionend3d.x)
	{
		if(x < Graphics.selectionstart3d.x || x > Graphics.selectionend3d.x)
			return false;
	}
	else
	{
		if(x > Graphics.selectionstart3d.x || x < Graphics.selectionend3d.x)
			return false;
	}


/*	if(Graphics.selectionstart3d.y < Graphics.selectionend3d.y)
	{
		if(y < Graphics.selectionstart3d.y || y > Graphics.selectionend3d.y)
			return false;
	}
	else
	{
		if(y > Graphics.selectionstart3d.y || y < Graphics.selectionend3d.y)
			return false;
	}*/

	if(Graphics.selectionstart3d.z < Graphics.selectionend3d.z)
	{
		if(z < Graphics.selectionstart3d.z || z > Graphics.selectionend3d.z)
			return false;
	}
	else
	{
		if(z > Graphics.selectionstart3d.z || z < Graphics.selectionend3d.z)
			return false;
	}

	return true;
}


bool cmp(cMenu* a, cMenu* b)
{
	if (a->item == b->item) 
		return a->title < b->title;
	else 
		return (a->item > b->item);
}


cMenu* cMenu::getnext(cMenu* curitem)
{
	for(int i = 0; i < items.size(); i++)
	{
		if(items[i] == curitem)
		{
			if (i < ((int)items.size()) - 2)
			{
				if (items[i+1]->item)
					return items[i+1];
				else
					return items[i+1]->items[0];
			}
		}
	}
	
	return parent->getnext(this);

}
