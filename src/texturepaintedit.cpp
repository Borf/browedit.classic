#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern double mouse3dxstart, mouse3dystart, mouse3dzstart;
#include "windows/areacopywindow.h"


int cProcessManagement::texturepaintedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(mouseX < cGraphics::w()-256)
				{
					if(cGraphics::textureTool == TOOL_SELECTAREA)
					{
						if (mouse3dxstart > mouse3dx)
						{
							double d = mouse3dx;
							mouse3dx = mouse3dxstart;
							mouse3dxstart = d;
						}
						if (mouse3dzstart > mouse3dz)
						{
							double d = mouse3dz;
							mouse3dz = mouse3dzstart;
							mouse3dzstart = d;
						}
						bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
						bool alt = (SDL_GetModState() & KMOD_ALT) != 0;

						
						int x,y;
						if(!ctrl && !alt)
						{
							for(x = 0; x < cGraphics::world->width; x++)
							{
								for(y = 0; y < cGraphics::world->height; y++)
									cGraphics::world->cubes[y][x].selected = false;
							}
						}

						for(x = (int)round(mouse3dxstart/10); x < round(mouse3dx/10); x++)
						{
							for(y = (int)round(mouse3dzstart/10); y < (int)round(mouse3dz/10); y++)
							{
								if (x >= 0 && x < cGraphics::world->width && y >= 0 && y < cGraphics::world->height)
								{
									cGraphics::world->cubes[y][x].selected = !alt;
								}
							}
						}
					}
					else if (cGraphics::textureTool == TOOL_BRUSH)
					{
						int texture = cGraphics::texturestart;

						int x = (int)mouse3dx / 10;
						int y = (int)mouse3dz / 10;
						for(int yy = 0; yy < cGraphics::textureGridSizeY; yy++)
						{
							for(int xx = 0; xx < cGraphics::textureGridSizeX; xx++)
							{
								int xxx,yyy;
								int xi = 1;
								int yi = 1;
								
								if		(cGraphics::textureRot == 0)	{ yyy = yy;										xxx = xx;										}
								else if (cGraphics::textureRot == 1)	{ yyy = xx;										xxx = cGraphics::textureBrush[0].size()-1-yy;		}
								else if (cGraphics::textureRot == 2)	{ yyy = cGraphics::textureBrush.size()-1-yy;		xxx = cGraphics::textureBrush[0].size()-1-xx;		}
								else if	(cGraphics::textureRot == 3)	{ yyy = cGraphics::textureBrush.size()-1-xx;		xxx = yy;										}
								
								if( cGraphics::textureBrush.size() <= yyy ||
									cGraphics::textureBrush[0].size() <= xxx)
									continue;
								
								if(cGraphics::textureBrush[yyy][xxx])
								{
									if(y-yy >= 0 && y-yy < cGraphics::world->height && x+xx >= 0 && x+xx < cGraphics::world->width)
									{
										cCube* c = &cGraphics::world->cubes[y-yy][x+xx];
										cTile t;
										memset(t.color,255,3);
										t.lightmap = -1;
										t.texture = texture;
//todo: make this code nicer and less repetiditiditive
										if(cGraphics::textureRot == 0)
										{
											t.u1 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v1 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u3 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v3 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u2 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v2 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u4 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v4 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
										}
										else if (cGraphics::textureRot == 1)
										{
											t.u1 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v1 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u3 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v3 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u2 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v2 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u4 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v4 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
										}
										else if (cGraphics::textureRot == 2)
										{
											t.u1 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v1 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u3 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v3 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u2 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v2 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u4 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v4 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
										}
										else if (cGraphics::textureRot == 3)
										{
											t.u1 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v1 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u3 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v3 = (yyy+1+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u2 = (xxx+1+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v2 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
											t.u4 = (xxx+cGraphics::textureBrushOffset.x) * 1/ cGraphics::textureGridSizeX;
											t.v4 = (yyy+cGraphics::textureBrushOffset.y) / cGraphics::textureGridSizeY;
										}
										c->tileUp = cGraphics::world->tiles.size();
										cGraphics::world->tiles.push_back(t);
									}
								}
							}
						}	
					}
				}
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
			case SDLK_LEFTBRACKET:
				cGraphics::texturestart--;
				if (cGraphics::texturestart < 0)
					cGraphics::texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::texturestart++;
				if (cGraphics::texturestart >= cGraphics::world->textures.size())
					cGraphics::texturestart--;
				break;
			case SDLK_SPACE:
				if(event.key.keysym.mod & KMOD_CTRL)
				{
					std::vector<std::vector<bool> > newBrush(cGraphics::textureBrush[0].size(), std::vector<bool> (cGraphics::textureBrush.size()));
					for(int y = 0; y < cGraphics::textureBrush.size(); y++)
					{
						for(int x = 0; x < cGraphics::textureBrush[y].size(); x++)
						{
							newBrush[cGraphics::textureBrush[0].size()-1-x][y] = cGraphics::textureBrush[y][x];
						}						
					}
					cGraphics::textureBrush = newBrush;
				}
				else
				cGraphics::textureRot = (cGraphics::textureRot+1)%4;
				break;
			case SDLK_LEFT:
				cGraphics::textureBrushOffset.x--;
				if(cGraphics::textureBrushOffset.x < -cGraphics::textureGridSizeX)
					cGraphics::textureBrushOffset.x = -cGraphics::textureGridSizeX;
				break;
			case SDLK_RIGHT:
				cGraphics::textureBrushOffset.x++;
				if(cGraphics::textureBrushOffset.x > cGraphics::textureGridSizeX)
					cGraphics::textureBrushOffset.x = cGraphics::textureGridSizeX;
				break;
			case SDLK_UP:
				cGraphics::textureBrushOffset.y--;
				if(cGraphics::textureBrushOffset.y < -cGraphics::textureGridSizeY)
					cGraphics::textureBrushOffset.y = -cGraphics::textureGridSizeY;
				break;
			case SDLK_DOWN:
				cGraphics::textureBrushOffset.y++;
				if(cGraphics::textureBrushOffset.y > cGraphics::textureGridSizeY)
					cGraphics::textureBrushOffset.y = cGraphics::textureGridSizeY;
				break;
			case SDLK_h:
				cGraphics::fliph = !cGraphics::fliph;
				break;
			case SDLK_v:
				cGraphics::flipv = !cGraphics::flipv;
				break;
			case SDLK_MINUS:
				cGraphics::textureBrushSize = max(cGraphics::textureBrushSize-1,1);
				break;
			case SDLK_EQUALS:
				cGraphics::textureBrushSize = min(cGraphics::textureBrushSize+1, 16);
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
