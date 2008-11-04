#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mouseX;
extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
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
				if(mousestartx < cGraphics::w()-256)
				{
					float selsizex = (fabs(cGraphics::selectionstart.x - cGraphics::selectionend.x) / 32);
					float selsizey = (fabs(cGraphics::selectionstart.y - cGraphics::selectionend.y) / 32);

					selsizex = floor(selsizex*cGraphics::brushsize);
					selsizey = floor(selsizey*cGraphics::brushsize);

					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					float selstartx = ((cGraphics::selectionstart.x - (cGraphics::w()-256)) / 32.0f);
					float selstarty = ((int)(cGraphics::selectionstart.y - 32) % 288) / 32;
					float selendx = ((cGraphics::selectionend.x - (cGraphics::w()-256)) / 32.0f);
					float selendy = ((int)(cGraphics::selectionend.y - 32) % 288) / 32;

					selstartx += (cGraphics::w()%32)/32.0f;
					selendx += (cGraphics::w()%32)/32.0f;

					glColor4f(1,1,1,0.7f);
					glEnable(GL_BLEND);

					if (cGraphics::textureRot % 2 == 1)
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

					cGraphics::worldContainer->undoStack->push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));


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
							t.texture = cGraphics::texturestart + (int)(cGraphics::selectionstart.y - 32) / 288;
							if(cGraphics::clearLightmaps)
								t.lightmap = 0;
							else
							{
								if(cGraphics::world->cubes[y][x].tileUp != -1)
									t.lightmap = cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp].lightmap;
							}
							if (cGraphics::textureRot == 0)
							{
								t.u1 = (selendx*cGraphics::brushsize-xx-1) * (1/(8.0f*cGraphics::brushsize));
								t.v1 = ((selendy*cGraphics::brushsize-yy-1) * (1/(8.0f*cGraphics::brushsize)));

								t.u2 = (selendx*cGraphics::brushsize-xx) * (1/(8.0f*cGraphics::brushsize));
								t.v2 = ((selendy*cGraphics::brushsize-yy-1) * (1/(8.0f*cGraphics::brushsize)));
								
								t.u3 = (selendx*cGraphics::brushsize-xx-1) * (1/(8.0f*cGraphics::brushsize));
								t.v3 = ((selendy*cGraphics::brushsize-yy) * (1/(8.0f*cGraphics::brushsize)));
								
								t.u4 = (selendx*cGraphics::brushsize-xx) * (1/(8.0f*cGraphics::brushsize));
								t.v4 = ((selendy*cGraphics::brushsize-yy) * (1/(8.0f*cGraphics::brushsize)));
							}
							if (cGraphics::textureRot == 1)
							{
								t.v1 = (selendx*cGraphics::brushsize-xx-1) * (1/(8.0f*cGraphics::brushsize));
								t.u1 = ((selstarty*cGraphics::brushsize+yy+1) * (1/(8.0f*cGraphics::brushsize)));

								t.v2 = (selendx*cGraphics::brushsize-xx) * (1/(8.0f*cGraphics::brushsize));
								t.u2 = ((selstarty*cGraphics::brushsize+yy+1) * (1/(8.0f*cGraphics::brushsize)));
								
								t.v3 = (selendx*cGraphics::brushsize-xx-1) * (1/(8.0f*cGraphics::brushsize));
								t.u3 = ((selstarty*cGraphics::brushsize+yy) * (1/(8.0f*cGraphics::brushsize)));
								
								t.v4 = (selendx*cGraphics::brushsize-xx) * (1/(8.0f*cGraphics::brushsize));
								t.u4 = ((selstarty*cGraphics::brushsize+yy) * (1/(8.0f*cGraphics::brushsize)));
							}
							if(cGraphics::textureRot == 2)
							{
								t.u1 = (selstartx*cGraphics::brushsize+xx+1) * (1/(8.0f*cGraphics::brushsize));
								t.v1 = ((selstarty*cGraphics::brushsize+yy+1) * (1/(8.0f*cGraphics::brushsize)));

								t.u2 = (selstartx*cGraphics::brushsize+xx) * (1/(8.0f*cGraphics::brushsize));
								t.v2 = ((selstarty*cGraphics::brushsize+yy+1) * (1/(8.0f*cGraphics::brushsize)));
								
								t.u3 = (selstartx*cGraphics::brushsize+xx+1) * (1/(8.0f*cGraphics::brushsize));
								t.v3 = ((selstarty*cGraphics::brushsize+yy) * (1/(8.0f*cGraphics::brushsize)));
								
								t.u4 = (selstartx*cGraphics::brushsize+xx) * (1/(8.0f*cGraphics::brushsize));
								t.v4 = ((selstarty*cGraphics::brushsize+yy) * (1/(8.0f*cGraphics::brushsize)));
							}
							if (cGraphics::textureRot == 3)
							{
								t.v1 = (selstartx*cGraphics::brushsize+xx+1) * (1/(8.0f*cGraphics::brushsize));
								t.u1 = ((selendy*cGraphics::brushsize-yy-1) * (1/(8.0f*cGraphics::brushsize)));

								t.v2 = (selstartx*cGraphics::brushsize+xx) * (1/(8.0f*cGraphics::brushsize));
								t.u2 = ((selendy*cGraphics::brushsize-yy-1) * (1/(8.0f*cGraphics::brushsize)));
								
								t.v3 = (selstartx*cGraphics::brushsize+xx+1) * (1/(8.0f*cGraphics::brushsize));
								t.u3 = ((selendy*cGraphics::brushsize-yy) * (1/(8.0f*cGraphics::brushsize)));
								
								t.v4 = (selstartx*cGraphics::brushsize+xx) * (1/(8.0f*cGraphics::brushsize));
								t.u4 = ((selendy*cGraphics::brushsize-yy) * (1/(8.0f*cGraphics::brushsize)));
							}
							

							if(cGraphics::fliph)
							{
								t.u1 = ((selendx+selstartx)/8.0)-t.u1;
								t.u2 = ((selendx+selstartx)/8.0)-t.u2;
								t.u3 = ((selendx+selstartx)/8.0)-t.u3;
								t.u4 = ((selendx+selstartx)/8.0)-t.u4;
							}
							if(cGraphics::flipv)
							{
								t.v1 = ((selendy+selstarty)/8.0)-t.v1;
								t.v2 = ((selendy+selstarty)/8.0)-t.v2;
								t.v3 = ((selendy+selstarty)/8.0)-t.v3;
								t.v4 = ((selendy+selstarty)/8.0)-t.v4;
							}

//									cTile* tt = &cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp];
							if(y >= 0 && y < cGraphics::world->height && x >= 0 && x < cGraphics::world->width)
							{
								cGraphics::world->tiles.push_back(t);
								cGraphics::world->cubes[y][x].tileUp = cGraphics::world->tiles.size()-1;
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
				if (cGraphics::brushsize > 0.125)
					cGraphics::brushsize/=2.0f;
				break;
			case SDLK_EQUALS:
				cGraphics::brushsize*=2;
				break;
			case SDLK_SPACE:
				cGraphics::textureRot = (cGraphics::textureRot + 1) % 4;
				break;
			case SDLK_h:
				cGraphics::fliph = !cGraphics::fliph;
				break;
			case SDLK_v:
				cGraphics::flipv = !cGraphics::flipv;
				break;
			case SDLK_c:
				if(mouseX < cGraphics::w()-256)
				{
					float selsizex = (fabs(cGraphics::selectionstart.x - cGraphics::selectionend.x) / 32);
					float selsizey = (fabs(cGraphics::selectionstart.y - cGraphics::selectionend.y) / 32);

					selsizex = floor(selsizex*cGraphics::brushsize);
					selsizey = floor(selsizey*cGraphics::brushsize);

					if (cGraphics::textureRot % 2 == 1)
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
							if(x >= 0 && x < cGraphics::world->width && y >= 0 && y < cGraphics::world->height)
							{
								if(cGraphics::world->cubes[y][x].tileUp != -1)
								{
									row.push_back(cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp]);
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
				if(mouseX < cGraphics::w()-256)
				{
					float selsizex = (fabs(cGraphics::selectionstart.x - cGraphics::selectionend.x) / 32);
					float selsizey = (fabs(cGraphics::selectionstart.y - cGraphics::selectionend.y) / 32);

					selsizex = floor(selsizex*cGraphics::brushsize);
					selsizey = floor(selsizey*cGraphics::brushsize);

					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					if (cGraphics::textureRot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}
					if(clipboardgat.size() != selsizex || selsizex == 0)
						break;
					if(clipboardgat[0].size() != selsizey)
						break;

					
					cGraphics::worldContainer->undoStack->push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));

					for(int x = posx; x > posx-selsizex; x--)
					{
						for(int y = posy; y > posy-selsizey; y--)
						{
							if(x < 0 || y < 0)
								continue;
							int xx = posx - x;
							int yy = posy - y;
							if(clipboardgat[xx][yy] == 2)
							{
								cGraphics::world->cubes[y][x].tileUp = -1;
							}
							if(clipboardgat[xx][yy] == 1)
							{
								cGraphics::world->tiles.push_back(clipboardtexture[xx][yy]);
								cGraphics::world->cubes[y][x].tileUp = cGraphics::world->tiles.size()-1;
							}
						}
					}						
				}
				break;
			case SDLK_BACKSPACE:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					float selsizex = (fabs(cGraphics::selectionstart.x - cGraphics::selectionend.x) / 32);
					float selsizey = (fabs((int)cGraphics::selectionstart.y - cGraphics::selectionend.y) / 32);
					selsizex = (int)floor(selsizex*cGraphics::brushsize);
					selsizey = (int)floor(selsizey*cGraphics::brushsize);

					if (cGraphics::textureRot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}


					cGraphics::worldContainer->undoStack->push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));
					for(int x = posx; x > posx-selsizex; x--)
					{
						for(int y = posy; y > posy-selsizey; y--)
						{
							if (y < 0 || y >= cGraphics::world->height)
								continue;
							if (x < 0 || x >= cGraphics::world->width)
								continue;
							cGraphics::world->cubes[y][x].tileUp = -1;
						}
					}
					break;
				}
			case SDLK_i:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					if(posx > -1 && posy > -1 && posx < cGraphics::world->width && posy < cGraphics::world->height)
					{
						Log(3,0,"Cube (%i,%i): %f,%f,%f,%f", posx, posy, cGraphics::world->cubes[posy][posx].cell1, cGraphics::world->cubes[posy][posx].cell2, cGraphics::world->cubes[posy][posx].cell3, cGraphics::world->cubes[posy][posx].cell4);
						Log(3,0,"tileUp: %i", cGraphics::world->cubes[posy][posx].tileUp);
						if(cGraphics::world->cubes[posy][posx].tileUp != -1)
							Log(3,0,"Lightmap: %i", cGraphics::world->tiles[cGraphics::world->cubes[posy][posx].tileUp].lightmap);
					}

					break;
				}
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
