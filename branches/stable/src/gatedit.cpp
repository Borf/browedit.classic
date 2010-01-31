#include <common.h>
#include <wm/window.h>
#include "graphics.h"
#include "undo/gattileedit.h"
#include "undo/gatheightedit.h"
#include "clipboard.h"
#include <wm/wm.h>


int cProcessManagement::gatedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(cGraphics::cMouse::lbuttondown && !cGraphics::cMouse::rbuttondown)
			{
				int posx = (int)cGraphics::cMouse::x3d / 5;
				int posy = (int)cGraphics::cMouse::z3d / 5;

				int f = (int)ceil(cGraphics::worldContainer->settings.brushsize);

			//	if (posx >= floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*cGraphics::world->height-(int)ceil(f/2.0f))
				{
					cGraphics::worldContainer->undoStack->push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

					glColor4f(1,0,0,1);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if (y < 0 || y >= cGraphics::world->height*2 || x < 0 || x >= cGraphics::world->width*2)
								continue;
							cGatTile* c = &cGraphics::world->gattiles[y][x];
							c->type = cGraphics::gatTiles[cGraphics::worldContainer->settings.texturestart];
						}
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(cClipBoard::pasting && cClipBoard::currentClipBoard && cClipBoard::currentClipBoard->type == cClipBoard::CLIP_GAT)
				{
					cClipBoard::currentClipBoard->apply();
					if((SDL_GetModState() & KMOD_CTRL) == 0)
						cClipBoard::pasting = false;
				}
				else
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					int f = (int)ceil(cGraphics::worldContainer->settings.brushsize);

				//	if (posx >= floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*cGraphics::world->height-(int)ceil(f/2.0f))
					{
						cGraphics::worldContainer->undoStack->push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

						glColor4f(1,0,0,1);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								if (y < 0 || y >= cGraphics::world->height*2 || x < 0 || x >= cGraphics::world->width*2)
									continue;
								cGatTile* c = &cGraphics::world->gattiles[y][x];
								c->type = cGraphics::gatTiles[cGraphics::worldContainer->settings.texturestart];
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
			case SDLK_EQUALS:
				cGraphics::worldContainer->settings.brushsize*=2;
				break;
			case SDLK_MINUS:
				if (cGraphics::worldContainer->settings.brushsize > 1)
					cGraphics::worldContainer->settings.brushsize/=2;
				break;
			case SDLK_SPACE:
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					float f = (int)ceil(cGraphics::worldContainer->settings.brushsize);


					if (posx >= (int)floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*cGraphics::world->height-(int)ceil(f/2.0f))
					{
						cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cCube* c = &cGraphics::world->cubes[y/2][x/2];

								if (y%2 == 0 && x%2 == 0)
								{
									cGraphics::world->gattiles[y][x].cell1 = (c->cell1+c->cell1) / 2.0f;
									cGraphics::world->gattiles[y][x].cell2 = (c->cell1+c->cell2) / 2.0f;
									cGraphics::world->gattiles[y][x].cell3 = (c->cell1+c->cell3) / 2.0f;
									cGraphics::world->gattiles[y][x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
								}
								if (y%2 == 0 && x%2 == 1)
								{
									cGraphics::world->gattiles[y][x].cell1 = (c->cell1+c->cell2) / 2.0f;
									cGraphics::world->gattiles[y][x].cell2 = (c->cell2+c->cell2) / 2.0f;
									cGraphics::world->gattiles[y][x].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									cGraphics::world->gattiles[y][x].cell4 = (c->cell4+c->cell2) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 1)
								{
									cGraphics::world->gattiles[y][x].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									cGraphics::world->gattiles[y][x].cell2 = (c->cell4 + c->cell2) / 2.0f;
									cGraphics::world->gattiles[y][x].cell3 = (c->cell4 + c->cell3) / 2.0f;
									cGraphics::world->gattiles[y][x].cell4 = (c->cell4 + c->cell4) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 0)
								{
									cGraphics::world->gattiles[y][x].cell1 = (c->cell3 + c->cell1) / 2.0f;
									cGraphics::world->gattiles[y][x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									cGraphics::world->gattiles[y][x].cell3 = (c->cell3 + c->cell3) / 2.0f;
									cGraphics::world->gattiles[y][x].cell4 = (c->cell3 + c->cell4) / 2.0f;
								}
							
							}
						}
					}
				}
				break;
			case SDLK_PAGEDOWN:
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					float f = ceil(cGraphics::worldContainer->settings.brushsize);


					if (posx >= (int)floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*cGraphics::world->height-(int)ceil(f/2.0f))
					{
						cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cGatTile* c = &cGraphics::world->gattiles[y][x];
								if (!cGraphics::slope || ((x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f)))
									c->cell1+=1;
								if (!cGraphics::slope || ((x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f)))
									c->cell2+=1;
								if (!cGraphics::slope || ((x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1))
									c->cell3+=1;
								if (!cGraphics::slope || ((x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1))
									c->cell4+=1;
							}
						}
					}
				}
				break;
			case SDLK_PAGEUP:
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					float f = (int)ceil(cGraphics::worldContainer->settings.brushsize);

					cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
					glColor4f(1,0,0,1);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if(x >= 0 && y >= 0 && y < (int)cGraphics::world->gattiles.size() && x < (int)cGraphics::world->gattiles[0].size())
							{
								cGatTile* c = &cGraphics::world->gattiles[y][x];
								if (!cGraphics::slope || ((x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f)))
									c->cell1-=1;
								if (!cGraphics::slope || ((x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f)))
									c->cell2-=1;
								if (!cGraphics::slope || ((x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1))
									c->cell3-=1;
								if (!cGraphics::slope || ((x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1))
									c->cell4-=1;
							}
						}
					}
				}
				break;
			case SDLK_c:
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					int f = (int)ceil(cGraphics::worldContainer->settings.brushsize);

					if (posx >= (int)floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= f && posy< 2*cGraphics::world->height-f)
					{
						cClipboardGat* clipboard = new cClipboardGat();
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							std::vector<std::pair<int, std::vector<float> > > row;
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								std::vector<float> c;
								c.push_back(cGraphics::world->gattiles[y][x].cell1);
								c.push_back(cGraphics::world->gattiles[y][x].cell2);
								c.push_back(cGraphics::world->gattiles[y][x].cell3);
								c.push_back(cGraphics::world->gattiles[y][x].cell4);
								row.push_back(std::pair<int, std::vector<float> >(cGraphics::world->gattiles[y][x].type, c));
							}
							clipboard->data.push_back(row);
						}
						cClipBoard::setClipBoard(clipboard);
						cClipBoard::pasting = true;
					}
				}
				break;
			case SDLK_p:
				{
					cClipBoard::pasting = !cClipBoard::pasting;
				}					
				break;
			case SDLK_f:
				{
					int posx = (int)cGraphics::cMouse::x3d / 5;
					int posy = (int)cGraphics::cMouse::z3d / 5;

					float f = (int)ceil(cGraphics::worldContainer->settings.brushsize);


					cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f)-1; x++)
					{
						for(int y = posy-(int)floor(f/2.0f)+1; y < posy+(int)ceil(f/2.0f); y++)
						{
							if (x >= 0 && x < cGraphics::world->width*2 && y > 0 && y <= cGraphics::world->height*2)
							{
								float to = cGraphics::world->gattiles[y][x].cell2;
								cGraphics::world->gattiles[y][x].cell2 = to;
								cGraphics::world->gattiles[y][x+1].cell1 = to;
								cGraphics::world->gattiles[y-1][x+1].cell3 = to;
								cGraphics::world->gattiles[y-1][x].cell4 = to;
							}
						}
					}

				}
				break;
			case SDLK_LEFTBRACKET:
				cGraphics::worldContainer->settings.texturestart--;
				if (cGraphics::worldContainer->settings.texturestart < 0)
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::worldContainer->settings.texturestart++;
				if (cGraphics::worldContainer->settings.texturestart > 8)
					cGraphics::worldContainer->settings.texturestart--;
				break;
			case SDLK_i:
				{
				int posx = (int)cGraphics::cMouse::x3d / 5;
				int posy = (int)cGraphics::cMouse::z3d / 5;
				char buf[100];
				sprintf(buf, "Your cursor is at %i,%i", posx, posy);
				if(cWM::getWindow(WT_MESSAGE))
					cWM::getWindow(WT_MESSAGE)->close();
				cWM::showMessage(buf);
				break;
				}



			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
