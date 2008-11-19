#include <common.h>
#include "graphics.h"
#include "undo.h"
#include <clipboard.h>

extern long tilex,tiley;
extern long lasttimer;



int cProcessManagement::detailheightedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(cClipBoard::pasting && cClipBoard::currentClipBoard && cClipBoard::currentClipBoard->type == cClipBoard::CLIP_HEIGHT)
				{
					cClipBoard::currentClipBoard->apply();
					if((SDL_GetModState() & KMOD_CTRL) == 0)
						cClipBoard::pasting = false;
				}
				else
				{
					int posx = tilex;
					int posy = tiley;
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					float mmin = 99999999.0f;
					float mmax = -9999999.0f;
					if (ctrl)
					{
						if (posx >= floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posx <= cGraphics::world->width-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy >= floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy<= cGraphics::world->height-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
						{
							for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x++)
							{
								for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
								{
									cCube* c = &cGraphics::world->cubes[y][x];
									mmin = min(min(min(min(mmin, c->cell1),c->cell2),c->cell3),c->cell4);
									mmax = max(max(max(max(mmax, c->cell1),c->cell2),c->cell3),c->cell4);
								}
							}
						}
						
					}

					cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));

	//				if (posx >= floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posx <= cGraphics::world->width-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy >= floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy<= cGraphics::world->height-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
					{
						for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x++)
						{
							for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
							{
								if(x >= 0 && y >= 0 && x < cGraphics::world->width && y < cGraphics::world->height)
								{
									cCube* c = &cGraphics::world->cubes[y][x];
									if(cGraphics::cMouse::lbuttondown && !cGraphics::cMouse::rbuttondown)
									{
										if (!cGraphics::slope || ((x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)))
											c->cell1-=1;
										if (!cGraphics::slope || ((x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)))
											c->cell2-=1;
										if (!cGraphics::slope || ((x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1))
											c->cell3-=1;
										if (!cGraphics::slope || ((x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1))
											c->cell4-=1;
										if(ctrl)
										{
											c->cell1 = max(mmin,c->cell1);
											c->cell2 = max(mmin,c->cell2);
											c->cell3 = max(mmin,c->cell3);
											c->cell4 = max(mmin,c->cell4);
										}
									}
									if(cGraphics::cMouse::lbuttondown && cGraphics::cMouse::rbuttondown)
									{
										if (!cGraphics::slope || ((x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)))
											c->cell1+=1;
										if (!cGraphics::slope || ((x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y > posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)))
											c->cell2+=1;
										if (!cGraphics::slope || ((x > posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1))
											c->cell3+=1;
										if (!cGraphics::slope || ((x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1) && y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1))
											c->cell4+=1;
										if(ctrl)
										{
											c->cell1 = min(mmax,c->cell1);
											c->cell2 = min(mmax,c->cell2);
											c->cell3 = min(mmax,c->cell3);
											c->cell4 = min(mmax,c->cell4);
										}
									}
									c->calcNormal();
								}
							}
						}
						lasttimer = SDL_GetTicks()+500;
					}
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_MINUS:
				if (cGraphics::worldContainer->settings.brushSizeDetailHeight > 1)
					cGraphics::worldContainer->settings.brushSizeDetailHeight/=2;
				break;
			case SDLK_EQUALS:
				cGraphics::worldContainer->settings.brushSizeDetailHeight*=2;
				break;
			case SDLK_c:
				{
					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;

					if (posx >= (int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posx <= cGraphics::world->width-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy >= (int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && posy<= cGraphics::world->height-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
					{
						cClipboardHeight* clipboard = new cClipboardHeight();

						for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
						{
							std::vector<std::vector<float> > row;
							for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x++)
							{
								std::vector<float> c;
								c.push_back(cGraphics::world->cubes[y][x].cell1);
								c.push_back(cGraphics::world->cubes[y][x].cell2);
								c.push_back(cGraphics::world->cubes[y][x].cell3);
								c.push_back(cGraphics::world->cubes[y][x].cell4);
								row.push_back(c);
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
					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;

					cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)));
					for(int x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1; x++)
					{
						for(int y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)+1; y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
						{
							if (x >= 0 && x < cGraphics::world->width-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f) && y > 0 && y <= cGraphics::world->height-(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f))
							{
								float to = cGraphics::world->cubes[y][x].cell2;
								cGraphics::world->cubes[y][x].cell2 = to;
								cGraphics::world->cubes[y][x+1].cell1 = to;
								cGraphics::world->cubes[y-1][x+1].cell3 = to;
								cGraphics::world->cubes[y-1][x].cell4 = to;

								cGraphics::world->cubes[y][x].calcNormal();
								cGraphics::world->cubes[y][x+1].calcNormal();
								cGraphics::world->cubes[y-1][x+1].calcNormal();
								cGraphics::world->cubes[y-1][x].calcNormal();
							}
						}
					}

				}
				break;
			case SDLK_s:
				{
					int x,y;
					int posx = (int)cGraphics::cMouse::x3d / 10;
					int posy = (int)cGraphics::cMouse::z3d / 10;
					cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f), posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)));
					for(x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1; x++)
					{
						for(y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)+1; y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
						{
							if (x >= 0 && x < cGraphics::world->width && y >= 0 && y < cGraphics::world->height)
							{
								float to = cGraphics::world->cubes[y][x].cell2;
								cGraphics::world->cubes[y][x].cell2 = to;
								if(x < cGraphics::world->width-1)
									cGraphics::world->cubes[y][x+1].cell1 = to;
								if (y > 0)	
								{
									if(x < cGraphics::world->width-1)
										cGraphics::world->cubes[y-1][x+1].cell3 = to;
									cGraphics::world->cubes[y-1][x].cell4 = to;
								}
							}
						}
					}
					
					for(x = posx-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); x < posx+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)-1; x++)
					{
						for(y = posy-(int)floor(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f)+1; y < posy+(int)ceil(cGraphics::worldContainer->settings.brushSizeDetailHeight/2.0f); y++)
						{
							if (x >= -1 && x < cGraphics::world->width && y >= 0 && y <= cGraphics::world->height)
							{
								float to = 0;
								int cnt = 0;
								if(x >= 0 && y < cGraphics::world->height)
								{
									if(y+1 < cGraphics::world->width)
									{
										if(x > 0)
										{
											to += cGraphics::world->cubes[y+1][x-1].cell2;
											cnt++;
										}
										to += cGraphics::world->cubes[y+1][x].cell2;
										cnt++;
										if(x+1 < cGraphics::world->width)
										{
											to += cGraphics::world->cubes[y+1][x+1].cell2;
											cnt++;
										}
									}
									if(x > 0)
									{
										to += cGraphics::world->cubes[y][x-1].cell2;
										cnt++;
									}
									to += cGraphics::world->cubes[y][x].cell2;
									cnt++;
									if(x+1 < cGraphics::world->width)
									{
										to += cGraphics::world->cubes[y][x+1].cell2;
										cnt++;
									}
									if(y > 0)
									{
										if(x > 0)
										{
											to += cGraphics::world->cubes[y-1][x-1].cell2;
											cnt++;
										}
										to += cGraphics::world->cubes[y-1][x].cell2;
										cnt++;
										if(x+1 < cGraphics::world->width)
										{
											to += cGraphics::world->cubes[y-1][x+1].cell2;
											cnt++;
										}
									}

									to = to / (float)cnt;
								}
								else if(x == -1 && y < cGraphics::world->height)
									to = cGraphics::world->cubes[y][x+1].cell2;
								else if(x != -1 && y == cGraphics::world->height)
									to = cGraphics::world->cubes[y-1][x].cell2;
								else
									to = cGraphics::world->cubes[y-1][x+1].cell2;

								if(x >= 0 && y < cGraphics::world->height)
									cGraphics::world->cubes[y][x].cell2 = to;
								if(x < cGraphics::world->width-1 && y < cGraphics::world->height)
									cGraphics::world->cubes[y][x+1].cell1 = to;
								if(y > 0)
								{
									if(x < cGraphics::world->width-1)
										cGraphics::world->cubes[y-1][x+1].cell3 = to;
									if(x >= 0)
										cGraphics::world->cubes[y-1][x].cell4 = to;
								}


								if(x >= 0 && y < cGraphics::world->height)
									cGraphics::world->cubes[y][x].calcNormal();
								if(x < cGraphics::world->width-1 && y < cGraphics::world->height)
									cGraphics::world->cubes[y][x+1].calcNormal();
								if(y > 0)
								{
									if(x < cGraphics::world->width-1)
										cGraphics::world->cubes[y-1][x+1].calcNormal();
									if(x >= 0)
										cGraphics::world->cubes[y-1][x].calcNormal();
								}
							}
						}
					}
					

				}
				break;
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
