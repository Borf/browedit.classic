#define __GRAPHICS_CPP__
#include "main.h"
#include "common.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <math.h>
#include <map>
#include "wm/hotkeywindow.h"
#include "menucommands.h"

#include "graphics.h"
#include "menu.h"

extern long			mousex, mousey;
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
extern cMenu*			currentobject;
extern string			rodir;
extern long				lastmotion;
extern string			fontname;
extern cMenu*			lastmenu;
extern string			skinFile;
double mouse3dx, mouse3dy, mouse3dz;
extern TiXmlDocument	config;

cMenu* popupmenu = NULL;

int cGraphics::draw(bool drawwm)
{
	frameticks = SDL_GetTicks() - lasttick;
	lasttick += frameticks;

	glClearColor(Graphics.backgroundcolor.x, Graphics.backgroundcolor.y, Graphics.backgroundcolor.z, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode

	if(topcamera)
	{
		LightPosition[0] = mouse3dx;
		LightPosition[1] = mouse3dz;
		LightPosition[2] = mouse3dy+500;
		LightPosition[3] = 1.0f;
	}
	else
	{
		LightPosition[0] = -camerapointer.x + cameraheight*sin(camerarot);
		LightPosition[1] = 10+cameraheight+cameraangle;
		LightPosition[2] = -camerapointer.y + cameraheight*cos(camerarot);
		LightPosition[3] = 1.0f;
	}
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);			// Position The Light

	glEnable(GL_LIGHTING);

	if(state != OBJECTSELECT && state != OBJECTPROPS)
		world.draw();

	glDisable(GL_LIGHTING);



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
			glVertex2f( (width / 2) - (font->textlen(message)/2.0f) - 50, height/2.0f - 50);
			glVertex2f( (width / 2) + (font->textlen(message)/2.0f) + 50, height/2.0f - 50);
			glVertex2f( (width / 2) + (font->textlen(message)/2.0f) + 50, height/2.0f + 50);
			glVertex2f( (width / 2) - (font->textlen(message)/2.0f) - 50, height/2.0f + 50);
		glEnd();
		font->print(0,0,0,(width / 2) - (font->textlen(message)/2.0f),height/2.0f,"%s", message.c_str());
	}

	if(texturepreview != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, texturepreview->texid());
		glBegin(GL_QUADS);
			glTexCoord2f(1,1);		glVertex2f( 256.0f, height-32.0f);
			glTexCoord2f(1,0);		glVertex2f( 256.0f, height-(32+256.0f));
			glTexCoord2f(0,0);		glVertex2f( 0.0f, height-(32+256.0f));
			glTexCoord2f(0,1);		glVertex2f( 0.0f, height-32.0f);
		glEnd();


	}

	if (previewmodel != NULL)
	{
		glDisable(GL_DEPTH_TEST);
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
	/*	glColor4f(1,1,1,1);
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
			previewmodel->pos = cVector3((w()/5)-25,-h()+32+250,0);
			previewmodel->draw(false);
			previewmodel->rot.y+=40*(frameticks / 1000.0f);
		}
		glEnable(GL_DEPTH_TEST);*/
	}
	if(world.loaded && editmode != MODE_OBJECTS && editmode != MODE_OBJECTGROUP && editmode != MODE_LIGHTS)
	{
		int i;
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		for(i = 0; 288*i+256 < height; i++)
		{
			if (editmode == MODE_GAT)
			{
				if (i+texturestart > gattextures.size()-1)
					continue;
				glBindTexture(GL_TEXTURE_2D, gattextures[i+texturestart]->texid());
			}
			else if (editmode == MODE_WATER)
			{
				if (i+world.water.type >= Graphics.watercount)
					continue;
				static float frame = 0;
				glBindTexture(GL_TEXTURE_2D, watertextures[i+world.water.type][(int)floor(frame)]->texid());
				frame+=0.25;
				if (frame >= Graphics.watertextures[i+world.water.type].size())
					frame = 0;
			}
			else
			{
				if (i+texturestart >= (int)world.textures.size())
					continue;
				glBindTexture(GL_TEXTURE_2D, world.textures[i+texturestart]->texid());
			}
			glBegin(GL_QUADS);
				glTexCoord2f(1,1);		glVertex2f( width, height-(32+288*i));
				glTexCoord2f(1,0);		glVertex2f( width, height-(32+288*i+256));
				glTexCoord2f(0,0);		glVertex2f( width-256, height-(32+288*i+256));
				glTexCoord2f(0,1);		glVertex2f( width-256, height-(32+288*i));
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.5f,0.5f,1);
		for(i = 0; 288*i+256 < height; i++)
		{
			if (editmode == MODE_GAT)
			{
				if (i+texturestart > 6)
					continue;
			}
			else if (editmode == MODE_WATER)
			{
				if (i+world.water.type > 5)
					continue;
			}
			glBegin(GL_LINE_LOOP);
				glTexCoord2f(1,1);		glVertex2f( width, height-(32+288*i));
				glTexCoord2f(1,0);		glVertex2f( width, height-(32+288*i+256));
				glTexCoord2f(0,0);		glVertex2f( width-256, height-(32+288*i+256));
				glTexCoord2f(0,1);		glVertex2f( width-256, height-(32+288*i));
			glEnd();
		}
		glEnd();


		glColor3f(1,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex2f( selectionstart.x, height-selectionstart.y);
			glVertex2f( selectionstart.x, height-selectionend.y);
			glVertex2f( selectionend.x, height-selectionend.y);
			glVertex2f( selectionend.x, height-selectionstart.y);
		glEnd();

		glColor3f(1,1,1);
	}
	else if (editmode == MODE_OBJECTGROUP)
	{
		char buf[100];
		if (!groupeditmode)
			sprintf(buf, "Selecting");
		else
			sprintf(buf, "Editing");
		font->print(1,1,1,width-font->textlen(buf), height-40, buf);
	}
	else if (!world.loaded)
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
		glBindTexture(GL_TEXTURE_2D, splash->texid());
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);		glVertex2f( 0, height-20);
			glTexCoord2f(0,0);		glVertex2f( 0, 0);
			glTexCoord2f(1,0);		glVertex2f( width-256, 0);
			glTexCoord2f(1,1);		glVertex2f( width-256, height-20);
		glEnd();
	}
	glEnable(GL_BLEND);

		
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	menu->draw();

	if(drawwm)
		WM.draw();


	char buf[100];
	if(currentobject != NULL)
		sprintf(buf, "Editmode: %s, File: %s", 	editmode == MODE_TEXTURE ?		GetMsg("menu/editmode/TEXTUREEDIT") : 
												editmode == MODE_HEIGHTDETAIL ? GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
												editmode == MODE_HEIGHTGLOBAL ? GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
												editmode == MODE_WALLS ?		GetMsg("menu/editmode/WALLEDIT") :
												editmode == MODE_OBJECTS ?		GetMsg("menu/editmode/OBJECTEDIT") :
												editmode == MODE_GAT ?			GetMsg("menu/editmode/GATEDIT") :
												editmode == MODE_WATER ?		GetMsg("menu/editmode/WATEREDIT") :
												editmode == MODE_EFFECTS ?		GetMsg("menu/editmode/EFFECTSEDIT") :
												editmode == MODE_SOUNDS ?		GetMsg("menu/editmode/SOUNDSEDIT") :
												editmode == MODE_LIGHTS ?		GetMsg("menu/editmode/LIGHTSEDIT") :
												editmode == MODE_OBJECTGROUP ?	GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
												editmode == MODE_SPRITE ?		GetMsg("menu/editmode/SPRITEEDIT") : 
	
	"", 
		((cMenuItem*)currentobject)->data2.c_str());
	else
		sprintf(buf, "Editmode: %s",	editmode == MODE_TEXTURE ?		GetMsg("menu/editmode/TEXTUREEDIT") : 
										editmode == MODE_HEIGHTDETAIL ? GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
										editmode == MODE_HEIGHTGLOBAL ? GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
										editmode == MODE_WALLS ?		GetMsg("menu/editmode/WALLEDIT") :
										editmode == MODE_OBJECTS ?		GetMsg("menu/editmode/OBJECTEDIT") :
										editmode == MODE_GAT ?			GetMsg("menu/editmode/GATEDIT") :
										editmode == MODE_WATER ?		GetMsg("menu/editmode/WATEREDIT") :
										editmode == MODE_EFFECTS ?		GetMsg("menu/editmode/EFFECTSEDIT") :
										editmode == MODE_SOUNDS ?		GetMsg("menu/editmode/SOUNDSEDIT") :
										editmode == MODE_LIGHTS ?		GetMsg("menu/editmode/LIGHTSEDIT") :
										editmode == MODE_OBJECTGROUP ?	GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
										editmode == MODE_SPRITE ?		GetMsg("menu/editmode/SPRITEEDIT") : 
	"");
	float l = font->textlen(buf);
	font->print(0,0,0,width-l,height-14,buf);


	if (SDL_GetTicks() - lastmotion > 500)
	{
		cWindow* w = WM.inwindow();
		if (w != NULL)
		{
			cWindowObject* o = w->inobject();
			if (o != NULL)
			{
				string popup = o->ppopup();
				if (popup != "")
				{
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_DEPTH_TEST);
					glColor3f(0.5,0.5,1);
					int len = font->textlen(popup);
					glBegin(GL_QUADS);
						glVertex2f(mousex-2, Graphics.h()-mousey-2);
						glVertex2f(mousex+len+2, Graphics.h()-mousey-2);
						glVertex2f(mousex+len+2, Graphics.h()-mousey+16);
						glVertex2f(mousex-2, Graphics.h()-mousey+16);
					glEnd();
					font->print(1,1,1,mousex, Graphics.h()-mousey, "%s", popup.c_str());
					glEnable(GL_DEPTH_TEST);
				}
			}
		}
	}

	if(popupmenu != NULL)
	{
		glDisable(GL_DEPTH_TEST);
		popupmenu->draw();
		glEnable(GL_DEPTH_TEST);
	}
	

	
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
	Log(3,0,GetMsg("graphics/INIT"));	
	cameraheight = 123;
	camerarot = 0.0f;
	//camerapointer = cVector2(980,980);
	camerapointer = cVector2(-774,-963.5);
	cameraangle = 0;


	int flags = 0;
	const SDL_VideoInfo* info = NULL;
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO ) < 0 ) {
		Log( 1,0, GetMsg("graphics/INITFAIL"), SDL_GetError( ) );
		return 0;
	}
	SDL_EnableUNICODE(1);
	info = SDL_GetVideoInfo( );

	if( !info ) {
		Log(1,0,GetMsg("graphics/QUERYFAIL"), SDL_GetError( ) );
		return -1;
	}

	int bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	flags = SDL_OPENGL;// | SDL_FULLSCREEN;
	if(fullscreen)
	{
		flags = SDL_OPENGL | SDL_FULLSCREEN;
	}
	if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
		Log( 1,0, GetMsg("graphics/VIDEOMODEFAIL"), SDL_GetError( ) );
		return -1;
	}

	SDL_WM_SetIcon(SDL_LoadBMP("data/icon.bmp"), NULL);
	char buf[100];
	sprintf(buf, "Borf's Ragnarok Online World Editor, revision %i", getversion());
	SDL_WM_SetCaption(buf, "BROWEdit");

	//SDL_ShowCursor(0);
	if (InitGL() == 0)							// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		Log(1,0,GetMsg("graphics/OPENGLFAIL"));
		return 0;						// Return FALSE
	}

	Log(3,0,GetMsg("graphics/OPENINGTEXTURES"));

	glDisable(GL_LIGHTING);
	font = new cFont();
	font->load("data/fonts/"+fontname+".tga");
	mask = cTextureLoaders::load("data/mask.tga");
	bulb = cTextureLoaders::load("data/bulb.tga");
	splash = cTextureLoaders::load("data/hamtaro.tga");
	Log(3,0,GetMsg("graphics/INITIALIZINGWM"));
	WM.init(skinFile);
	WM.addwindow(new cHotkeyWindow(WM.texture, &WM.font, Graphics.WM.skin));

	int i;
	for(i = 0; i < gattiles.size(); i++)
	{
		char buf[64];
		sprintf(buf, "data/gat%i.tga", gattiles[i]);
		gattextures.push_back(TextureCache.load(buf));
	}

	
	waterdir =		config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("directory")->FirstChild()->Value();
	waterext =		config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("extension")->FirstChild()->Value();
	watercount=atoi(config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("count")->FirstChild()->Value());

	watertextures.resize(watercount);
	for(i = 0; i < watercount; i++)
	{
		for(int ii = 0; ii < 32; ii++)
		{
			char buf[100];
			sprintf(buf, "%s%swater%i%02i%s", rodir.c_str(), waterdir.c_str(), i, ii, waterext.c_str());
			watertextures[i].push_back(TextureCache.load(buf));
		}
	}
	if(watercount == 0)
	{
		watertextures.resize(1);
		watertextures[i].push_back(TextureCache.load(rodir + waterdir + "water" + waterext));
	}

	Log(3,0,GetMsg("file/DONELOADING"), "water.txt");

	previewmodel = NULL;

	LightAmbient[0] = 0.1f;
	LightAmbient[1] = 0.1f;
	LightAmbient[2] = 0.1f;
	LightAmbient[3] = 2.0f;

	LightDiffuse[0] = 0.9f;
	LightDiffuse[1] = 0.9f;
	LightDiffuse[2] = 0.9f;
	LightDiffuse[3] = 2.0f;

	LightPosition[0] = 0.0f;
	LightPosition[1] = 0.0f;
	LightPosition[2] = 0.0f;
	LightPosition[3] = 0.0f;


	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	glEnable(GL_LIGHT1);										// Enable Light One
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);


	/*popupmenu = new cMenu();
	popupmenu->parent = NULL;
	popupmenu->drawstyle = 1;
	popupmenu->x = 100;
	popupmenu->y = 100;
	popupmenu->w = 150;
	popupmenu->opened = true;
	cMenuItem* mm;
	cMenu* favs;
	ADDMENU(favs,		popupmenu, "Favorites",		popupmenu->x + 150,200); // File
	favs->y = 100;

	ADDMENUITEM(mm,popupmenu,"Disable Shadows",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupmenu,"Snap to floor",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupmenu,"Set to 50 over floor",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupmenu,"Set as sunlight",		&MenuCommand_new); //new

	ADDMENUITEM(mm,favs,"Torch",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Spotlight",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Disco Red",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Disco Green",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Disco Blue",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Faint stuff",		&MenuCommand_new); //new
	ADDMENUITEM(mm,favs,"Random Colors",		&MenuCommand_new); //new
	favs->opened = true;*/
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




int cGraphics::InitGL(void)
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
	glAlphaFunc(GL_GREATER, 0.1f);


	return 1;										// Initialization Went OK
}


void cGraphics::KillGLWindow(void)								// Properly Kill The Window
{
	SDL_ShowCursor(0);
	world.unload();
	TextureCache.unload(mask);
	TextureCache.unload(bulb);
	int i;
	for(i = 0; i < gattextures.size(); i++)
		TextureCache.unload(gattextures[i]);

	for(i = 0; i < watercount; i++)
	{
		for(unsigned int ii = 0; ii < watertextures[i].size(); ii++)
		{
			TextureCache.unload(watertextures[i][ii]);
		}
	}

	WM.unload();

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
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			glVertex2f(0, Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h());
			glVertex2f(0, Graphics.h());
		glEnd();
		glBegin(GL_LINES);
		for(i = 0; i < 10; i++)
		{
			glColor4f(i/3.0f,i/3.0f,i/3.0f,1);
			glVertex2f(0, Graphics.h()-20+i);
			glVertex2f(Graphics.w(), Graphics.h()-20+i);
		}
		glEnd();
		glColor4f(0,0,0,1);
		glBegin(GL_QUADS);
			glVertex2f(0, Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-19);
			glVertex2f(0, Graphics.h()-19);
		glEnd();
		for(i = 0; i < (int)items.size(); i++)
		{
			if (mousex >= x + items[i]->x && mousex < x + items[i]->x + items[i]->w && mousey < 20)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,0.5);
				glBegin(GL_QUADS);
					glVertex2f(x+items[i]->x, 		Graphics.h()-y-15);
					glVertex2f(x+items[i]->x+items[i]->w, 	Graphics.h()-y-15);
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
		glColor4f(0,0,0,1);
	}
	else if (drawstyle == 1)
	{
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,0.75);
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
		
		glColor4f(0,0,0,0.5);
		for(i = 0; i < (int)items.size(); i++)
		{
			if (Graphics.font->textlen(items[i]->title.c_str()) > maxlen-50)
				maxlen = Graphics.font->textlen(items[i]->title.c_str())+50;
			float color = 0;
			if ((mousex > x && mousex < x+maxlen && (mousey) > y+i*20 && (mousey) < y+i*20+20))
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
			if(items[i]->items.size() > 0)
			{
				Graphics.font->print(color,color,color,x+maxlen-12,Graphics.h()-y-20*i-18,">");
				Graphics.font->print(color,color,color,x+maxlen-12+1,Graphics.h()-y-20*i-18-1,"%c",7);
			}
			if(items[i]->opened)
				items[i]->draw();
		}
		if(w != maxlen && !updatedchildrenpos && parent != NULL)
		{
			if(parent->drawstyle != 0)
				w = maxlen;
			else
				updatedchildrenpos = true;
			for(unsigned int ii = 0; ii < items.size(); ii++)
			{
				items[ii]->x = x+maxlen;
				items[ii]->y = y+ii*20;
				if (items[ii]->y + (int)items[ii]->items.size()*20 > Graphics.h())
					items[ii]->y = Graphics.h() - items[ii]->items.size()*20;
			}
		}
	}
}


cMenu* cMenu::inwindow(int xx, int yy)
{
	int i,m = w;
	if (parent != NULL && parent->drawstyle == 0)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			if (Graphics.font->textlen(items[i]->title.c_str()) > m-50)
				m = Graphics.font->textlen(items[i]->title.c_str())+50;

		}
	}
		
	mouseover = false;
	if (xx > x && xx < x+m && Graphics.h()-yy > y && Graphics.h()-yy < y+(int)h() && opened)
	{
		mouseover = true;
		return this;
	}
	if (opened)
	{
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
	unsigned int i,ii;
	int m;
	if (drawstyle == 0)
	{
		for(i = 0; i < items.size(); i++)
		{
			m = items[i]->w;
			for(ii = 0; ii < items[i]->items.size(); ii++)
			{
				if (Graphics.font->textlen(items[i]->items[ii]->title.c_str()) > m-50)
					m = Graphics.font->textlen(items[i]->items[ii]->title.c_str())+50;
			}

			
			if (mousex > x+items[i]->x && mousex < x+items[i]->x+m)
			{
				items[i]->opened = !items[i]->opened;
			}
		}
	}
	else //if (opened)
	{
		m = w;
		if(parent != NULL)
		{
			if (parent->drawstyle == 0)
			{
				for(i = 0; i < items.size(); i++)
				{
					if (Graphics.font->textlen(items[i]->title.c_str()) > m-50)
						m = Graphics.font->textlen(items[i]->title.c_str())+50;

				}
			}
		}
		for(i = 0; i < items.size(); i++)
		{

			if (mousex > x && mousex < x+m && (mousey) > y+20*i && (mousey) < y+20*i+20)
			{
				if(items[i]->item)
				{
					if (items[i]->mouseoverproc)
					{
						if(!items[i]->mouseoverproc(items[i]))
							return;
					}
					lastmenu = this;
					menu->closemenu();
					closemenu();
					((cMenuItem*)items[i])->proc((cMenuItem*)items[i]);
					return;
				}
				else
				{
					opened = true;
					for(unsigned int ii = 0; ii < items.size(); ii++)
					{
						if(i != ii)
							items[ii]->closemenu();
					}
					items[i]->opened = !items[i]->opened;
					if (!items[i]->opened)
						items[i]->closemenu();
				}
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
	for(unsigned int i = 0; i < items.size(); i++)
	{
		if(items[i] == curitem)
		{
			if (i < (items.size()) - 1)
			{
				if (items[i+1]->item)
					return items[i+1];
				else
					return items[i+1]->getfirstitem();
			}
		}
	}
	
	return parent->getnext(this);

}

cMenu* cMenu::getprev(cMenu* curitem)
{
	for(unsigned int i = 0; i < items.size(); i++)
	{
		if(items[i] == curitem)
		{
			if (i > 0)
			{
				if (items[i-1]->item)
					return items[i-1];
				else
					return items[i-1]->getlastitem();
			}
		}
	}
	
	return parent->getprev(this);
}


cMenu* cMenu::getfirstitem()
{
	if(items.size() == 0 || item)
		return this;
	if(items[0]->item)
		return items[0];
	else
		return items[0]->getfirstitem();
}

cMenu* cMenu::getlastitem()
{
	if(items.size() == 0 || item)
		return this;
	if(items[items.size()-1]->item)
		return items[items.size()-1];
	else
		return items[items.size()-1]->getlastitem();

}



cMenu* cMenu::finddata(string d)
{
	if(item)
	{
		if(((cMenuItem*)this)->data == d)
			return this;
	}
	for(unsigned int i = 0; i < items.size(); i++)
	{
		cMenu* m = items[i]->finddata(d);
		if (m != NULL)
			return m;
	}
	return NULL;
}
