#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern std::vector<std::vector<int > > clipboardgat;

std::vector<std::vector<cTile> > clipboardtexture;

int cProcessManagement::textureedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(mousestartx < Graphics.w()-256)
				{
					float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
					float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

					selsizex = floor(selsizex*Graphics.brushsize);
					selsizey = floor(selsizey*Graphics.brushsize);

					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					float selstartx = ((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f);
					float selstarty = ((int)(Graphics.selectionstart.y - 32) % 288) / 32;
					float selendx = ((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f);
					float selendy = ((int)(Graphics.selectionend.y - 32) % 288) / 32;

					selstartx += (Graphics.w()%32)/32.0f;
					selendx += (Graphics.w()%32)/32.0f;

					glColor4f(1,1,1,0.7f);
					glEnable(GL_BLEND);

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

					undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));


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
							t.color[0] = (char)255;
							t.color[1] = (char)255;
							t.color[2] = (char)255;
							t.color[3] = (char)255;
							t.texture = Graphics.texturestart + (int)(Graphics.selectionstart.y - 32) / 288;
							if(Graphics.clearLightmaps)
								t.lightmap = 0;
							else
							{
								if(Graphics.world.cubes[y][x].tileUp != -1)
									t.lightmap = Graphics.world.tiles[Graphics.world.cubes[y][x].tileUp].lightmap;
							}
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
								t.u1 = ((selendx+selstartx)/8.0)-t.u1;
								t.u2 = ((selendx+selstartx)/8.0)-t.u2;
								t.u3 = ((selendx+selstartx)/8.0)-t.u3;
								t.u4 = ((selendx+selstartx)/8.0)-t.u4;
							}
							if(Graphics.flipv)
							{
								t.v1 = ((selendy+selstarty)/8.0)-t.v1;
								t.v2 = ((selendy+selstarty)/8.0)-t.v2;
								t.v3 = ((selendy+selstarty)/8.0)-t.v3;
								t.v4 = ((selendy+selstarty)/8.0)-t.v4;
							}

//									cTile* tt = &Graphics.world.tiles[Graphics.world.cubes[y][x].tileUp];
							if(y >= 0 && y < Graphics.world.height && x >= 0 && x < Graphics.world.width)
							{
								Graphics.world.tiles.push_back(t);
								Graphics.world.cubes[y][x].tileUp = Graphics.world.tiles.size()-1;
							}
						}
					}
				}
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_MINUS:
				if (Graphics.brushsize > 0.125)
					Graphics.brushsize/=2.0f;
				break;
			case SDLK_EQUALS:
				Graphics.brushsize*=2;
				break;
			case SDLK_SPACE:
				Graphics.texturerot = (Graphics.texturerot + 1) % 4;
				break;
			case SDLK_h:
				Graphics.fliph = !Graphics.fliph;
				break;
			case SDLK_v:
				Graphics.flipv = !Graphics.flipv;
				break;
			case SDLK_c:
				if(mouseX < Graphics.w()-256)
				{
					float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
					float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

					selsizex = floor(selsizex*Graphics.brushsize);
					selsizey = floor(selsizey*Graphics.brushsize);

					if (Graphics.texturerot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}

					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					clipboardtexture.clear();
					clipboardgat.clear();

					for(int x = posx; x > posx-selsizex; x--)
					{
						std::vector<cTile> row;
						std::vector<int> row2;
						for(int y = posy; y > posy-selsizey; y--)
						{
							if(x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
							{
								if(Graphics.world.cubes[y][x].tileUp != -1)
								{
									row.push_back(Graphics.world.tiles[Graphics.world.cubes[y][x].tileUp]);
									row2.push_back(1);
								}
								else
								{
									row.push_back(cTile());
									row2.push_back(2);
								}
							}
							else
							{
								row.push_back(cTile());
								row2.push_back(0);
							}

						}
						clipboardtexture.push_back(row);
						clipboardgat.push_back(row2);
					}						
				}
				break; 
			case SDLK_p:
				if(mouseX < Graphics.w()-256)
				{
					float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
					float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

					selsizex = floor(selsizex*Graphics.brushsize);
					selsizey = floor(selsizey*Graphics.brushsize);

					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					if (Graphics.texturerot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}
					if(clipboardgat.size() != selsizex || selsizex == 0)
						break;
					if(clipboardgat[0].size() != selsizey)
						break;

					
					undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));

					for(int x = posx; x > posx-selsizex; x--)
					{
						for(int y = posy; y > posy-selsizey; y--)
						{
							int xx = posx - x;
							int yy = posy - y;
							if(clipboardgat[xx][yy] == 2)
							{
								Graphics.world.cubes[y][x].tileUp = -1;
							}
							if(clipboardgat[xx][yy] == 1)
							{
								Graphics.world.tiles.push_back(clipboardtexture[xx][yy]);
								Graphics.world.cubes[y][x].tileUp = Graphics.world.tiles.size()-1;
							}
						}
					}						
				}
				break;
			case SDLK_BACKSPACE:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
					float selsizey = (fabs((int)Graphics.selectionstart.y - Graphics.selectionend.y) / 32);
					selsizex = (int)floor(selsizex*Graphics.brushsize);
					selsizey = (int)floor(selsizey*Graphics.brushsize);

					if (Graphics.texturerot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}


					undostack.push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));
					for(int x = posx; x > posx-selsizex; x--)
					{
						for(int y = posy; y > posy-selsizey; y--)
						{
							if (y < 0 || y >= Graphics.world.height)
								continue;
							if (x < 0 || x >= Graphics.world.width)
								continue;
							Graphics.world.cubes[y][x].tileUp = -1;
						}
					}
					break;
				}
			case SDLK_i:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					if(posx > -1 && posy > -1 && posx < Graphics.world.width && posy < Graphics.world.height)
					{
						Log(3,0,"Cube (%i,%i): %f,%f,%f,%f", posx, posy, Graphics.world.cubes[posy][posx].cell1, Graphics.world.cubes[posy][posx].cell2, Graphics.world.cubes[posy][posx].cell3, Graphics.world.cubes[posy][posx].cell4);
						Log(3,0,"tileUp: %i", Graphics.world.cubes[posy][posx].tileUp);
						if(Graphics.world.cubes[posy][posx].tileUp != -1)
							Log(3,0,"Lightmap: %i", Graphics.world.tiles[Graphics.world.cubes[posy][posx].tileUp].lightmap);
					}

					break;
				}
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
