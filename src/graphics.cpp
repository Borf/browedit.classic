#define __GRAPHICS_CPP__
#include "graphics.h"

#include "main.h"
#include "common.h"
#include "world.h"
#include "font.h"
#include "texture.h"
#include "frustum.h"
#include "wm/wm.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <math.h>
#include <map>
#include "windows/hotkeywindow.h"
#include "menucommands.h"

#include "menu.h"

extern long			mouseX, mouseY;
extern eState			state;
extern cFileSystem		fs;
extern void				ChangeGrid();
extern std::string			message;
extern bool				showmessage;
extern cMenu*			menu;
extern std::map<long, std::string, std::less<long> >	idtomodel;
extern eMode			editmode;
float f = 0;
extern bool				lbuttondown;
extern cMenu*			currentobject;
extern std::string			rodir;
extern long				lastmotion;
extern std::string			fontname;
extern std::string			skinFile;
double mouse3dx, mouse3dy, mouse3dz;
extern TiXmlDocument	config;

cMenu* popupmenu = NULL;

int cGraphics::draw(bool drawwm)
{
	frameticks = SDL_GetTicks() - lasttick;
	lasttick += frameticks;

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode

	if(topCamera)
	{
		lightPosition[0] = mouse3dx;
		lightPosition[1] = -mouse3dz;
		lightPosition[2] = 1000;
		lightPosition[3] = 1.0f;
	}
	else
	{
		lightPosition[0] = -camerapointer.x + cameraheight*sin(camerarot);
		lightPosition[1] = 10+cameraheight+cameraangle;
		lightPosition[2] = -camerapointer.y + cameraheight*cos(camerarot);
		lightPosition[3] = 1.0f;
	}
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);			// Position The Light

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

	if(texturePreview != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, texturePreview->texId());
		glBegin(GL_QUADS);
			glTexCoord2f(1,1);		glVertex2f( 256.0f, height-32.0f);
			glTexCoord2f(1,0);		glVertex2f( 256.0f, height-(32+256.0f));
			glTexCoord2f(0,0);		glVertex2f( 0.0f, height-(32+256.0f));
			glTexCoord2f(0,1);		glVertex2f( 0.0f, height-32.0f);
		glEnd();


	}

	if (previewModel != NULL)
	{
		glDisable(GL_DEPTH_TEST);
		if (previewColor != 0)
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1,1,1,previewColor > 20 ? 1 : previewColor / 20.0f);
			glBegin(GL_QUADS);
				glVertex3f( 0,0,-1000);
				glVertex3f( 400,0,-1000);
				glVertex3f( 400,400,-1000);
				glVertex3f( 0,400,-1000);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			glColor4f(1,1,1,previewColor > 10 ? 1 : previewColor / 20.0f);
			previewModel->pos = cVector3(40,-100,0);
			previewModel->draw(false);
			if (previewColor > -1)
				previewColor--;
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
			previewModel->pos = cVector3((w()/5)-25,-h()+32+250,0);
			previewModel->draw(false);
			previewModel->rot.y+=40*(frameticks / 1000.0f);
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
				if ((unsigned int)i+texturestart > gatTextures.size()-1)
					continue;
				glBindTexture(GL_TEXTURE_2D, gatTextures[i+texturestart]->texId());
			}
			else if (editmode == MODE_WATER)
			{
				if (i+world.water.type >= (int)waterCount)
					continue;
				static float frame = 0;
				glBindTexture(GL_TEXTURE_2D, waterTextures[i+world.water.type][(int)floor(frame)]->texId());
				frame+=0.25;
				if (frame >= waterTextures[i+world.water.type].size())
					frame = 0;
			}
			else
			{
				if (i+texturestart >= (int)world.textures.size())
					continue;
				glBindTexture(GL_TEXTURE_2D, world.textures[i+texturestart]->texId());
			}
			glBegin(GL_QUADS);
				glTexCoord2f(1,1);		glVertex2f( width, height-(32+288*i));
				glTexCoord2f(1,0);		glVertex2f( width, height-(32+288*i+256));
				glTexCoord2f(0,0);		glVertex2f( width-256, height-(32+288*i+256));
				glTexCoord2f(0,1);		glVertex2f( width-256, height-(32+288*i));
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		if(editmode == MODE_TEXTUREPAINT)
		{
			glEnable(GL_BLEND);
//			int ww = textureBrush[0].size();
//			int hh = textureBrush.size();
//			cVector2 off[4] = { cVector2(-1,0), cVector2(1,0), cVector2(0,1), cVector2(0,-1) };

			glBegin(GL_QUADS);
			glColor4f(1,0,0,0.5f);

			glVertex2f(width-256+(0+textureBrushOffset.x)*(256/textureGridSizeX),		height-(32+(0+textureBrushOffset.y)*(256/textureGridSizeY)));
			glVertex2f(width-256+(0+0.25f+textureBrushOffset.x)*(256/textureGridSizeX),	height-(32+(0+textureBrushOffset.y)*(256/textureGridSizeY)));
			glVertex2f(width-256+(0+0.25f+textureBrushOffset.x)*(256/textureGridSizeX),	height-(32+(0+0.25f+textureBrushOffset.y)*(256/textureGridSizeY)));
			glVertex2f(width-256+(0+textureBrushOffset.x)*(256/textureGridSizeX),		height-(32+(0+0.25f+textureBrushOffset.y)*(256/textureGridSizeY)));

			glColor4f(0,0,0,0.25f);
			for(unsigned int yy = 0; yy < textureBrush.size(); yy++)
			{
				for(unsigned int xx = 0; xx < textureBrush[yy].size(); xx++)
				{
					if(textureBrush[yy][xx])
					{
/*						for(int i = 0; i < 4; i++)
						{
							cVector2 p = off[i] + cVector2(xx,yy);
							bool draw = true;
							if(p.x >= 0 && p.x < ww && p.y >= 0 && p.y < hh)
							{
								if(textureBrush[p.y][p.x])
									draw = false;
							}
							if(draw)
							{
								glVertex2f(width-256,							height-(32+0));
								glVertex2f(width-256+xx*(256/texturegridsize),	height-(32+yy));
							}
						}*/
						if(xx+textureBrushOffset.x >= textureGridSizeX || xx+textureBrushOffset.x < 0)
							continue;
						if(yy+textureBrushOffset.y >= textureGridSizeY || yy+textureBrushOffset.y < 0)
							continue;

						glVertex2f(width-256+(xx+textureBrushOffset.x)*(256/textureGridSizeX),		height-(32+(yy+textureBrushOffset.y)*(256/textureGridSizeY)));
						glVertex2f(width-256+(xx+1+textureBrushOffset.x)*(256/textureGridSizeX),	height-(32+(yy+textureBrushOffset.y)*(256/textureGridSizeY)));
						glVertex2f(width-256+(xx+1+textureBrushOffset.x)*(256/textureGridSizeX),	height-(32+(yy+1+textureBrushOffset.y)*(256/textureGridSizeY)));
						glVertex2f(width-256+(xx+textureBrushOffset.x)*(256/textureGridSizeX),		height-(32+(yy+1+textureBrushOffset.y)*(256/textureGridSizeY)));


					}
				}
			}
			glEnd();
			glDisable(GL_BLEND);
		}
		else
		{
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
		}
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
		glBindTexture(GL_TEXTURE_2D, splash->texId());
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
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	menu->draw();

	if(drawwm)
		WM.draw();


	char buf[100];
	if(currentobject != NULL)
		sprintf(buf, "Editmode: %s, File: %s", 	editmode == MODE_TEXTURE ?			GetMsg("menu/editmode/TEXTUREEDIT") : 
												editmode == MODE_HEIGHTDETAIL ?		GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
												editmode == MODE_HEIGHTGLOBAL ?		GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
												editmode == MODE_WALLS ?			GetMsg("menu/editmode/WALLEDIT") :
												editmode == MODE_OBJECTS ?			GetMsg("menu/editmode/OBJECTEDIT") :
												editmode == MODE_GAT ?				GetMsg("menu/editmode/GATEDIT") :
												editmode == MODE_WATER ?			GetMsg("menu/editmode/WATEREDIT") :
												editmode == MODE_EFFECTS ?			GetMsg("menu/editmode/EFFECTSEDIT") :
												editmode == MODE_SOUNDS ?			GetMsg("menu/editmode/SOUNDSEDIT") :
												editmode == MODE_LIGHTS ?			GetMsg("menu/editmode/LIGHTSEDIT") :
												editmode == MODE_OBJECTGROUP ?		GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
												editmode == MODE_SPRITE ?			GetMsg("menu/editmode/SPRITEEDIT") : 
												editmode == MODE_TEXTUREPAINT ?		GetMsg("menu/editmode/TEXTUREPAINTEDIT") : 
	
	"", 
		((cMenuItem*)currentobject)->data2.c_str());
	else
		sprintf(buf, "Editmode: %s",	editmode == MODE_TEXTURE ?			GetMsg("menu/editmode/TEXTUREEDIT") : 
										editmode == MODE_HEIGHTDETAIL ?		GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
										editmode == MODE_HEIGHTGLOBAL ?		GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
										editmode == MODE_WALLS ?			GetMsg("menu/editmode/WALLEDIT") :
										editmode == MODE_OBJECTS ?			GetMsg("menu/editmode/OBJECTEDIT") :
										editmode == MODE_GAT ?				GetMsg("menu/editmode/GATEDIT") :
										editmode == MODE_WATER ?			GetMsg("menu/editmode/WATEREDIT") :
										editmode == MODE_EFFECTS ?			GetMsg("menu/editmode/EFFECTSEDIT") :
										editmode == MODE_SOUNDS ?			GetMsg("menu/editmode/SOUNDSEDIT") :
										editmode == MODE_LIGHTS ?			GetMsg("menu/editmode/LIGHTSEDIT") :
										editmode == MODE_OBJECTGROUP ?		GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
										editmode == MODE_SPRITE ?			GetMsg("menu/editmode/SPRITEEDIT") : 
										editmode == MODE_TEXTUREPAINT ?		GetMsg("menu/editmode/TEXTUREPAINTEDIT") : 
	"");
	float l = font->textlen(buf);
	font->print(0,0,0,width-l,height-14,buf);


	if (SDL_GetTicks() - lastmotion > 500)
	{
		cWindow* w = WM.inwindow();
		if (w != NULL)
		{
			cWindowObject* o = w->inObject();
			if (o != NULL)
			{
				std::string popup = o->getPopup();
				if (popup != "")
				{
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_DEPTH_TEST);
					glColor3f(0.5,0.5,1);
					int len = font->textlen(popup);
					glBegin(GL_QUADS);
						glVertex2f(mouseX-2, h()-mouseY-2);
						glVertex2f(mouseX+len+2, h()-mouseY-2);
						glVertex2f(mouseX+len+2, h()-mouseY+16);
						glVertex2f(mouseX-2, h()-mouseY+16);
					glEnd();
					font->print(1,1,1,mouseX, h()-mouseY, "%s", popup.c_str());
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
	if (initGL() == 0)							// Initialize Our Newly Created GL Window
	{
		killGLWindow();								// Reset The Display
		Log(1,0,GetMsg("graphics/OPENGLFAIL"));
		return 0;						// Return FALSE
	}

	Log(3,0,GetMsg("graphics/OPENINGTEXTURES"));

	glDisable(GL_LIGHTING);
	font = new cFont();
	font->load("data/fonts/"+fontname+".tga");
	splash = TextureCache.load(config.FirstChildElement("config")->FirstChildElement("splash")->FirstChild()->Value());
	Log(3,0,GetMsg("graphics/INITIALIZINGWM"));
	WM.init(skinFile);
	WM.addwindow(new cHotkeyWindow(WM.texture, WM.font, WM.skin));

	unsigned int i;
	for(i = 0; i < gatTiles.size(); i++)
	{
		char buf[64];
		sprintf(buf, "data/gat%i.tga", gatTiles[i]);
		gatTextures.push_back(TextureCache.load(buf));
	}
	gatBorder = TextureCache.load("data/gatBorder.tga");

	
	waterDirectory =		config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("directory")->FirstChild()->Value();
	waterExtension =		config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("extension")->FirstChild()->Value();
	waterCount=atoi(config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("count")->FirstChild()->Value());

	waterTextures.resize(waterCount);
	for(i = 0; i < waterCount; i++)
	{
		for(int ii = 0; ii < 32; ii++)
		{
			char buf[100];
			sprintf(buf, "%s%swater%i%02i%s", rodir.c_str(), waterDirectory.c_str(), i, ii, waterExtension.c_str());
			waterTextures[i].push_back(TextureCache.load(buf, false));
		}
	}
	if(waterCount == 0)
	{
		waterTextures.resize(1);
		waterTextures[i].push_back(TextureCache.load(rodir + waterDirectory + "water" + waterExtension));
	}

	Log(3,0,GetMsg("file/DONELOADING"), "water.txt");

	previewModel = NULL;

	lightAmbient[0] = 0.1f;
	lightAmbient[1] = 0.1f;
	lightAmbient[2] = 0.1f;
	lightAmbient[3] = 2.0f;

	lightDiffuse[0] = 0.9f;
	lightDiffuse[1] = 0.9f;
	lightDiffuse[2] = 0.9f;
	lightDiffuse[3] = 2.0f;

	lightPosition[0] = 0.0f;
	lightPosition[1] = 0.0f;
	lightPosition[2] = 0.0f;
	lightPosition[3] = 0.0f;


	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,lightPosition);			// Position The Light
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










int cGraphics::resizeGLScene(GLsizei w, GLsizei h)	// Resize And Initialize The GL Window
{
	if (h==0)										// Prevent A Divide By Zero By
	{
		h=1;										// Making Height Equal One
	}
	width = w;
	height = h;
	return 1;								// Return TRUE

}




int cGraphics::initGL(void)
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
    glDisable( GL_CULL_FACE );

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);


	return 1;										// Initialization Went OK
}


void cGraphics::killGLWindow(void)								// Properly Kill The Window
{
	SDL_ShowCursor(0);
	world.unload();

	TextureCache.unload(splash);
	unsigned int i;
	for(i = 0; i < gatTextures.size(); i++)
		TextureCache.unload(gatTextures[i]);

	TextureCache.unload(gatBorder);

	for(i = 0; i < waterCount; i++)
	{
		for(unsigned int ii = 0; ii < waterTextures[i].size(); ii++)
		{
			TextureCache.unload(waterTextures[i][ii]);
		}
	}

	WM.unload();

	return;
}



extern cGraphics 		Graphics;

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

cGraphics::cGraphics()
{
	width=1024;
	height=768;
	bits=32;
	fullscreen=false;
	selectedObjectProp = 0;
	showgrid = true;
	brushsize = 1;
	texturestart = 0;
	
	textureRot = 0;
	fliph = false;
	flipv = false;
	showObjects = false;
	selectedObject = -1;
	objectStartDrag = false;
	slope = false;
	quadtreeView = -1;
	showBoundingBoxes = false;
	gatType = 0;
	showLightmaps = false;
	showTileColors = true;
	showWater = true;
	showOglLighting = true;
	lasttick = 0;
	wallHeightMin = cVector2(-1,-1);
	wallHeightMax = cVector2(-1,-1);
	texturePreview = NULL;
	gridsize = 1;
	gridoffsetx = 0;
	gridoffsety = 0;
	topCamera = false;
	showambientlighting = true;
	groupeditmode = false;
	animateWater = true;
	
	showNoTiles = true;
	selectionCenter = cVector3(-1,-1,-1);
	showgat = false;
	backgroundColor = cVector3(0,0,0);
	noTileColor = cVector3(1,1,1);
	showDot = true;
	showSprites = true;
	showAllLights = false;
	clearLightmaps = false;
	gatTransparency = 0.3f;
	textureTool = TOOL_BRUSH;
	textureBrush.resize(2, std::vector<bool>(4,false));
	textureBrush[0][0] = true;
	textureBrush[0][1] = true;
	textureBrush[0][2] = true;
	textureBrush[0][3] = true;
	textureBrush[1][0] = true;
	textureBrushOffset = cVector2(0,0);
	textureGridSizeX = 4;
	textureGridSizeY = 4;
	textureBrushSize = 1;
}


