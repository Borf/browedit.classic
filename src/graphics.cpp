#define __GRAPHICS_CPP__
#include "graphics.h"

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
#include <undo.h>
#include "settings.h"
#include "menu.h"

extern void												ChangeGrid();
extern cMenu*											currentobject;
extern long												lastmotion;

int cGraphics::draw(bool drawwm)
{
	frameTicks = SDL_GetTicks() - lastTick;
	lastTick += frameTicks;

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode

	if(worldContainer)
	{
		if(worldContainer->view.topCamera)
		{
			worldContainer->settings.lightPosition[0] = cMouse::x3d;
			worldContainer->settings.lightPosition[1] = -cMouse::z3d;
			worldContainer->settings.lightPosition[2] = 1000;
			worldContainer->settings.lightPosition[3] = 1.0f;
		}
		else
		{
			worldContainer->settings.lightPosition[0] = -worldContainer->camera.pointer.x + worldContainer->camera.height*sin(worldContainer->camera.rot);
			worldContainer->settings.lightPosition[1] = 10+worldContainer->camera.height+worldContainer->camera.angle;
			worldContainer->settings.lightPosition[2] = -worldContainer->camera.pointer.y + worldContainer->camera.height*cos(worldContainer->camera.rot);
			worldContainer->settings.lightPosition[3] = 1.0f;
		}
		glLightfv(GL_LIGHT1, GL_POSITION, worldContainer->settings.lightPosition);			// Position The Light
		glEnable(GL_LIGHTING);
	}
	else
		glDisable(GL_LIGHTING);


	if(world)
		world->draw();

	glDisable(GL_LIGHTING);



	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,width,0,height,-10000,10000);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

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
		if (cSettings::editMode == MODE_OBJECTS)
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
			previewModel->rot.y+=40*(frameTicks / 1000.0f);
		}
		glEnable(GL_DEPTH_TEST);*/
	}
	if(world && world->loaded && cSettings::editMode != MODE_OBJECTS && cSettings::editMode != MODE_OBJECTGROUP && cSettings::editMode != MODE_LIGHTS)
	{
		int i;
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		for(i = 0; 288*i+256 < height; i++)
		{
			if (cSettings::editMode == MODE_GAT)
			{
				if (i+worldContainer->settings.texturestart > (int)gatTextures.size()-1)
					continue;
				glBindTexture(GL_TEXTURE_2D, gatTextures[i+worldContainer->settings.texturestart]->texId());
			}
			else if (cSettings::editMode == MODE_WATER)
			{
				if (i+world->water.type >= (int)waterCount)
					continue;
				static float frame = 0;
				glBindTexture(GL_TEXTURE_2D, waterTextures[i+world->water.type][(int)floor(frame)]->texId());
				frame+=0.25;
				if (frame >= waterTextures[i+world->water.type].size())
					frame = 0;
			}
			else
			{
				if (i+worldContainer->settings.texturestart >= (int)world->textures.size())
					continue;
				glBindTexture(GL_TEXTURE_2D, world->textures[i+worldContainer->settings.texturestart]->texId());
			}
			glBegin(GL_QUADS);
				glTexCoord2f(1,1);		glVertex2f( width, height-(32+288*i));
				glTexCoord2f(1,0);		glVertex2f( width, height-(32+288*i+256));
				glTexCoord2f(0,0);		glVertex2f( width-256, height-(32+288*i+256));
				glTexCoord2f(0,1);		glVertex2f( width-256, height-(32+288*i));
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		if(cSettings::editMode == MODE_TEXTUREPAINT)
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
				if (cSettings::editMode == MODE_GAT)
				{
					if (i+worldContainer->settings.texturestart > 6)
						continue;
				}
				else if (cSettings::editMode == MODE_WATER)
				{
					if (i+world->water.type > 5)
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
				glVertex2f( worldContainer->settings.selectionstart.x,	height-worldContainer->settings.selectionstart.y);
				glVertex2f( worldContainer->settings.selectionstart.x,	height-worldContainer->settings.selectionend.y);
				glVertex2f( worldContainer->settings.selectionend.x,	height-worldContainer->settings.selectionend.y);
				glVertex2f( worldContainer->settings.selectionend.x,	height-worldContainer->settings.selectionstart.y);
			glEnd();
		}
		glColor3f(1,1,1);
	}
	else if (cSettings::editMode == MODE_OBJECTGROUP)
	{
		char buf[100];
		if (!groupeditmode)
			sprintf(buf, "Selecting");
		else
			sprintf(buf, "Editing");
		font->print(1,1,1,width-font->textLen(buf), height-40, buf);
	}
	else if (!world || !world->loaded)
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
	menu->update();
	menu->draw();

	if(drawwm)
		cWM::draw();


	char buf[100];
	if(currentobject != NULL)
		sprintf(buf, "Editmode: %s, File: %s", 	cSettings::editMode == MODE_TEXTURE ?			GetMsg("menu/editmode/TEXTUREEDIT") : 
												cSettings::editMode == MODE_HEIGHTDETAIL ?		GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
												cSettings::editMode == MODE_HEIGHTGLOBAL ?		GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
												cSettings::editMode == MODE_WALLS ?			GetMsg("menu/editmode/WALLEDIT") :
												cSettings::editMode == MODE_OBJECTS ?			GetMsg("menu/editmode/OBJECTEDIT") :
												cSettings::editMode == MODE_GAT ?				GetMsg("menu/editmode/GATEDIT") :
												cSettings::editMode == MODE_WATER ?			GetMsg("menu/editmode/WATEREDIT") :
												cSettings::editMode == MODE_EFFECTS ?			GetMsg("menu/editmode/EFFECTSEDIT") :
												cSettings::editMode == MODE_SOUNDS ?			GetMsg("menu/editmode/SOUNDSEDIT") :
												cSettings::editMode == MODE_LIGHTS ?			GetMsg("menu/editmode/LIGHTSEDIT") :
												cSettings::editMode == MODE_OBJECTGROUP ?		GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
												cSettings::editMode == MODE_SPRITE ?			GetMsg("menu/editmode/SPRITEEDIT") : 
												cSettings::editMode == MODE_TEXTUREPAINT ?		GetMsg("menu/editmode/TEXTUREPAINTEDIT") : 
	
	"", 
		((cMenuItem*)currentobject)->data2.c_str());
	else
		sprintf(buf, "Editmode: %s",	cSettings::editMode == MODE_TEXTURE ?			GetMsg("menu/editmode/TEXTUREEDIT") : 
										cSettings::editMode == MODE_HEIGHTDETAIL ?		GetMsg("menu/editmode/DETAILTERRAINEDIT") : 
										cSettings::editMode == MODE_HEIGHTGLOBAL ?		GetMsg("menu/editmode/GLOBALHEIGHTEDIT") :
										cSettings::editMode == MODE_WALLS ?			GetMsg("menu/editmode/WALLEDIT") :
										cSettings::editMode == MODE_OBJECTS ?			GetMsg("menu/editmode/OBJECTEDIT") :
										cSettings::editMode == MODE_GAT ?				GetMsg("menu/editmode/GATEDIT") :
										cSettings::editMode == MODE_WATER ?			GetMsg("menu/editmode/WATEREDIT") :
										cSettings::editMode == MODE_EFFECTS ?			GetMsg("menu/editmode/EFFECTSEDIT") :
										cSettings::editMode == MODE_SOUNDS ?			GetMsg("menu/editmode/SOUNDSEDIT") :
										cSettings::editMode == MODE_LIGHTS ?			GetMsg("menu/editmode/LIGHTSEDIT") :
										cSettings::editMode == MODE_OBJECTGROUP ?		GetMsg("menu/editmode/OBJECTGROUPEDIT") : 
										cSettings::editMode == MODE_SPRITE ?			GetMsg("menu/editmode/SPRITEEDIT") : 
										cSettings::editMode == MODE_TEXTUREPAINT ?		GetMsg("menu/editmode/TEXTUREPAINTEDIT") : 
	"");
	float l = font->textLen(buf);
	font->print(0,0,0,width-l,height-14,buf);


	if (SDL_GetTicks() - lastmotion > 500)
	{
		cWindow* w = cWM::inWindow();
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
					int len = font->textLen(popup);
					glBegin(GL_QUADS);
						glVertex2f(cMouse::x-2, h()-cMouse::y-2);
						glVertex2f(cMouse::x+len+2, h()-cMouse::y-2);
						glVertex2f(cMouse::x+len+2, h()-cMouse::y+16);
						glVertex2f(cMouse::x-2, h()-cMouse::y+16);
					glEnd();
					font->print(1,1,1,cMouse::x, h()-cMouse::y, "%s", popup.c_str());
					glEnable(GL_DEPTH_TEST);
				}
			}
		}
	}

	if(popupMenu != NULL)
	{
		glDisable(GL_DEPTH_TEST);
		popupMenu->draw();
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


int cGraphicsBase::init(int pWidth, int pHeight, int pBpp, bool pFullscreen)
{
	width = pWidth;
	height = pHeight;
	bits = pBpp;
	fullscreen = pFullscreen;

	Log(3,0,GetMsg("graphics/INIT"));	
	int flags = 0;
	const SDL_VideoInfo* info = NULL;
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO ) < 0 ) {
		Log( 1,0, GetMsg("graphics/INITFAIL"), SDL_GetError( ) );
		return -1;
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
		return -1;
	}
	return 0;

}




int cGraphics::init(int pWidth, int pHeight, int pBpp, bool pFullscreen)
{
	cGraphicsBase::init(pWidth, pHeight, pBpp, pFullscreen);

	Log(3,0,GetMsg("graphics/OPENINGTEXTURES"));

	glDisable(GL_LIGHTING);
	font = new cFont();
	font->load("data/fonts/"+cSettings::fontName+".tga");
	splash = cTextureCache::load(cSettings::config.FirstChildElement("config")->FirstChildElement("splash")->FirstChild()->Value());
	Log(3,0,GetMsg("graphics/INITIALIZINGWM"));
	cWM::init(cSettings::skinFile);
	cWM::addWindow(new cHotkeyWindow());

	unsigned int i;
	for(i = 0; i < gatTiles.size(); i++)
	{
		char buf[64];
		sprintf(buf, "data/gat%i.tga", gatTiles[i]);
		gatTextures.push_back(cTextureCache::load(buf));
	}
	gatBorder = cTextureCache::load("data/gatBorder.tga");

	
	waterDirectory =		cSettings::config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("directory")->FirstChild()->Value();
	waterExtension =		cSettings::config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("extension")->FirstChild()->Value();
	waterCount=atoi(cSettings::config.FirstChildElement("config")->FirstChildElement("water")->FirstChildElement("count")->FirstChild()->Value());

	waterTextures.resize(waterCount);
	for(i = 0; i < waterCount; i++)
	{
		for(int ii = 0; ii < 32; ii++)
		{
			char buf[100];
			sprintf(buf, "%s%swater%i%02i%s", cSettings::roDir.c_str(), waterDirectory.c_str(), i, ii, waterExtension.c_str());
			waterTextures[i].push_back(cTextureCache::load(buf, TEX_NOCLAMP));
		}
	}
	if(waterCount == 0)
	{
		waterTextures.resize(1);
		waterTextures[i].push_back(cTextureCache::load(cSettings::roDir + waterDirectory + "water" + waterExtension));
	}

	Log(3,0,GetMsg("file/DONELOADING"), "water.txt");

	previewModel = NULL;

	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);										// Enable Light One
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_COLOR_MATERIAL);


	/*popupMenu = new cMenu();
	popupMenu->parent = NULL;
	popupMenu->drawstyle = 1;
	popupMenu->x = 100;
	popupMenu->y = 100;
	popupMenu->w = 150;
	popupMenu->opened = true;
	cMenuItem* mm;
	cMenu* favs;
	ADDMENU(favs,		popupMenu, "Favorites",		popupMenu->x + 150,200); // File
	favs->y = 100;

	ADDMENUITEM(mm,popupMenu,"Disable Shadows",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupMenu,"Snap to floor",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupMenu,"Set to 50 over floor",		&MenuCommand_new); //new
	ADDMENUITEM(mm,popupMenu,"Set as sunlight",		&MenuCommand_new); //new

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










int cGraphicsBase::resizeGLScene(GLsizei w, GLsizei h)	// Resize And Initialize The GL Window
{
	if (h==0)										// Prevent A Divide By Zero By
	{
		h=1;										// Making Height Equal One
	}
	width = w;
	height = h;
	return 1;								// Return TRUE

}




int cGraphicsBase::initGL(void)
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

void cGraphics::closeAndCleanup()
{
	cGraphicsBase::killGLWindow();
	world->unload();
	
	cTextureCache::unload(splash);
	unsigned int i;
	for(i = 0; i < gatTextures.size(); i++)
		cTextureCache::unload(gatTextures[i]);
	
	cTextureCache::unload(gatBorder);
	
	for(i = 0; i < waterCount; i++)
	{
		for(unsigned int ii = 0; ii < waterTextures[i].size(); ii++)
		{
			cTextureCache::unload(waterTextures[i][ii]);
		}
	}
}


void cGraphicsBase::killGLWindow(void)								// Properly Kill The Window
{
	SDL_ShowCursor(0);
}


bool cGraphics::is3dSelected(float x, float y, float z)
{
	if(worldContainer->settings.selectionstart3d.x < worldContainer->settings.selectionend3d.x)
	{
		if(x < worldContainer->settings.selectionstart3d.x || x > worldContainer->settings.selectionend3d.x)
			return false;
	}
	else
	{
		if(x > worldContainer->settings.selectionstart3d.x || x < worldContainer->settings.selectionend3d.x)
			return false;
	}


/*	if(selectionstart3d.y < selectionend3d.y)
	{
		if(y < selectionstart3d.y || y > selectionend3d.y)
			return false;
	}
	else
	{
		if(y > selectionstart3d.y || y < selectionend3d.y)
			return false;
	}*/

	if(worldContainer->settings.selectionstart3d.z < worldContainer->settings.selectionend3d.z)
	{
		if(z < worldContainer->settings.selectionstart3d.z || z > worldContainer->settings.selectionend3d.z)
			return false;
	}
	else
	{
		if(z > worldContainer->settings.selectionstart3d.z || z < worldContainer->settings.selectionend3d.z)
			return false;
	}

	return true;
}



bool cGraphics::objectStartDrag = false;
bool cGraphics::slope = false;
int cGraphics::quadtreeView = -1;
int cGraphics::gatType = 0;
cTexture* cGraphics::texturePreview = NULL;
bool cGraphics::groupeditmode = false;

cVector3 cGraphics::selectionCenter = cVector3(-1,-1,-1);
cVector3 cGraphics::backgroundColor = cVector3(0,0,0);
cVector3 cGraphics::noTileColor = cVector3(1,1,1);
bool cGraphics::clearLightmaps = false;
float cGraphics::gatTransparency = 0.3f;
eTool cGraphics::textureTool = TOOL_BRUSH;
std::vector<std::vector<bool> > cGraphics::textureBrush;/*resize(2, std::vector<bool>(4,false));
cBrowGraphics::textureBrush[0][0] = true;
cBrowGraphics::textureBrush[0][1] = true;
cBrowGraphics::textureBrush[0][2] = true;
cBrowGraphics::textureBrush[0][3] = true;
cBrowGraphics::textureBrush[1][0] = true;*/
cVector2 cGraphics::textureBrushOffset = cVector2(0,0);
float cGraphics::textureGridSizeX = 4;
float cGraphics::textureGridSizeY = 4;
int cGraphics::textureBrushSize = 1;

std::vector<int> cGraphics::gatTiles;
cRSMModel* cGraphics::previewModel;
unsigned int cGraphics::waterCount;
cTexture* cGraphics::splash = NULL;
std::vector<std::vector<cTexture*> > cGraphics::waterTextures;
std::vector<cTexture*> cGraphics::gatTextures;
int cGraphics::previewColor;
cFont* cGraphics::font;
std::string cGraphics::waterExtension;
std::string cGraphics::waterDirectory;
cTexture* cGraphics::gatBorder;





cWorld*							cGraphics::world = NULL;
cWorldContainer*				cGraphics::worldContainer = NULL;
std::vector<cWorldContainer*>	cGraphics::worlds;
cWorldContainer::cView			cGraphics::view;




cGraphics::cGraphics()
{
	cGraphicsBase::cGraphicsBase();
}



void cGraphics::newWorld()
{
	world = new cWorld();
	worldContainer = new cWorldContainer(world);
	view = worldContainer->view;
	worlds.insert(worlds.begin(), worldContainer);
	updateMenu();
}


extern cMenu* openMaps;

void cGraphics::updateMenu()
{
	unsigned int i;
	for(i = 0; i < openMaps->items.size(); i++)
		delete openMaps->items[i];
	openMaps->items.clear();

	for(i = 0; i < worlds.size(); i++)
	{
		cMenuItem* mm;
		ADDMENUITEM(mm,openMaps,worlds[i]->world->fileName,	&MenuCommand_switchMap);
		mm->data3 = i;
	}

}

int					cGraphicsBase::width =				1024;
int					cGraphicsBase::height =				768;
int					cGraphicsBase::bits =				32;
bool				cGraphicsBase::fullscreen =			false;
long 				cGraphicsBase::lastTick =			0;
long				cGraphicsBase::frameTicks =			0;
cMenu*				cGraphicsBase::popupMenu =			NULL;
cMenu*				cGraphicsBase::menu =				NULL;
long				cGraphicsBase::dragoffsety =		0;
long				cGraphicsBase::dragoffsetx =		0;
bool				cGraphicsBase::cMouse::lbuttondown =false;
bool				cGraphicsBase::cMouse::rbuttondown =false;
long				cGraphicsBase::cMouse::x =			0;
long				cGraphicsBase::cMouse::y =			0;
long				cGraphicsBase::cMouse::xOld =		0;
long				cGraphicsBase::cMouse::yOld =		0;
long				cGraphicsBase::cMouse::xStart =		0;
long				cGraphicsBase::cMouse::yStart =		0;
double				cGraphicsBase::cMouse::x3d =		0;
double				cGraphicsBase::cMouse::y3d =		0;
double				cGraphicsBase::cMouse::z3d =		0;
double				cGraphicsBase::cMouse::x3dStart =	0;
double				cGraphicsBase::cMouse::y3dStart =	0;
double				cGraphicsBase::cMouse::z3dStart =	0;
bool				cGraphicsBase::cMouse::doubleClick =false;
long				cGraphicsBase::cMouse::lastlclick =	0;
long				cGraphicsBase::cMouse::lastrclick =	0;
double				cGraphicsBase::cMouse::click3dx	=	0;
double				cGraphicsBase::cMouse::click3dy	=	0;
double				cGraphicsBase::cMouse::click3dz	=	0;


cGraphicsBase::cGraphicsBase()
{
}

long cGraphicsBase::getFrameTicks()
{
	return frameTicks;	
}


cWorldContainer::cCamera::cCamera()
{
	angle = 0;
	height = 123;
	rot = 0.0f;
	pointer = cVector2(-774,-963.5);
}

cWorldContainer::cWorldContainer( cWorld* w )
{
	world = w;
	undoStack = new cUndoStack();
}

cWorldContainer::~cWorldContainer()
{
	if(world)
		delete world;
	world = NULL;
	if(undoStack)
		delete undoStack;
	undoStack = NULL;
}
cWorldContainer::cView::cView()
{
	showLightmaps = false;
	showTileColors = true;
	showWater = true;
	showNoTiles = true;
	showGat = false;
	showDot = true;
	showSprites = true;
	showAllLights = false;
	showGrid = true;
	showObjects = false;
	showBoundingBoxes = false;
	showObjectsAsTransparent = false;
	showWaterAnimation = true;
	topCamera = false;
	sideCamera = false;
}

cWorldContainer::cSettings::cSettings()
{
	gridSize			= 1;
	gridoffsetx			= 0;
	gridoffsety			= 0;
	selectedObject		= 0;
	brushSizeDetailHeight= 1;
	brushsize			= 1;
	texturestart		= 0;
	textureRot			= 0;
	fliph				= false;
	flipv				= false;
	selectionstart		= cVector2(0,0);
	selectionend		= cVector2(0,0);
	wallHeightMin		= cVector2(-1,-1);
	wallHeightMax		= cVector2(-1,-1);
}
