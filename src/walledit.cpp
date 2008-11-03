#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
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
				if (y < 0 || y > cGraphics::world->height-1)
					break;
				if (x < 0 || x > cGraphics::world->width-1)
					break;
				if (cGraphics::wallHeightMin == cVector2(x,y))
					cGraphics::wallHeightMin = cVector2(-1,-1);
				else
					cGraphics::wallHeightMin = cVector2(x,y);
			}
			else if(event.button.button == SDL_BUTTON_RIGHT && movement < 3)
			{
				int x = (int)mouse3dx / 10;
				int y = (int)mouse3dz / 10;
				if (y < 0 || y > cGraphics::world->height-1)
					break;
				if (x < 0 || x > cGraphics::world->width-1)
					break;
				if (cGraphics::wallHeightMax == cVector2(x,y))
					cGraphics::wallHeightMax = cVector2(-1,-1);
				else
					cGraphics::wallHeightMax = cVector2(x,y);

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
					std::vector<std::pair<int, cTile> > tilesedited;

					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(cGraphics::world->cubes[y][x].tileOtherSide == -1)
							break;

						int yy = y;
						while(cGraphics::world->cubes[yy][x].tileOtherSide != -1)
							yy++;
						int ymax = yy;
						yy = y;
						while(cGraphics::world->cubes[yy][x].tileOtherSide != -1)
							yy--;
						int ymin = yy+1;

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &cGraphics::world->tiles[cGraphics::world->cubes[yy][x].tileOtherSide];
							tilesedited.push_back(std::pair<int, cTile>(cGraphics::world->cubes[yy][x].tileOtherSide, *t));
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
						if(cGraphics::world->cubes[y][x].tileSide == -1)
							break;

						int xx = x;
						while(cGraphics::world->cubes[y][xx].tileSide != -1)
							xx++;
						int xmax = xx;
						xx = x;
						while(cGraphics::world->cubes[y][xx].tileSide != -1)
							xx--;
						int xmin = xx+1;

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide];
							tilesedited.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, *t));
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
					std::vector<std::pair<int, cTile> > tilesedited;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0)
						break;
					if (x < 0)
						break;

					if(SDL_GetModState() & KMOD_ALT)
					{
						if(cGraphics::world->cubes[y][x].tileOtherSide == -1)
							break;

						int yy = y;
						int ymax = yy+1;
						int ymin = yy;
						int ydiff = 2;

						if (SDL_GetModState() & KMOD_SHIFT)
						{
							yy = y;
							while(cGraphics::world->cubes[yy][x].tileOtherSide != -1)
								yy++;
							ymax = yy;
							yy = y;
							while(cGraphics::world->cubes[yy][x].tileOtherSide != -1)
								yy--;
							ymin = yy+1;
							ydiff = 4;
						}

						for(yy = ymin; yy < ymax; yy++)
						{
							cTile* t = &cGraphics::world->tiles[cGraphics::world->cubes[yy][x].tileOtherSide];
							tilesedited.push_back(std::pair<int, cTile>(cGraphics::world->cubes[yy][x].tileOtherSide, *t));
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
						if(cGraphics::world->cubes[y][x].tileSide == -1)
							break;

						int xx = x;
						int xmax = xx+1;
						int xmin = xx;
						int xdiff = 4;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							xx = x;
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx++;
							xmax = xx;
							xx = x;
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx--;
							xmin = xx+1;
							xdiff = 4;
						}

						for(xx = xmin; xx < xmax; xx++)
						{
							cTile* t = &cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide];
							tilesedited.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, *t));
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
					std::vector<std::pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 

						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide]));
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v3+=0.03125;
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v4+=0.03125;
						}
					}
					else
					{
							tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][x].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide]));
							cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u1+=0.03125;
							cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u3+=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_PAGEUP:
				{
					std::vector<std::pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 

						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide]));
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v3-=0.03125;
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v4-=0.03125;
						}
					}
					else
					{
						tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][x].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide]));
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u1-=0.03125;
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u3-=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_HOME:
				{
					std::vector<std::pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						for(xx = xmin; xx < xmax; xx++)
						{
							if (y < 0 || y > cGraphics::world->height)
								continue;
							if (xx < 0 || xx > cGraphics::world->width)
								continue;

							tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide]));
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v1+=0.03125;
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v2+=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > cGraphics::world->height-1)
							break;
						if (x < 0 || x > cGraphics::world->width-1)
							break;
						tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][x].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide]));
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u2+=0.03125;
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u4+=0.03125;
					}
					if(tileschanged.size() > 0)
						undostack.push(new cUndoTileEdit(tileschanged));
				}
				break;
			case SDLK_END:
				{
					std::vector<std::pair<int, cTile> > tileschanged;
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if(SDL_GetModState() & KMOD_SHIFT)
					{
						int xx = x;
						int xmax;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx++;
							xmax = xx;
						}
						else
							xmax = xx+1;
						xx = x;
						int xmin;
						if (SDL_GetModState() & KMOD_CTRL)
						{
							while(cGraphics::world->cubes[y][xx].tileSide != -1)
								xx--;
							xmin = xx+1;
						}
						else
							xmin = xx;
						 
						for(xx = xmin; xx < xmax; xx++)
						{
							tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][xx].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide]));
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v1-=0.03125;
							cGraphics::world->tiles[cGraphics::world->cubes[y][xx].tileSide].v2-=0.03125;
						}
					}
					else
					{
						if (y < 0 || y > cGraphics::world->height)
							break;
						if (x < 0 || x > cGraphics::world->width)
							break;
						tileschanged.push_back(std::pair<int, cTile>(cGraphics::world->cubes[y][x].tileSide, cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide]));
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u2+=0.03125;
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u4+=0.03125;
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u2-=0.03125;
						cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileSide].u4-=0.03125;
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
					if (y < 0 || y > cGraphics::world->height - 1)
						break;
					if (x < 0 || x > cGraphics::world->width - 1)
						break;

					float selstartx = (((cGraphics::selectionstart.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
					float selstarty = (((int)(cGraphics::selectionstart.y - 32) % 288) / 32) / 8.0f;
					float selendx = (((cGraphics::selectionend.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
					float selendy = (((int)(cGraphics::selectionend.y - 32) % 288) / 32) / 8.0f;
					float selheight = selendy - selstarty;
					float selwidth = selendx - selstartx;

					std::vector<std::pair<std::pair<int,int>, int> > wallschanged;
					if(SDL_GetModState() & KMOD_ALT)
					{
						if(cGraphics::world->cubes[y][x].tileOtherSide == -1)
							break;

						int yy = y;
						while(cGraphics::world->cubes[yy][x].tileOtherSide != -1 && yy < cGraphics::world->height-1)
							yy++;
						int ymax = yy;
						yy = y;
						while(cGraphics::world->cubes[yy][x].tileOtherSide != -1 && yy >= 0)
						{
							yy--;
							if(yy == -1)
								break;
						}
						int ymin = yy+1;

					
						float vmin = 99999, vmax = -99999;
						for(yy = ymin; yy < ymax; yy++)
						{
							vmin = min(vmin, cGraphics::world->cubes[yy][x].cell4/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[yy][x].cell4/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[yy][x].cell2/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[yy][x].cell2/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[yy][x+1].cell3/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[yy][x+1].cell3/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[yy][x+1].cell1/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[yy][x+1].cell1/32.0f);
						}
						if (!(cGraphics::wallHeightMin == cVector2(-1,-1)))
						{
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell4/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell2/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell3/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell1/32.0f);
						}
						if (!(cGraphics::wallHeightMax == cVector2(-1,-1)))
						{
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell4/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell2/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell3/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell1/32.0f);
						}



						for(yy = ymin; yy < ymax; yy++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = cGraphics::texturestart + ((int)cGraphics::selectionstart.y - 32) / 288;
							t.lightmap = 0;
							if(wrap)
							{

								int gridsize = (int)(cGraphics::gridsize * 4);
								int gridoff = (int)(cGraphics::gridoffsetx * gridsize);
								t.u1 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((yy+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((yy+gridoff)%gridsize) *  ((float)selwidth/gridsize);


								if (cGraphics::world->cubes[yy][x].cell4 > cGraphics::world->cubes[yy][x+1].cell3)
								{
									t.v3 = selstarty+(cGraphics::world->cubes[yy][x+1].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty+(cGraphics::world->cubes[yy][x+1].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v1 = selendy+(cGraphics::world->cubes[yy][x].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v2 = selendy+(cGraphics::world->cubes[yy][x].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (cGraphics::world->cubes[yy][x].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v2 = selstarty + (cGraphics::world->cubes[yy][x].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (cGraphics::world->cubes[yy][x+1].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (cGraphics::world->cubes[yy][x+1].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
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
							cGraphics::world->tiles.push_back(t);
							wallschanged.push_back(std::pair<std::pair<int,int>,int>(std::pair<int,int>(x,yy),cGraphics::world->cubes[yy][x].tileOtherSide));
							cGraphics::world->cubes[yy][x].tileOtherSide = cGraphics::world->tiles.size()-1;
						}
						if(wallschanged.size() > 0)
							undostack.push(new cUndoChangeWalls(1, wallschanged));
					}
					else
					{
						if(cGraphics::world->cubes[y][x].tileSide == -1)
							break;

						int xx = x;
						while(cGraphics::world->cubes[y][xx].tileSide != -1 && xx < cGraphics::world->width)
							xx++;
						int xmax = xx;
						xx = x;
						while(cGraphics::world->cubes[y][xx].tileSide != -1 && xx >= 0)
						{
							xx--;
							if(xx == -1)
								break;
						}
						int xmin = xx+1;

						float vmin = 99999, vmax = -99999;
						for(xx = xmin; xx < xmax; xx++)
						{
							vmin = min(vmin, cGraphics::world->cubes[y][xx].cell3/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[y][xx].cell3/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[y][xx].cell4/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[y][xx].cell4/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[y+1][xx].cell1/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[y+1][xx].cell1/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[y+1][xx].cell2/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[y+1][xx].cell2/32.0f);
						}
						if (!(cGraphics::wallHeightMin == cVector2(-1,-1)))
						{
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell4/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell2/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell3/32.0f);
							vmin = min(vmin, cGraphics::world->cubes[(int)cGraphics::wallHeightMin.y][(int)cGraphics::wallHeightMin.x].cell1/32.0f);
						}
						if (!(cGraphics::wallHeightMax == cVector2(-1,-1)))
						{
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell4/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell2/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell3/32.0f);
							vmax = max(vmax, cGraphics::world->cubes[(int)cGraphics::wallHeightMax.y][(int)cGraphics::wallHeightMax.x].cell1/32.0f);
						}


						for(xx = xmin; xx < xmax; xx++)
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = cGraphics::texturestart + ((int)cGraphics::selectionstart.y - 32) / 288;
							t.lightmap = 0;


							if (wrap)
							{
								int gridsize = (int)(cGraphics::gridsize * 4);
								int gridoff = (int)(cGraphics::gridoffsetx * gridsize);

								t.u1 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u2 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);
								t.u3 = selstartx + ((xx+gridoff)%gridsize) *  ((float)selwidth/gridsize);
								t.u4 = selstartx + ((xx+gridoff)%gridsize+1) *  ((float)selwidth/gridsize);


								if (cGraphics::world->cubes[y][xx].cell3 > cGraphics::world->cubes[y+1][xx].cell1)
								{
									t.v1 = selendy + (cGraphics::world->cubes[y][xx].cell3/32.0f-vmax)/(vmax-vmin)*selheight;
							 		t.v2 = selendy + (cGraphics::world->cubes[y][xx].cell4/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v3 = selstarty + (cGraphics::world->cubes[y+1][xx].cell1/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v4 = selstarty + (cGraphics::world->cubes[y+1][xx].cell2/32.0f-vmin)/(vmax-vmin)*selheight;
								}
								else
								{
									t.v1 = selstarty + (cGraphics::world->cubes[y][xx].cell3/32.0f-vmin)/(vmax-vmin)*selheight;
							 		t.v2 = selstarty + (cGraphics::world->cubes[y][xx].cell4/32.0f-vmin)/(vmax-vmin)*selheight;
									t.v3 = selendy + (cGraphics::world->cubes[y+1][xx].cell1/32.0f-vmax)/(vmax-vmin)*selheight;
									t.v4 = selendy + (cGraphics::world->cubes[y+1][xx].cell2/32.0f-vmax)/(vmax-vmin)*selheight;
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

							cGraphics::world->tiles.push_back(t);
							wallschanged.push_back(std::pair<std::pair<int,int>,int>(std::pair<int,int>(xx,y),cGraphics::world->cubes[y][xx].tileSide));
							cGraphics::world->cubes[y][xx].tileSide = cGraphics::world->tiles.size()-1;
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
					if (y < 0 || y > (int)cGraphics::world->cubes.size()-1)
						break;
					if (x < 0 || x > (int)cGraphics::world->cubes[0].size()-1)
						break;

					undostack.push(new cUndoChangeWall(0,x,y, cGraphics::world->cubes[y][x].tileSide));

					int from = x;
					int to = x+1;

					if(event.key.keysym.mod&KMOD_CTRL)
					{
						while( (cGraphics::world->cubes[y][from].cell1 +
								cGraphics::world->cubes[y][from].cell2) !=
							   (cGraphics::world->cubes[y+1][from].cell3 + 
							    cGraphics::world->cubes[y+1][from].cell4))
						{
							from--;
							if(from == -1)
								break;
						}


						from++;

						while( (cGraphics::world->cubes[y][to].cell1 +
								cGraphics::world->cubes[y][to].cell2) !=
							   (cGraphics::world->cubes[y+1][to].cell3 + 
							    cGraphics::world->cubes[y+1][to].cell4))
						{
							to++;
							if(to == cGraphics::world->width)
								break;
						}
					
					}


					bool set = cGraphics::world->cubes[y][x].tileSide != -1;

					for(int xx = from; xx < to; xx++)
					{
						if(set)
						{
							cGraphics::world->cubes[y][xx].tileSide = -1;
						}
						else
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = cGraphics::texturestart + ((int)cGraphics::selectionstart.y - 32) / 288;
							t.lightmap = 0;
							float selstartx = (((cGraphics::selectionstart.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
							float selstarty = (((int)(cGraphics::selectionstart.y - 32) % 288) / 32) / 8.0f;
							float selendx = (((cGraphics::selectionend.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
							float selendy = (((int)(cGraphics::selectionend.y - 32) % 288) / 32) / 8.0f;

							
							t.u1 = selstartx;
							t.v1 = selstarty;

							t.u2 = selendx;
							t.v2 = selstarty;
							
							t.u3 = selstartx;
							t.v3 = selendy;
							
							t.u4 = selendx;
							t.v4 = selendy;
							cGraphics::world->tiles.push_back(t);
							cGraphics::world->cubes[y][xx].tileSide = cGraphics::world->tiles.size()-1;
						}
					}
				}
				break;
			case SDLK_PERIOD:
				{
					int x = (int)mouse3dx / 10;
					int y = (int)mouse3dz / 10;
					if (y < 0 || y > (int)cGraphics::world->cubes.size()-1)
						break;
					if (x <= 0 || x > (int)cGraphics::world->cubes[y].size()-2)
						break;

					undostack.push(new cUndoChangeWall(1,x,y, cGraphics::world->cubes[y][x].tileOtherSide));

					int from = y;
					int to = y+1;

					if(event.key.keysym.mod&KMOD_CTRL)
					{
						while( (cGraphics::world->cubes[from][x].cell1 +
								cGraphics::world->cubes[from][x].cell3) !=
							   (cGraphics::world->cubes[from][x+1].cell2 + 
							    cGraphics::world->cubes[from][x+1].cell4))
						{
							from--;
							if(from == -1)
								break;
						}


						from++;

						while( (cGraphics::world->cubes[to][x].cell1 +
								cGraphics::world->cubes[to][x].cell3) !=
							   (cGraphics::world->cubes[to][x+1].cell2 + 
							    cGraphics::world->cubes[to][x+1].cell4))
						{
							to++;
							if(to == cGraphics::world->height)
								break;
						}
					
					}


					bool set = cGraphics::world->cubes[y][x].tileOtherSide != -1;

					for(int yy = from; yy < to; yy++)
					{
						if(set)
							cGraphics::world->cubes[yy][x].tileOtherSide = -1;
						else
						{
							cTile t;
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = cGraphics::texturestart + (int)(cGraphics::selectionstart.y - 32) / 288;
							t.lightmap = 0;
							float selstartx = (((cGraphics::selectionstart.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
							float selstarty = (((int)(cGraphics::selectionstart.y - 32) % 288) / 32) / 8.0f;
							float selendx = (((cGraphics::selectionend.x - (cGraphics::w()-256)) / 32.0f)) / 8.0f;
							float selendy = (((int)(cGraphics::selectionend.y - 32) % 288) / 32) / 8.0f;

							
							t.u1 = selstartx;
							t.v1 = selstarty;

							t.u2 = selendx;
							t.v2 = selstarty;
							
							t.u3 = selstartx;
							t.v3 = selendy;
							
							t.u4 = selendx;
							t.v4 = selendy;
							cGraphics::world->tiles.push_back(t);
							cGraphics::world->cubes[yy][x].tileOtherSide = cGraphics::world->tiles.size()-1;
						}
					}
				}
				break;
			case SDLK_LEFTBRACKET:
				cGraphics::texturestart--;
				if (cGraphics::texturestart < 0)
					cGraphics::texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::texturestart++;
				if (cGraphics::texturestart > ((int)cGraphics::world->textures.size()) - (cGraphics::h() / 288))
					cGraphics::texturestart--;
				break;

			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
