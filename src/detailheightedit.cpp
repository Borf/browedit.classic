#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern long tilex,tiley;
extern int brushsize;
extern bool lbuttondown, rbuttondown;
extern long lasttimer;
extern std::vector<std::vector<std::vector<float> > > clipboard;



int cProcessManagement::detailheightedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				int posx = tilex;
				int posy = tiley;
				bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
				float mmin = 99999999.0f;
				float mmax = -9999999.0f;
				if (ctrl)
				{
					if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
					{
						for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
						{
							for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
							{
								cCube* c = &Graphics.world.cubes[y][x];
								mmin = min(min(min(min(mmin, c->cell1),c->cell2),c->cell3),c->cell4);
								mmax = max(max(max(max(mmax, c->cell1),c->cell2),c->cell3),c->cell4);
							}
						}
					}
					
				}

				undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));

//				if (posx >= floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
				{
					for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
					{
						for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							if(x >= 0 && y >= 0 && x < Graphics.world.width && y < Graphics.world.height)
							{
								cCube* c = &Graphics.world.cubes[y][x];
								if(lbuttondown && !rbuttondown)
								{
									if (!Graphics.slope || ((x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f)))
										c->cell1-=1;
									if (!Graphics.slope || ((x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f)))
										c->cell2-=1;
									if (!Graphics.slope || ((x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1))
										c->cell3-=1;
									if (!Graphics.slope || ((x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1))
										c->cell4-=1;
									if(ctrl)
									{
										c->cell1 = max(mmin,c->cell1);
										c->cell2 = max(mmin,c->cell2);
										c->cell3 = max(mmin,c->cell3);
										c->cell4 = max(mmin,c->cell4);
									}
								}
								if(lbuttondown && rbuttondown)
								{
									if (!Graphics.slope || ((x > posx-(int)floor(brushsize/2.0f)) && y > posy-(int)floor(brushsize/2.0f)))
										c->cell1+=1;
									if (!Graphics.slope || ((x < posx+(int)ceil(brushsize/2.0f)-1) && y > posy-(int)floor(brushsize/2.0f)))
										c->cell2+=1;
									if (!Graphics.slope || ((x > posx-(int)floor(brushsize/2.0f)) && y < posy+(int)ceil(brushsize/2.0f)-1))
										c->cell3+=1;
									if (!Graphics.slope || ((x < posx+(int)ceil(brushsize/2.0f)-1) && y < posy+(int)ceil(brushsize/2.0f)-1))
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
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_MINUS:
				if (brushsize > 1)
					brushsize/=2;
				break;
			case SDLK_EQUALS:
				brushsize*=2;
				break;
			case SDLK_c:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					if (posx >= (int)floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= (int)floor(brushsize/2.0f) && posy<= Graphics.world.height-(int)ceil(brushsize/2.0f))
					{
						clipboard.clear();
						for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							std::vector<std::vector<float> > row;
							for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
							{
								std::vector<float> c;
								c.push_back(Graphics.world.cubes[y][x].cell1);
								c.push_back(Graphics.world.cubes[y][x].cell2);
								c.push_back(Graphics.world.cubes[y][x].cell3);
								c.push_back(Graphics.world.cubes[y][x].cell4);
								row.push_back(c);
							}
							clipboard.push_back(row);
						}
					}
				}
				break;
			case SDLK_p:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					if ((int)clipboard.size() != brushsize)
						break;

					undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
//					if (posx >= (int)floor(brushsize/2.0f) && posx <= Graphics.world.width-(int)ceil(brushsize/2.0f) && posy >= (int)floor(brushsize/2.0f) && posy <= Graphics.world.height-(int)ceil(brushsize/2.0f))
					{
						int yy = 0;
						for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							std::vector<std::vector<float> > row;
							int xx = 0;
							for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
							{
								if (x >= Graphics.world.width || x < 0 || y < 0 || y >= Graphics.world.height)
									continue;
								Graphics.world.cubes[y][x].cell1 = clipboard[yy][xx][0];
								Graphics.world.cubes[y][x].cell2 = clipboard[yy][xx][1];
								Graphics.world.cubes[y][x].cell3 = clipboard[yy][xx][2];
								Graphics.world.cubes[y][x].cell4 = clipboard[yy][xx][3];
								Graphics.world.cubes[y][x].calcNormal();
								xx++;
							}
							yy++;
						}
					}
				}
				break;
			case SDLK_f:
				{
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;

					undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
					for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
					{
						for(int y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width-(int)ceil(brushsize/2.0f) && y > 0 && y <= Graphics.world.height-(int)ceil(brushsize/2.0f))
							{
								float to = Graphics.world.cubes[y][x].cell2;
								Graphics.world.cubes[y][x].cell2 = to;
								Graphics.world.cubes[y][x+1].cell1 = to;
								Graphics.world.cubes[y-1][x+1].cell3 = to;
								Graphics.world.cubes[y-1][x].cell4 = to;

								Graphics.world.cubes[y][x].calcNormal();
								Graphics.world.cubes[y][x+1].calcNormal();
								Graphics.world.cubes[y-1][x+1].calcNormal();
								Graphics.world.cubes[y-1][x].calcNormal();
							}
						}
					}

				}
				break;
			case SDLK_s:
				{
					int x,y;
					int posx = (int)mouse3dx / 10;
					int posy = (int)mouse3dz / 10;
					undostack.push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
					for(x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
					{
						for(y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
							{
								float to = Graphics.world.cubes[y][x].cell2;
								Graphics.world.cubes[y][x].cell2 = to;
								if(x < Graphics.world.width-1)
									Graphics.world.cubes[y][x+1].cell1 = to;
								if (y > 0)	
								{
									if(x < Graphics.world.width-1)
										Graphics.world.cubes[y-1][x+1].cell3 = to;
									Graphics.world.cubes[y-1][x].cell4 = to;
								}
							}
						}
					}
					
					for(x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f)-1; x++)
					{
						for(y = posy-(int)floor(brushsize/2.0f)+1; y < posy+(int)ceil(brushsize/2.0f); y++)
						{
							if (x >= -1 && x < Graphics.world.width && y >= 0 && y <= Graphics.world.height)
							{
								float to = 0;
								int cnt = 0;
								if(x >= 0 && y < Graphics.world.height)
								{
									if(y+1 < Graphics.world.width)
									{
										if(x > 0)
										{
											to += Graphics.world.cubes[y+1][x-1].cell2;
											cnt++;
										}
										to += Graphics.world.cubes[y+1][x].cell2;
										cnt++;
										if(x+1 < Graphics.world.width)
										{
											to += Graphics.world.cubes[y+1][x+1].cell2;
											cnt++;
										}
									}
									if(x > 0)
									{
										to += Graphics.world.cubes[y][x-1].cell2;
										cnt++;
									}
									to += Graphics.world.cubes[y][x].cell2;
									cnt++;
									if(x+1 < Graphics.world.width)
									{
										to += Graphics.world.cubes[y][x+1].cell2;
										cnt++;
									}
									if(y > 0)
									{
										if(x > 0)
										{
											to += Graphics.world.cubes[y-1][x-1].cell2;
											cnt++;
										}
										to += Graphics.world.cubes[y-1][x].cell2;
										cnt++;
										if(x+1 < Graphics.world.width)
										{
											to += Graphics.world.cubes[y-1][x+1].cell2;
											cnt++;
										}
									}

									to = to / (float)cnt;
								}
								else if(x == -1 && y < Graphics.world.height)
									to = Graphics.world.cubes[y][x+1].cell2;
								else if(x != -1 && y == Graphics.world.height)
									to = Graphics.world.cubes[y-1][x].cell2;
								else
									to = Graphics.world.cubes[y-1][x+1].cell2;

								if(x >= 0 && y < Graphics.world.height)
									Graphics.world.cubes[y][x].cell2 = to;
								if(x < Graphics.world.width-1 && y < Graphics.world.height)
									Graphics.world.cubes[y][x+1].cell1 = to;
								if(y > 0)
								{
									if(x < Graphics.world.width-1)
										Graphics.world.cubes[y-1][x+1].cell3 = to;
									if(x >= 0)
										Graphics.world.cubes[y-1][x].cell4 = to;
								}


								if(x >= 0 && y < Graphics.world.height)
									Graphics.world.cubes[y][x].calcNormal();
								if(x < Graphics.world.width-1 && y < Graphics.world.height)
									Graphics.world.cubes[y][x+1].calcNormal();
								if(y > 0)
								{
									if(x < Graphics.world.width-1)
										Graphics.world.cubes[y-1][x+1].calcNormal();
									if(x >= 0)
										Graphics.world.cubes[y-1][x].calcNormal();
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
