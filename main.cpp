#define _MAIN_CPP_
#include "common.h"
#include "filesystem.h"
#include <math.h>
#include "main.h"
#include "menu.h"
#include <fstream>
#include <list>
#include <gd/gd.h>

#ifdef WIN32
#include <windows.h>
#include <SDL/SDL_syswm.h>
#endif

cFileSystem fs;

#define VERTEXDIST 10

string inputboxresult;


list<cGraphics> undos;
void MakeUndo();
void Undo();

void ChangeGrid();
void UpdateTriangleMenu();
void CleanSurfaces();
int process_events();
bool running = true;
eMode editmode = MODE_TEXTURE;
float paintspeed = 0.1f;
extern double mouse3dx, mouse3dy, mouse3dz;

string rodir;

#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)

int brushsize;

//file
MENUCOMMAND(new);
MENUCOMMAND(open);
MENUCOMMAND(save);
MENUCOMMAND(saveAs);
MENUCOMMAND(exit);
MENUCOMMAND(steepness);
MENUCOMMAND(random1);
MENUCOMMAND(shading);
MENUCOMMAND(exportheight);
MENUCOMMAND(textures);
MENUCOMMAND(mode);
MENUCOMMAND(flatten);
MENUCOMMAND(editrandom);
MENUCOMMAND(grid);
MENUCOMMAND(mode_detail);
MENUCOMMAND(speed);
MENUCOMMAND(fill);

cMenu*	menu;
cMenu* grid;

int cursorsize = 1;

#define ADDMENUITEM(m, p, t, pr) m = new cMenuItem(); m->title = t; m->item = true; m->drawstyle = 1; ((cMenuItem*)m)->proc = pr; p->items.push_back(m);
#define ADDMENU(m,p,t,xpos,width) m = new cMenu(); m->title = t; m->item = false; m->drawstyle = 1; m->y = 20; m->x = xpos; m->w = width; p->items.push_back(m);
cMenu* mode;
cMenu* editdetail;
cMenu* speed;
int main(int argc, char *argv[])
{

	cMenu* file;
	cMenu* mm;
	cMenu* steepness;
	cMenu* rnd;
	cMenu* view;
	cMenu* edit;

	menu = new cMenu();
	menu->title = "root";
	menu->item = false;
	menu->drawstyle = 0;
	menu->opened = true;
	menu->x = 0;
	menu->y = 0;
	menu->w = Graphics.w();
	
	
	ADDMENU(file,		menu, "File",				0,100);
	ADDMENU(rnd,		menu, "Generate",			100,100);
	ADDMENU(view,		menu, "View",				200,50);
	ADDMENU(mode,		menu, "Edit Mode",			250,100);
	ADDMENU(edit,		menu, "Edit",				350,100);
	
	ADDMENU(steepness,	edit, "Steepness...",		480,100);

	ADDMENUITEM(mm,file,"New"	,				&MenuCommand_new);
	ADDMENUITEM(mm,file,"Open",					&MenuCommand_open);
	ADDMENUITEM(mm,file,"Save",					&MenuCommand_save);
	ADDMENUITEM(mm,file,"Save As",				&MenuCommand_saveAs);
	ADDMENUITEM(mm,file,"Export Heightmaps",	&MenuCommand_exportheight);
	ADDMENUITEM(mm,file,"Exit",					&MenuCommand_exit);

	ADDMENUITEM(mm,steepness, "0.5", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "1.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "1.5", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "2.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "4.0", &MenuCommand_steepness);
	ADDMENUITEM(mm,steepness, "8.0", &MenuCommand_steepness);
	
	ADDMENUITEM(mm,rnd, "Random 1", &MenuCommand_random1);

	ADDMENUITEM(mm,view,"Textures",&MenuCommand_textures);
	ADDMENUITEM(mm,view,"Grid",&MenuCommand_grid);
	mm->ticked = true;
	grid = mm;
	
	ADDMENUITEM(mm,mode,"Texture Edit",			&MenuCommand_mode);
	mm->ticked = true;
	ADDMENUITEM(mm,mode,"Global Heightmap Edit",	&MenuCommand_mode);
	ADDMENU(editdetail,mode,"Detail Terrain Edit...",400,100);


	ADDMENUITEM(mm, editdetail, "1", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "2", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "4", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "8", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "16", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "32", &MenuCommand_mode_detail);
	ADDMENUITEM(mm, editdetail, "64", &MenuCommand_mode_detail);

	ADDMENUITEM(mm,mode,"Wall Edit",	&MenuCommand_mode);


	ADDMENUITEM(mm,edit,"Flatten map",			&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,"Round Flatten",		&MenuCommand_flatten);
	ADDMENUITEM(mm,edit,"Randomize a bit",		&MenuCommand_editrandom);
	ADDMENUITEM(mm,edit,"Fill",					&MenuCommand_fill);

	ADDMENU(speed,edit, "Speed", 480, 100);
	ADDMENUITEM(mm,speed,"5",&MenuCommand_speed);
	mm->ticked = true;
	ADDMENUITEM(mm,speed,"10",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"15",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"20",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"25",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"30",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"40",&MenuCommand_speed);
	ADDMENUITEM(mm,speed,"50",&MenuCommand_speed);
	
	changetoserverdir();				
	log_open("log_worldeditor.txt","worldedit",2);
	fs.LoadFile("data.dat");
	
	cFile* pFile = fs.open("config.txt");
	if (pFile != NULL)
	{
		rodir = pFile->readline();
	}
	else
		rodir = "c:\\program files\\gravity\\ro\\data\\";

	lastlclick = 0;
	lastrclick = 0;
	
	if (!Graphics.init())
		return 1;

	Graphics.world.newworld();
	strcpy(Graphics.world.filename, string(rodir + "customtown").c_str());
	Graphics.world.load();
	long lasttimer = SDL_GetTicks();
	while( running ) {
		process_events( );														// process keypresses
		if (!Graphics.draw())												// Active?  Was There A Quit Received?
			break;
		SDL_GL_SwapBuffers();
		Sleep(1);																// god save the CPU
	}

	// Shutdown
	Graphics.KillGLWindow();						// Kill The Window
	log_close();
	return 0;							// Exit The Program
}


/**
 * Container for process handlers
 */
class cProcessManagement
{
public:
	int main_process_events(SDL_Event &);
};
cProcessManagement processManagement;

/**
 * Main Process Management loop, decides what process handlers to call
 */
int process_events()
{
    SDL_Event event;
	bool refresh = true;

    while(refresh ? SDL_PollEvent(&event) : SDL_WaitEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F4:
				if(SDL_GetModState() & KMOD_ALT)
					running = false;
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
			default:
				break;
			}
			default:
				break;
		}
		
		
		if (showmessage)
		{
			if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONUP)
				showmessage = false;
		}
			processManagement.main_process_events(event);

		if(!refresh)
			break;
	}
	return 0;
}


float steepness = 3;
void checkheight(int x, int y)
{
}

/**
 * Main mode process handler
 */
int cProcessManagement::main_process_events(SDL_Event &event)
{
	switch(event.type)
	{
	case SDL_MOUSEMOTION:
		{
			dragged = true;
			mousex = event.motion.x;
			mousey = event.motion.y;
			cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
			if(m != NULL)
				break;

			if (rbuttondown && !lbuttondown)
			{
				if(SDL_GetModState() & KMOD_SHIFT)
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						Graphics.cameraangle += (oldmousey - mousey) / 10.0f;
						Graphics.cameraangle = max(min(Graphics.cameraangle, 20), -10);
						Graphics.camerarot += (oldmousex - mousex) / 100.0f;
						while(Graphics.camerarot < 0)
							Graphics.camerarot+=2*(float)PI;
						while(Graphics.camerarot > 2*PI)
							Graphics.camerarot-=2*(float)PI;
					}
					else
					{
						Graphics.cameraheight += (oldmousey - mousey) / 2.0f;
						//Graphics.cameraheight = max(min(Graphics.cameraheight, 150), 10);
						Graphics.camerarot += (oldmousex - mousex) / 100.0f;
						while(Graphics.camerarot < 0)
							Graphics.camerarot+=2*(float)PI;
						while(Graphics.camerarot > 2*PI)
							Graphics.camerarot-=2*(float)PI;
					}
				}
				else if (SDL_GetModState() & KMOD_CTRL)
				{
					if (!(Graphics.selectionstart3d == Graphics.selectionend3d))
					{
					}
					float xoff = (mousex - oldmousex) * cos(Graphics.cameraangle / 180.0f * 3.1415);
					float yoff = mousey - oldmousey;
				}
				else
				{
					cVector2 v = cVector2((oldmousex - mousex),  (oldmousey - mousey)) / 1.0f;
		
					v.rotate(-Graphics.camerarot / PI * 180.0f);

					Graphics.camerapointer = Graphics.camerapointer - v;
					long size = Graphics.world.size;
/*					while (Graphics.camerapointer.x+(size/2) > size)
						Graphics.camerapointer.x -= size;
					while (Graphics.camerapointer.y+(size/2) > size)
						Graphics.camerapointer.y -= size;

					while (Graphics.camerapointer.x+(size/2) < 0)
						Graphics.camerapointer.x += size;
					while (Graphics.camerapointer.y+(size/2) < 0)
						Graphics.camerapointer.y += size;*/

				}
			}
			else if (lbuttondown)
			{
				if(startmousex > Graphics.w()-256)
				{
					Graphics.selectionstart.x = floor(startmousex / 32)*32;
					Graphics.selectionstart.y = floor(startmousey / 32)*32;
					Graphics.selectionend.x = ceil(mousex / 32)*32;
					Graphics.selectionend.y = ceil(mousey / 32)*32;
					if(Graphics.selectionstart.x > Graphics.selectionend.x)
					{
						float f = Graphics.selectionstart.x;
						Graphics.selectionstart.x = Graphics.selectionend.x;
						Graphics.selectionend.x = f;
					}
					if(Graphics.selectionstart.y > Graphics.selectionend.y)
					{
						float f = Graphics.selectionstart.y;
						Graphics.selectionstart.y = Graphics.selectionend.y;
						Graphics.selectionend.y = f;
					}
				}

			}
			oldmousex = mousex;
			oldmousey = mousey;
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			startmousex = mousex = event.motion.x;
			startmousey = mousey = event.motion.y;
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				lbuttondown = true;
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				if(m == NULL)
				{
					if (editmode == MODE_TEXTURE)
					{
						if(startmousex < Graphics.w()-256)
						{
							float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
							float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);
							selsizex = floor(selsizex*Graphics.brushsize);
							selsizey = floor(selsizey*Graphics.brushsize);
							int posx = mouse3dx / 10;
							int posy = mouse3dz / 10;

							float selstartx = ((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f);
							float selstarty = ((int)(Graphics.selectionstart.y - 32) % 288) / 32;
							float selendx = ((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f);
							float selendy = ((int)(Graphics.selectionend.y - 32) % 288) / 32;

							if (Graphics.texturerot % 2 == 1)
							{
								float a = selsizex;
								selsizex = selsizey;
								selsizey = a;

								a = selstartx;
								selstartx = selstarty;
								selstarty = a;

								a = selendx;
								selendx = selendy;
								selendy = a;

							}

							for(int x = posx; x > posx-selsizex; x--)
							{
								for(int y = posy; y > posy-selsizey; y--)
								{
									int xx = posx - x;
									int yy = posy - y;
									if (y < 0)
										continue;
									if (x < 0)
										continue;
									cTile t;
									t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
									t.lightmap = 0;
									if (Graphics.texturerot == 0)
									{
										t.u1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.v1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

										t.u2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.v2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
										
										t.u3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.v3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.u4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.v4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if (Graphics.texturerot == 1)
									{
										t.v1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.u1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

										t.v2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.u2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
										
										t.v3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
										t.u3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.v4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
										t.u4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if(Graphics.texturerot == 2)
									{
										t.u1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.v1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

										t.u2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.v2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
										
										t.u3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.v3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.u4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.v4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if (Graphics.texturerot == 3)
									{
										t.v1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.u1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

										t.v2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.u2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
										
										t.v3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
										t.u3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
										
										t.v4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
										t.u4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
									}
									if(Graphics.fliph)
									{
										t.u1 = 1-t.u1;
										t.u2 = 1-t.u2;
										t.u3 = 1-t.u3;
										t.u4 = 1-t.u4;
									}
									if(Graphics.flipv)
									{
										t.v1 = 1-t.v1;
										t.v2 = 1-t.v2;
										t.v3 = 1-t.v3;
										t.v4 = 1-t.v4;
									}
//									cTile* tt = &Graphics.world.tiles[Graphics.world.cubes[y][x].tileup];
									Graphics.world.tiles.push_back(t);
									Graphics.world.cubes[y][x].tileup = Graphics.world.tiles.size()-1;
								}
							}
						
						
						}
					}
					else if (editmode == MODE_HEIGHTDETAIL)
					{
						int posx = mouse3dx / 10;
						int posy = mouse3dz / 10;

						if (posx >= brushsize && posx < Graphics.world.width-brushsize && posy >= brushsize && posy< Graphics.world.height-brushsize)
						{
							glColor4f(1,0,0,1);
							glDisable(GL_TEXTURE_2D);
							glDisable(GL_BLEND);
							for(int x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
							{
								for(int y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
								{
									cCube* c = &Graphics.world.cubes[y][x];
									if(lbuttondown && !rbuttondown)
									{
										c->cell1-=1;
										c->cell2-=1;
										c->cell3-=1;
										c->cell4-=1;
									}
									if(lbuttondown && rbuttondown)
									{
										c->cell1+=1;
										c->cell2+=1;
										c->cell3+=1;
										c->cell4+=1;
									}
								}
							}
						}

					}
				}
			}
			else // rbutton
			{
				rbuttondown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				lbuttondown = false;
				bool doubleclick = false;
				long l = SDL_GetTicks();
				if (l - lastlclick < 250)
				{
					Log(3,0,"Doubleclick");
					doubleclick = true;
					lastlclick = SDL_GetTicks();
				}
				else
					lastlclick = SDL_GetTicks();
				lbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;
				menu->unmouseover();
				cMenu* m = menu->inwindow((int)mousex, Graphics.h()-(int)mousey);
				if (m == NULL)
				{
					menu->closemenu();
					menu->opened = true;
				}
				if (m != NULL && m->opened)
				{
					m->click((int)mousex, Graphics.h()-(int)mousey);
				}
				else // no menu
				{
					if (!(Graphics.selectionstart == Graphics.selectionend))
					{
						
					}
				}
				break;
				
			}
			else // right button
			{
				rbuttondown = false;
				bool doubleclick = false;
				long l = SDL_GetTicks();
				if (l - lastrclick < 250)
				{
					Log(3,0,"Doubleclick");
					doubleclick = true;
					lastrclick = 0;
				}
				else
					lastrclick = SDL_GetTicks();
				
				lastrclick = SDL_GetTicks();
				rbuttondown = false;
				mousex = event.motion.x;
				mousey = event.motion.y;

				
				break;
			}
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				Graphics.camerapointer.y++;
				if (Graphics.camerapointer.y > Graphics.world.size)
					Graphics.camerapointer.y-= Graphics.world.size;
				break;
			case SDLK_DOWN:
				Graphics.camerapointer.y--;
				if (Graphics.camerapointer.y < 0)
					Graphics.camerapointer.y+= Graphics.world.size;
				break;
			case SDLK_RIGHT:
				Graphics.camerapointer.x++;
				if (Graphics.camerapointer.x > Graphics.world.size)
					Graphics.camerapointer.x-= Graphics.world.size;
				break;
			case SDLK_LEFT:
				Graphics.camerapointer.x--;
				if (Graphics.camerapointer.x < 0)
					Graphics.camerapointer.x+= Graphics.world.size;
				break;
			case SDLK_MINUS:
				Graphics.brushsize/=2;
				break;
			case SDLK_EQUALS:
				Graphics.brushsize*=2;
				break;
			case SDLK_LEFTBRACKET:
				Graphics.texturestart--;
				if (Graphics.texturestart < 0)
					Graphics.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				Graphics.texturestart++;
				if (Graphics.texturestart >= Graphics.world.textures.size() - (Graphics.h() / 288))
					Graphics.texturestart--;
				break;
			case SDLK_SPACE:
				Graphics.texturerot = (Graphics.texturerot + 1) % 4;
				break;
			case SDLK_h:
				if (editmode == MODE_TEXTURE)
					Graphics.fliph = !Graphics.fliph;
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;
					if(Graphics.world.cubes[y][x].tileside == -1)
						break;
					float f;
					f = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1 = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2 = f;
					f = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3 = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4 = f;
				}
				break;
			case SDLK_v:
				if (editmode == MODE_TEXTURE)
					Graphics.flipv = !Graphics.flipv;
				if(editmode == MODE_WALLS)
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;
					if(Graphics.world.cubes[y][x].tileside == -1)
						break;
					float f;
					f = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v1;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v1 = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v3;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v3 = f;
					f = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v2;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v2 = Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v4;
					Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].v4 = f;
				}
				break;
			case SDLK_g:
				MenuCommand_grid((cMenuItem*)grid);
				break;
			case SDLK_d:
				{
					int posx = mouse3dx / 10;
					int posy = mouse3dz / 10;

					if (posx >= brushsize && posx < Graphics.world.width-brushsize && posy >= brushsize && posy< Graphics.world.height-brushsize)
					{
						Log(3,0,"Pos: %i,%i", posx, posy);
						Log(3,0,"Cube: tileup: %i, tileside: %i, tileaside: %i, v1: %f, v2: %f, v3: %f, v4: %f", Graphics.world.cubes[posy][posx].tileup, Graphics.world.cubes[posy][posx].tileside, Graphics.world.cubes[posy][posx].tileaside, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
						Log(3,0,"Tileup: texture: %i, v1: (%f,%f), v2: (%f,%f), v3: (%f,%f), v4: (%f,%f)", Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].texture, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v1, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v2, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v3, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].u4, Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileup].v4);
					}
				}
				break;
			case SDLK_COMMA:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(Graphics.world.cubes[y][x].tileside != -1)
					{
						Graphics.world.cubes[y][x].tileside = -1;
						break;
					}

					cTile t;
					t.texture = Graphics.texturestart;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileside = Graphics.world.tiles.size()-1;

					break;
				}
			case SDLK_PERIOD:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(Graphics.world.cubes[y][x].tileaside != -1)
					{
						Graphics.world.cubes[y][x].tileaside = -1;
						break;
					}

					cTile t;
					t.texture = Graphics.texturestart;
					t.lightmap = 0;
					t.u1 = 0;
					t.v1 = 0;

					t.u2 = 1;
					t.v2 = 0;
					
					t.u3 = 0;
					t.v3 = 1;
					
					t.u4 = 1;
					t.v4 = 1;
					Graphics.world.tiles.push_back(t);
					Graphics.world.cubes[y][x].tileaside = Graphics.world.tiles.size()-1;

					break;
				}
			case SDLK_PAGEDOWN:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4+=0.03125;
						}
					}
					else
					{
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3+=0.03125;
					}
					break;
				}
			case SDLK_PAGEUP:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4-=0.03125;
						}
					}
					else
					{
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3-=0.03125;
					}
					break;
				}
			case SDLK_HOME:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2+=0.03125;
						}
					}
					else
					{
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
					}
					break;
				}
			case SDLK_END:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						int xdiff = 4;

						for(xx = xmin; xx < xmax; xx++)
						{
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2-=0.03125;
						}
					}
					else
					{
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4-=0.03125;
					}
					break;
				}
			case SDLK_w:
				{
					int x = mouse3dx / 10;
					int y = mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(Graphics.world.cubes[y][x].tileside == -1)
						break;

					int xx = x;
					while(Graphics.world.cubes[y][xx].tileside != -1)
						xx++;
					int xmax = xx;
					xx = x;
					while(Graphics.world.cubes[y][xx].tileside != -1)
						xx--;
					int xmin = xx+1;
					int xdiff = 4;

					for(xx = xmin; xx < xmax; xx++)
					{
						cTile t;
						t.texture = Graphics.texturestart;
						t.lightmap = 0;
						t.u1 = (xx-xmin) *  (1.0/(float)xdiff);
						t.v1 = 0;

						t.u2 = (xx-xmin+1) *  (1.0/(float)xdiff);
						t.v2 = 0;
						
						t.u3 = (xx-xmin) *  (1.0/(float)xdiff);
						t.v3 = 1;
						
						t.u4 = (xx-xmin+1) *  (1.0/(float)xdiff);
						t.v4 = 1;
						Graphics.world.tiles.push_back(t);
						Graphics.world.cubes[y][xx].tileside = Graphics.world.tiles.size()-1;
					}

					break;
				}
			default:
				break;
		}

	}
	return true;
}



void MakeUndo()
{
	undos.push_back(Graphics);
}

void Undo()
{
	if(undos.size() > 0)
	{
		Graphics = undos.back();
		undos.pop_back();
	}
}



MENUCOMMAND(new)
{

	return true;
}



MENUCOMMAND(open)
{
#ifdef WIN32
	char curdir[100];
	getcwd(curdir, 100);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version)	;
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;

	strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO Maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	if (GetOpenFileName(&ofn))
	{
		while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
		Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';

		chdir(curdir);
		Graphics.world.load();
	}
#else


#endif
	return true;
}


MENUCOMMAND(save)
{
#ifdef WIN32
	if(Graphics.world.filename[0] == '\0')
	{
		char curdir[100];
		getcwd(curdir, 100);
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);
		HWND hWnd = wmInfo.window;
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
		ofn.lpstrFile = Graphics.world.filename;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
				Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
			Graphics.world.save();
		}
		chdir(curdir);
	}
	else
		Graphics.world.save();
#else

#endif

	return true;
}
MENUCOMMAND(saveAs)
{
#ifdef WIN32
	char curdir[100];
	getcwd(curdir, 100);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	strcpy(Graphics.world.filename, replace(Graphics.world.filename, "/", "\\").c_str());
	ofn.lpstrFile = Graphics.world.filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO maps\0*.rsw\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&ofn))
	{
		Graphics.world.filename[0] = '\0';
		chdir(curdir);
	}
	else
	{
		while(Graphics.world.filename[strlen(Graphics.world.filename)-1] != '.')
			Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
		Graphics.world.filename[strlen(Graphics.world.filename)-1] = '\0';
		chdir(curdir);
		Graphics.world.save();

	}
#else
	message = "Sorry, you can't save-as in linux, since I don't know how to make a save-as box yet";
	showmessage = true;
#endif
	return true;
}
MENUCOMMAND(exit)
{
	exit(0);
	return true;
}	
MENUCOMMAND(undo)
{
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_u;
	SDL_PushEvent(&ev);
	return true;
}


int ClassifyPoint(cVector3 point, cVector3 pO, cVector3 pN)
{
	cVector3 TempVect;
	TempVect.x = pO.x - point.x;
	TempVect.y = pO.y - point.y;
	TempVect.z = pO.z - point.z;
	cVector3 dir = TempVect;
	GLfloat d = dir.Dot(pN);;
	
	if (d < -0.00001f)
		return 1;
	else
		if (d > 0.00001f)
			return -1;
		return 0;
}




MENUCOMMAND(lightmaps)
{
	return true;
}



MENUCOMMAND(lightmapsreset)
{

	return true;
}


MENUCOMMAND(steepness)
{
	steepness = atof(src->title.c_str());
	return true;
}


MENUCOMMAND(random1)
{
	int x,y;
	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileaside = -1;
			Graphics.world.cubes[y][x].tileside = -1;
		}
	}
	for(y = 2; y < Graphics.world.height/2-1; y++)
	{
		for(x = 2; x < Graphics.world.width/2-1; x++)
		{
		//	Graphics.world.cubes[2*y][2*x].tileup = 1;

			Graphics.world.cubes[2*y][2*x].cell1 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell2 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell3 = -rand()%10;
			Graphics.world.cubes[2*y][2*x].cell4 = -rand()%10;

			Graphics.world.cubes[2*y][2*x-1].cell2 =	Graphics.world.cubes[2*y][2*x].cell1;
			Graphics.world.cubes[2*y-1][2*x-1].cell4 =	Graphics.world.cubes[2*y][2*x].cell1;
			Graphics.world.cubes[2*y-1][2*x].cell3 =	Graphics.world.cubes[2*y][2*x].cell1;


			Graphics.world.cubes[2*y][2*x+1].cell1 =	Graphics.world.cubes[2*y][2*x].cell2;
			Graphics.world.cubes[2*y-1][2*x+1].cell3 =	Graphics.world.cubes[2*y][2*x].cell2;
			Graphics.world.cubes[2*y-1][2*x].cell4 =	Graphics.world.cubes[2*y][2*x].cell2;
		

			Graphics.world.cubes[2*y][2*x-1].cell4 =	Graphics.world.cubes[2*y][2*x].cell3;
			Graphics.world.cubes[2*y+1][2*x-1].cell2 =	Graphics.world.cubes[2*y][2*x].cell3;
			Graphics.world.cubes[2*y+1][2*x].cell1 =	Graphics.world.cubes[2*y][2*x].cell3;

			Graphics.world.cubes[2*y][2*x+1].cell3 =	Graphics.world.cubes[2*y][2*x].cell4;
			Graphics.world.cubes[2*y+1][2*x+1].cell1 =	Graphics.world.cubes[2*y][2*x].cell4;
			Graphics.world.cubes[2*y+1][2*x].cell2 =	Graphics.world.cubes[2*y][2*x].cell4;
		
		}
	}




	return true;
}


MENUCOMMAND(exportheight)
{
	return true;
}
MENUCOMMAND(mode)
{
	string title = src->title;
	for(int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = false;
	src->ticked = true;

	if(title == "Global Terrain Edit")
	{
		editmode = MODE_HEIGHTGLOBAL;
	}
	else if (title == "Detail Terrain Edit...")
	{
		editmode = MODE_HEIGHTDETAIL;
	}
	else if (title == "Texture Edit")
	{
		editmode = MODE_TEXTURE;
	}
	else if (title == "Wall Edit")
	{
		editmode = MODE_WALLS;
	}
	return true;
}
MENUCOMMAND(textures)
{
	Graphics.world.showtextures = !Graphics.world.showtextures;
	src->ticked = !src->ticked;
	return true;
}

MENUCOMMAND(flatten)
{
	for(int y = 0; y < Graphics.world.height; y++)
	{
		for(int x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileaside = -1;
			Graphics.world.cubes[y][x].tileside = -1;
			Graphics.world.cubes[y][x].cell1 = 0;
			Graphics.world.cubes[y][x].cell2 = 0;
			Graphics.world.cubes[y][x].cell3 = 0;
			Graphics.world.cubes[y][x].cell4 = 0;
		}
	}
	return true;	
}

MENUCOMMAND(editrandom)
{
	return true;
}

MENUCOMMAND(grid)
{
	src->ticked = !src->ticked;
	Graphics.showgrid = src->ticked;
	return true;
}

MENUCOMMAND(mode_detail)
{
	for(int i = 0; i < mode->items.size(); i++)
		mode->items[i]->ticked = (mode->items[i]->title == "Detail Terrain Edit" ? true : false);
	for(i = 0; i < editdetail->items.size(); i++)
		editdetail->items[i]->ticked = false;
	src->ticked = true;
	editmode = MODE_HEIGHTDETAIL;
	brushsize = atoi(src->title.c_str());

	return true;
}
MENUCOMMAND(speed)
{
	for(int i =0 ; i < speed->items.size(); i++)
		speed->items[i]->ticked = false;
	src->ticked = true;
	paintspeed = atof(src->title.c_str()) / 50.0f;
	return true;
}

MENUCOMMAND(fill)
{
	int x,y;

	int startid = Graphics.world.tiles.size();

	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 4; x++)
		{
			cTile t;
			t.lightmap = 0;
			t.texture = Graphics.texturestart;
			t.u1 = x/4.0;
			t.v1 = y/4.0;
			t.u2 = (x+1)/4.0;
			t.v2 = (y)/4.0;
			t.u3 = (x)/4.0;
			t.v3 = (y+1)/4.0;
			t.u4 = (x+1)/4.0;
			t.v4 = (y+1)/4.0;
			Graphics.world.tiles.push_back(t);
		}
	}


	for(y = 0; y < Graphics.world.height; y++)
	{
		for(x = 0; x < Graphics.world.width; x++)
		{
			Graphics.world.cubes[y][x].tileup = startid + (x%4) + 4*(y % 4);
		}
	}
	return true;
}