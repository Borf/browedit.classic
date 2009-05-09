#include <common.h>
#include "graphics.h"
#include "undo/texture.h"
#include "clipboard.h"


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
				if(cGraphics::cMouse::xStart < cGraphics::w()-256)
				{
					if(cClipBoard::pasting && cClipBoard::currentClipBoard && cClipBoard::currentClipBoard->type == cClipBoard::CLIP_TEXTURE)
					{
						cClipBoard::currentClipBoard->apply();
						if((SDL_GetModState() & KMOD_CTRL) == 0)
							cClipBoard::pasting = false;
					}
					else
					{
						float selsizex = (fabs(cGraphics::worldContainer->settings.selectionstart.x - cGraphics::worldContainer->settings.selectionend.x) / 32);
						float selsizey = (fabs(cGraphics::worldContainer->settings.selectionstart.y - cGraphics::worldContainer->settings.selectionend.y) / 32);

						selsizex = floor(selsizex*cGraphics::worldContainer->settings.brushsize);
						selsizey = floor(selsizey*cGraphics::worldContainer->settings.brushsize);

						int posx = (int)cGraphics::cMouse::x3d / 10;
						int posy = (int)cGraphics::cMouse::z3d / 10;

						float selstartx = ((cGraphics::worldContainer->settings.selectionstart.x - (cGraphics::w()-256)) / 32.0f);
						float selstarty = ((int)(cGraphics::worldContainer->settings.selectionstart.y - 32) % 288) / 32;
						float selendx = ((cGraphics::worldContainer->settings.selectionend.x - (cGraphics::w()-256)) / 32.0f);
						float selendy = ((int)(cGraphics::worldContainer->settings.selectionend.y - 32) % 288) / 32;

						selstartx += (cGraphics::w()%32)/32.0f;
						selendx += (cGraphics::w()%32)/32.0f;

						glColor4f(1,1,1,0.7f);
						glEnable(GL_BLEND);

						if (cGraphics::worldContainer->settings.textureRot % 2 == 1)
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
								if (y < 0 || y >= cGraphics::world->height)
									continue;
								if (x < 0 || x >= cGraphics::world->width)
									continue;
								cTile t;
								t.color[0] = (char)255;
								t.color[1] = (char)255;
								t.color[2] = (char)255;
								t.color[3] = (char)255;
								t.texture = cGraphics::worldContainer->settings.texturestart + (int)(cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;
								if(cGraphics::clearLightmaps)
									t.lightmap = 0;
								else
								{
									if(cGraphics::world->cubes[y][x].tileUp != -1)
										t.lightmap = cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp].lightmap;
									else
										t.lightmap = 0;
								}
								if (cGraphics::worldContainer->settings.textureRot == 0)
								{
									t.u1 = (selendx*cGraphics::worldContainer->settings.brushsize-xx-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v1 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));

									t.u2 = (selendx*cGraphics::worldContainer->settings.brushsize-xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v2 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.u3 = (selendx*cGraphics::worldContainer->settings.brushsize-xx-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v3 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.u4 = (selendx*cGraphics::worldContainer->settings.brushsize-xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v4 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
								}
								if (cGraphics::worldContainer->settings.textureRot == 1)
								{
									t.v1 = (selendx*cGraphics::worldContainer->settings.brushsize-xx-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u1 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));

									t.v2 = (selendx*cGraphics::worldContainer->settings.brushsize-xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u2 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.v3 = (selendx*cGraphics::worldContainer->settings.brushsize-xx-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u3 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.v4 = (selendx*cGraphics::worldContainer->settings.brushsize-xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u4 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
								}
								if(cGraphics::worldContainer->settings.textureRot == 2)
								{
									t.u1 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v1 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));

									t.u2 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v2 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.u3 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v3 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.u4 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.v4 = ((selstarty*cGraphics::worldContainer->settings.brushsize+yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
								}
								if (cGraphics::worldContainer->settings.textureRot == 3)
								{
									t.v1 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u1 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));

									t.v2 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u2 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy-1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.v3 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx+1) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u3 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
									
									t.v4 = (selstartx*cGraphics::worldContainer->settings.brushsize+xx) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize));
									t.u4 = ((selendy*cGraphics::worldContainer->settings.brushsize-yy) * (1/(8.0f*cGraphics::worldContainer->settings.brushsize)));
								}
								

								if(cGraphics::worldContainer->settings.fliph)
								{
									t.u1 = ((selendx+selstartx)/8.0)-t.u1;
									t.u2 = ((selendx+selstartx)/8.0)-t.u2;
									t.u3 = ((selendx+selstartx)/8.0)-t.u3;
									t.u4 = ((selendx+selstartx)/8.0)-t.u4;
								}
								if(cGraphics::worldContainer->settings.flipv)
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
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_MINUS:
				if (cGraphics::worldContainer->settings.brushsize > 0.125)
					cGraphics::worldContainer->settings.brushsize/=2.0f;
				break;
			case SDLK_EQUALS:
				cGraphics::worldContainer->settings.brushsize*=2;
				break;
			case SDLK_SPACE:
				cGraphics::worldContainer->settings.textureRot = (cGraphics::worldContainer->settings.textureRot + 1) % 4;
				break;
			case SDLK_h:
				cGraphics::worldContainer->settings.fliph = !cGraphics::worldContainer->settings.fliph;
				break;
			case SDLK_v:
				cGraphics::worldContainer->settings.flipv = !cGraphics::worldContainer->settings.flipv;
				break;
			case SDLK_c:
				if(cGraphics::cMouse::x < cGraphics::w()-256)
				{
					float selsizex = (fabs(cGraphics::worldContainer->settings.selectionstart.x - cGraphics::worldContainer->settings.selectionend.x) / 32);
					float selsizey = (fabs(cGraphics::worldContainer->settings.selectionstart.y - cGraphics::worldContainer->settings.selectionend.y) / 32);

					selsizex = floor(selsizex*cGraphics::worldContainer->settings.brushsize);
					selsizey = floor(selsizey*cGraphics::worldContainer->settings.brushsize);

					if (cGraphics::worldContainer->settings.textureRot % 2 == 1)
					{
						float a = selsizex;
						selsizex = selsizey;
						selsizey = a;
					}

					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;
					cClipboardTexture* clipboard = new cClipboardTexture();

					for(int x = posx; x > posx-selsizex; x--)
					{
						std::vector<std::pair<int, cTile> > row;
						for(int y = posy; y > posy-selsizey; y--)
						{
							if(x >= 0 && x < cGraphics::world->width && y >= 0 && y < cGraphics::world->height)
							{
								if(cGraphics::world->cubes[y][x].tileUp != -1)
								{
									row.push_back(std::pair<int, cTile>(1, cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp]));
								}
								else
								{
									row.push_back(std::pair<int ,cTile>(2, cTile()));
								}
							}
							else
							{
								row.push_back(std::pair<int ,cTile>(0, cTile()));
							}

						}
						clipboard->data.push_back(row);
					}
					cClipBoard::setClipBoard(clipboard);
					cClipBoard::pasting = true;
				}
				break; 
			case SDLK_p:
				if(cClipBoard::currentClipBoard && cClipBoard::currentClipBoard->type == cClipBoard::CLIP_TEXTURE)
					cClipBoard::pasting = !cClipBoard::pasting;
				break;
			case SDLK_BACKSPACE:
				{
					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;
					float selsizex = (fabs(cGraphics::worldContainer->settings.selectionstart.x - cGraphics::worldContainer->settings.selectionend.x) / 32);
					float selsizey = (fabs((int)cGraphics::worldContainer->settings.selectionstart.y - cGraphics::worldContainer->settings.selectionend.y) / 32);
					selsizex = (int)floor(selsizex*cGraphics::worldContainer->settings.brushsize);
					selsizey = (int)floor(selsizey*cGraphics::worldContainer->settings.brushsize);

					if (cGraphics::worldContainer->settings.textureRot % 2 == 1)
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
					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;
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
				cGraphics::worldContainer->settings.texturestart--;
				if (cGraphics::worldContainer->settings.texturestart < 0)
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::worldContainer->settings.texturestart++;
				if (cGraphics::worldContainer->settings.texturestart > ((int)cGraphics::world->textures.size()) - (cGraphics::h() / 288))
					cGraphics::worldContainer->settings.texturestart--;
				break;
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
