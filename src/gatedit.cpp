#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown, rbuttondown;
extern vector<vector<vector<float> > > clipboard;
vector<vector<int > > clipboardgat;


int cProcessManagement::gatedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown && !rbuttondown)
			{
				int posx = (int)mouse3dx / 5;
				int posy = (int)mouse3dz / 5;

				int f = (int)ceil(Graphics.brushsize);

			//	if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
				{
					undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

					glColor4f(1,0,0,1);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if (y < 0 || y >= Graphics.world.height*2 || x < 0 || x >= Graphics.world.width*2)
								continue;
							cGatTile* c = &Graphics.world.gattiles[y][x];
							c->type = Graphics.texturestart;
						}
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				int posx = (int)mouse3dx / 5;
				int posy = (int)mouse3dz / 5;

				int f = (int)ceil(Graphics.brushsize);

			//	if (posx >= floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
				{
					undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

					glColor4f(1,0,0,1);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if (y < 0 || y >= Graphics.world.height*2 || x < 0 || x >= Graphics.world.width*2)
								continue;
							cGatTile* c = &Graphics.world.gattiles[y][x];
							c->type = Graphics.texturestart;
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
				Graphics.brushsize*=2;
				break;
			case SDLK_MINUS:
				if (Graphics.brushsize > 1)
					Graphics.brushsize/=2;
				break;
			case SDLK_SPACE:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);


					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
					{
						undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cCube* c = &Graphics.world.cubes[y/2][x/2];

								if (y%2 == 0 && x%2 == 0)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell1) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell1+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell1+c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
								}
								if (y%2 == 0 && x%2 == 1)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell2+c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell4+c->cell2) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 1)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell4 + c->cell2) / 2.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell4 + c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell4 + c->cell4) / 2.0f;
								}
								if (y%2 == 1 && x%2 == 0)
								{
									Graphics.world.gattiles[y][x].cell1 = (c->cell3 + c->cell1) / 2.0f;
									Graphics.world.gattiles[y][x].cell2 = (c->cell1+c->cell4+c->cell2+c->cell3) / 4.0f;
									Graphics.world.gattiles[y][x].cell3 = (c->cell3 + c->cell3) / 2.0f;
									Graphics.world.gattiles[y][x].cell4 = (c->cell3 + c->cell4) / 2.0f;
								}
							
							}
						}
					}
				}
				break;
			case SDLK_PAGEDOWN:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);


					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
					{
						undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cGatTile* c = &Graphics.world.gattiles[y][x];
								if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f))
									c->cell1+=1;
								if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f))
									c->cell2+=1;
								if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1)
									c->cell3+=1;
								if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1)
									c->cell4+=1;
							}
						}
					}
				}
				break;
			case SDLK_PAGEUP:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);

					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= (int)floor(f/2.0f) && posy< 2*Graphics.world.height-(int)ceil(f/2.0f))
					{
						undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
						glColor4f(1,0,0,1);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
						for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
						{
							for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
							{
								cGatTile* c = &Graphics.world.gattiles[y][x];
								if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y > posy-(int)floor(f/2.0f))
									c->cell1-=1;
								if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y > posy-(int)floor(f/2.0f))
									c->cell2-=1;
								if (!Graphics.slope || (x > posx-(int)floor(f/2.0f)) && y < posy+(int)ceil(f/2.0f)-1)
									c->cell3-=1;
								if (!Graphics.slope || (x < posx+(int)ceil(f/2.0f)-1) && y < posy+(int)ceil(f/2.0f)-1)
									c->cell4-=1;
							}
						}
					}
				}
				break;
			case SDLK_c:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					int f = (int)ceil(Graphics.brushsize);

					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
					{
						clipboard.clear();
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							vector<vector<float> > row;
							vector<int > row2;
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								vector<float> c;
								c.push_back(Graphics.world.gattiles[y][x].cell1);
								c.push_back(Graphics.world.gattiles[y][x].cell2);
								c.push_back(Graphics.world.gattiles[y][x].cell3);
								c.push_back(Graphics.world.gattiles[y][x].cell4);
								row.push_back(c);
								row2.push_back(Graphics.world.gattiles[y][x].type);
							}
							clipboard.push_back(row);
							clipboardgat.push_back(row2);
						}
					}
				}
				break;
			case SDLK_p:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					int f = (int)ceil(Graphics.brushsize);

					if ((int)clipboard.size() != f)
						break;

					undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
					undostack.push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));

					if (posx >= (int)floor(f/2.0f) && posx < 2*Graphics.world.width-(int)ceil(f/2.0f) && posy >= f && posy< 2*Graphics.world.height-f)
					{
						int yy = 0;
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							int xx = 0;
							for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
							{
								Graphics.world.gattiles[y][x].cell1 = clipboard[yy][xx][0];
								Graphics.world.gattiles[y][x].cell2 = clipboard[yy][xx][1];
								Graphics.world.gattiles[y][x].cell3 = clipboard[yy][xx][2];
								Graphics.world.gattiles[y][x].cell4 = clipboard[yy][xx][3];
								Graphics.world.gattiles[y][x].type = clipboardgat[yy][xx];
								xx++;
							}
							yy++;
						}
					}
				}
				break;
			case SDLK_f:
				{
					int posx = (int)mouse3dx / 5;
					int posy = (int)mouse3dz / 5;

					float f = (int)ceil(Graphics.brushsize);


					undostack.push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
					for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
					{
						for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
						{
							if (x >= 0 && x < Graphics.world.width*2 && y > 0 && y <= Graphics.world.height*2)
							{
								float to = Graphics.world.gattiles[y][x].cell2;
								Graphics.world.gattiles[y][x].cell2 = to;
								Graphics.world.gattiles[y][x+1].cell1 = to;
								Graphics.world.gattiles[y-1][x+1].cell3 = to;
								Graphics.world.gattiles[y-1][x].cell4 = to;
							}
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
				if (Graphics.texturestart > 8)
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
