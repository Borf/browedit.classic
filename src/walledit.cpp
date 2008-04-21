#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern int movement;

int cProcessManagement::walledit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{
				int x = (int)mouse3dx / 10;
				int y = (int)mouse3dz / 10;
				if (y < 0 || y > Graphics.world.height-1)
					break;
				if (x < 0 || x > Graphics.world.width-1)
					break;
				if (Graphics.wallheightmin == cVector2(x,y))
					Graphics.wallheightmin = cVector2(-1,-1);
				else
					Graphics.wallheightmin = cVector2(x,y);
			}
			else if(event.button.button == SDL_BUTTON_RIGHT && movement < 3)
			{
				int x = (int)mouse3dx / 10;
				int y = (int)mouse3dz / 10;
				if (y < 0 || y > Graphics.world.height-1)
					break;
				if (x < 0 || x > Graphics.world.width-1)
					break;
				if (Graphics.wallheightmax == cVector2(x,y))
					Graphics.wallheightmax = cVector2(-1,-1);
				else
					Graphics.wallheightmax = cVector2(x,y);

			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_h:
				{
					vector<pair<int, cTile> > tilesedited;

					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy++;
						int ymax = yy;
						yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1)
							yy--;
						int ymin = yy+1;

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[yy][x].tileaside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[yy][x].tileaside, *t));
							float f;
							f = t->u1;
							t->u1 = t->u2;
							t->u2 = f;

							f = t->u3;
							t->u3 = t->u4;
							t->u4 = f;
						}
					}
					else
					{
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

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, *t));
							float f;
							f = t->u1;
							t->u1 = t->u2;
							t->u2 = f;

							f = t->u3;
							t->u3 = t->u4;
							t->u4 = f;
						}
					}
					if (tilesedited.size() > 0)
						undostack.push(new cUndoTileEdit(tilesedited));
				}
				break;
			case SDLK_v:
				{
					vector<pair<int, cTile> > tilesedited;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						int ymax = yy+1;
						int ymin = yy;
						int ydiff = 2;

						if (SDL_GetModState() & KMOD_SHIFT)
						{
							yy = y;
							while(Graphics.world.cubes[yy][x].tileaside != -1)
								yy++;
							ymax = yy;
							yy = y;
							while(Graphics.world.cubes[yy][x].tileaside != -1)
								yy--;
							ymin = yy+1;
							ydiff = 4;
						}

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[yy][x].tileaside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[yy][x].tileaside, *t));
							float f;
							f = t->v1;
							t->v1 = t->v3;
							t->v3 = f;

							f = t->v2;
							t->v2 = t->v4;
							t->v4 = f;
						}
					}
					else
					{
						if(Graphics.world.cubes[y][x].tileside == -1)
							break;

						int xx = x;
						int xmax = xx+1;
						int xmin = xx;
						int xdiff = 4;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							xx = x;
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx++;
							xmax = xx;
							xx = x;
							while(Graphics.world.cubes[y][xx].tileside != -1)
								xx--;
							xmin = xx+1;
							xdiff = 4;
						}

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside];
							tilesedited.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, *t));
							float f;
							f = t->v1;
							t->v1 = t->v3;
							t->v3 = f;

							f = t->v2;
							t->v2 = t->v4;
							t->v4 = f;
						}
					}
					if (tilesedited.size() > 0)
						undostack.push(new cUndoTileEdit(tilesedited));
				}
				break;
			case SDLK_PAGEDOWN:
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
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
						 

						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4+=0.03125;
						}
					}
					else
					{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3+=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_PAGEUP:
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
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
						 

						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v3-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v4-=0.03125;
						}
					}
					else
					{
						tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u1-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u3-=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_HOME:
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
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
						 
						for(xx = xmin; xx < xmax; xx++)
						{
							if (y < 0 || y > Graphics.world.height)
								continue;
							if (xx < 0 || xx > Graphics.world.width)
								continue;

							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1+=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2+=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > Graphics.world.height-1)
							break;
						if (x < 0 || x > Graphics.world.width-1)
							break;
						tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_END:
				{
					vector<pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
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
						 
						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][xx].tileside, Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside]));
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v1-=0.03125;
							Graphics.world.tiles[Graphics.world.cubes[y][xx].tileside].v2-=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > Graphics.world.height)
							break;
						if (x < 0 || x > Graphics.world.width)
							break;
						tileschanged.push_back(pair<int, cTile>(Graphics.world.cubes[y][x].tileside, Graphics.world.tiles[Graphics.world.cubes[y][x].tileside]));
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4+=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u2-=0.03125;
						Graphics.world.tiles[Graphics.world.cubes[y][x].tileside].u4-=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_w:
				{
					bool wrap = true;
					if (SDL_GetModState() & KMOD_SHIFT)
						wrap = false;


					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > Graphics.world.height - 1)
						break;
					if (x < 0 || x > Graphics.world.width - 1)
						break;

					float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;
					float selheight = selendy - selstarty;
					float selwidth = selendx - selstartx;

					vector<pair<pair<int,int>, int> > wallschanged;
					if(SDL_GetModState() & KMOD_ALT)
					{
						if(Graphics.world.cubes[y][x].tileaside == -1)
							break;

						int yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1 && yy < Graphics.world.height-1)
							yy++;
						int ymax = yy;
						yy = y;
						while(Graphics.world.cubes[yy][x].tileaside != -1 && yy > 0)
							yy--;
						int ymin = yy+1;

					
						float vmin = 99999, vmax = -99999;
						for(yy = ymin; yy < ymax; yy++)
						{
							vmin = min(vmin, Graphics.world.cubes[yy][x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x+1].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x+1].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[yy][x+1].cell1/32.0f);
							vmax = max(vmax, Graphics.world.cubes[yy][x+1].cell1/32.0f);
						}
						if (!(Graphics.wallheightmin == cVector2(-1,-1)))
						{
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell1/32.0f);
						}



						for(yy = ymin; yy < ymax; yy++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;
							if(wrap)
							{

								int gridsize = (int)(Graphics.gridsize * 4);
								int gridoff = (int)(Graphics.gridoffsetx * gridsize);
								t.u1 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);


								if (Graphics.world.cubes[yy][x].cell4 > Graphics.world.cubes[yy][x+1].cell3)
								{
									t.v3 = selstarty+(Graphics.world.cubes[yy][x+1].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty+(Graphics.world.cubes[yy][x+1].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v1 = selendy+(Graphics.world.cubes[yy][x].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v2 = selendy+(Graphics.world.cubes[yy][x].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (Graphics.world.cubes[yy][x].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v2 = selstarty + (Graphics.world.cubes[yy][x].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (Graphics.world.cubes[yy][x+1].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (Graphics.world.cubes[yy][x+1].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
								}
							}
							else
							{
								int index = yy-ymin;
								t.v1 = selstarty;
								t.v2 = selstarty;
								t.v3 = selendy;
								t.v4 = selendy;

								t.u1 = selstartx + ((index+1)/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u2 = selstartx + (index/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u3 = selstartx + ((index+1)/(float)(ymax-ymin)) * (selendx-selstartx);
								t.u4 = selstartx + (index/(float)(ymax-ymin)) * (selendx-selstartx);
							}
							Graphics.world.tiles.push_back(t);
							wallschanged.push_back(pair<pair<int,int>,int>(pair<int,int>(x,yy),Graphics.world.cubes[yy][x].tileaside));
							Graphics.world.cubes[yy][x].tileaside = Graphics.world.tiles.size()-1;
						}
						if(wallschanged.size() > 0)
							undostack.push(new cUndoChangeWalls(1, wallschanged));
					}
					else
					{
						if(Graphics.world.cubes[y][x].tileside == -1)
							break;

						int xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1 && xx < Graphics.world.width)
							xx++;
						int xmax = xx;
						xx = x;
						while(Graphics.world.cubes[y][xx].tileside != -1 && xx > 0)
							xx--;
						int xmin = xx+1;

						float vmin = 99999, vmax = -99999;
						for(xx = xmin; xx < xmax; xx++)
						{
							vmin = min(vmin, Graphics.world.cubes[y][xx].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y][xx].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y][xx].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y][xx].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y+1][xx].cell1/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y+1][xx].cell1/32.0f);
							vmin = min(vmin, Graphics.world.cubes[y+1][xx].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[y+1][xx].cell2/32.0f);
						}
						if (!(Graphics.wallheightmin == cVector2(-1,-1)))
						{
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell4/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell2/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell3/32.0f);
							vmin = min(vmin, Graphics.world.cubes[(int)Graphics.wallheightmin.y][(int)Graphics.wallheightmin.x].cell1/32.0f);
						}
						if (!(Graphics.wallheightmax == cVector2(-1,-1)))
						{
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell4/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell2/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell3/32.0f);
							vmax = max(vmax, Graphics.world.cubes[(int)Graphics.wallheightmax.y][(int)Graphics.wallheightmax.x].cell1/32.0f);
						}


						for(xx = xmin; xx < xmax; xx++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;


							if (wrap)
							{
								int gridsize = (int)(Graphics.gridsize * 4);
								int gridoff = (int)(Graphics.gridoffsetx * gridsize);

								t.u1 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);


								if (Graphics.world.cubes[y][xx].cell3 > Graphics.world.cubes[y+1][xx].cell1)
								{
									t.v1 = selendy + (Graphics.world.cubes[y][xx].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
							 		t.v2 = selendy + (Graphics.world.cubes[y][xx].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v3 = selstarty + (Graphics.world.cubes[y+1][xx].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty + (Graphics.world.cubes[y+1][xx].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (Graphics.world.cubes[y][xx].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
							 		t.v2 = selstarty + (Graphics.world.cubes[y][xx].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (Graphics.world.cubes[y+1][xx].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (Graphics.world.cubes[y+1][xx].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
								}


							}
							else
							{
								int index = xx-xmin;
								t.v1 = selstarty;
								t.v2 = selstarty;
								t.v3 = selendy;
								t.v4 = selendy;

								t.u1 = selstartx + (index/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u2 = selstartx + ((index+1)/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u3 = selstartx + (index/(float)(xmax-xmin)) * (selendx-selstartx);
								t.u4 = selstartx + ((index+1)/(float)(xmax-xmin)) * (selendx-selstartx);
							}

							Graphics.world.tiles.push_back(t);
							wallschanged.push_back(pair<pair<int,int>,int>(pair<int,int>(xx,y),Graphics.world.cubes[y][xx].tileside));
							Graphics.world.cubes[y][xx].tileside = Graphics.world.tiles.size()-1;
						}
						if(wallschanged.size() > 0)
							undostack.push(new cUndoChangeWalls(0, wallschanged));
					}
				}
				break;
			case SDLK_COMMA:
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > (int)Graphics.world.cubes.size()-1)
						break;
					if (x < 0 || x > (int)Graphics.world.cubes[0].size()-1)
						break;

					undostack.push(new cUndoChangeWall(0,x,y, Graphics.world.cubes[y][x].tileside));

					int from = x;
					int to = x+1;

					if(event.key.keysym.mod&KMOD_CTRL)
					{
						while( (Graphics.world.cubes[y][from].cell1 +
								Graphics.world.cubes[y][from].cell2) !=
							   (Graphics.world.cubes[y+1][from].cell3 + 
							    Graphics.world.cubes[y+1][from].cell4))
						{
							if(from == 0)
								break;
							from--;
						}


						from++;

						while( (Graphics.world.cubes[y][to].cell1 +
								Graphics.world.cubes[y][to].cell2) !=
							   (Graphics.world.cubes[y+1][to].cell3 + 
							    Graphics.world.cubes[y+1][to].cell4))
						{
							to++;
							if(to == Graphics.world.width)
								break;
						}
					
					}


					bool set = Graphics.world.cubes[y][x].tileside != -1;

					for(int xx = from; xx < to; xx++)
					{
						if(set)
						{
							Graphics.world.cubes[y][xx].tileside = -1;
						}
						else
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + ((int)Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;
							float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
							float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
							float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
							float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;

							
							t.u1 = selstartx;
							t.v1 = selstarty;

							t.u2 = selendx;
							t.v2 = selstarty;
							
							t.u3 = selstartx;
							t.v3 = selendy;
							
							t.u4 = selendx;
							t.v4 = selendy;
							Graphics.world.tiles.push_back(t);
							Graphics.world.cubes[y][xx].tileside = Graphics.world.tiles.size()-1;
						}
					}
				}
				break;
			case SDLK_PERIOD:
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > (int)Graphics.world.cubes.size()-1)
						break;
					if (x <= 0 || x > (int)Graphics.world.cubes[y].size()-2)
						break;

					undostack.push(new cUndoChangeWall(1,x,y, Graphics.world.cubes[y][x].tileaside));

					int from = y;
					int to = y+1;

					if(event.key.keysym.mod&KMOD_CTRL)
					{
						while( (Graphics.world.cubes[from][x].cell1 +
								Graphics.world.cubes[from][x].cell3) !=
							   (Graphics.world.cubes[from][x+1].cell2 + 
							    Graphics.world.cubes[from][x+1].cell4))
						{
							if(from == 0)
								break;
							from--;
						}


						from++;

						while( (Graphics.world.cubes[to][x].cell1 +
								Graphics.world.cubes[to][x].cell3) !=
							   (Graphics.world.cubes[to][x+1].cell2 + 
							    Graphics.world.cubes[to][x+1].cell4))
						{
							to++;
							if(to == Graphics.world.height)
								break;
						}
					
					}


					bool set = Graphics.world.cubes[y][x].tileaside != -1;

					for(int yy = from; yy < to; yy++)
					{
						if(set)
							Graphics.world.cubes[yy][x].tileaside = -1;
						else
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + (int)(Graphics.selectionstart.y - 32) / 288;
							t.lightmap = 0;
							float selstartx = (((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
							float selstarty = (((int)(Graphics.selectionstart.y - 32) % 288) / 32) / 8.0f;
							float selendx = (((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f)) / 8.0f;
							float selendy = (((int)(Graphics.selectionend.y - 32) % 288) / 32) / 8.0f;

							
							t.u1 = selstartx;
							t.v1 = selstarty;

							t.u2 = selendx;
							t.v2 = selstarty;
							
							t.u3 = selstartx;
							t.v3 = selendy;
							
							t.u4 = selendx;
							t.v4 = selendy;
							Graphics.world.tiles.push_back(t);
							Graphics.world.cubes[yy][x].tileaside = Graphics.world.tiles.size()-1;
						}
					}
				}
				break;
			case SDLK_LEFTBRACKET:
				Graphics.texturestart--;
				if (Graphics.texturestart < 0)
					Graphics.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				Graphics.texturestart++;
				if (Graphics.texturestart > ((int)Graphics.world.textures.size()) - (Graphics.h() / 288))
					Graphics.texturestart--;
				break;

			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
