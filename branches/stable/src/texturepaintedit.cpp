#include <common.h>
#include "graphics.h"
#include "undo.h"
#include <menucommands.h>

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
				if(cGraphics::cMouse::x < cGraphics::w()-256)
				{
					if(cGraphics::textureTool == TOOL_SELECTAREA)
					{
						if (cGraphics::cMouse::x3dStart > cGraphics::cMouse::x3d)
						{
							double d = cGraphics::cMouse::x3d;
							cGraphics::cMouse::x3d = cGraphics::cMouse::x3dStart;
							cGraphics::cMouse::x3dStart = d;
						}
						if (cGraphics::cMouse::z3dStart > cGraphics::cMouse::z3d)
						{
							double d = cGraphics::cMouse::z3d;
							cGraphics::cMouse::z3d = cGraphics::cMouse::z3dStart;
							cGraphics::cMouse::z3dStart = d;
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

						for(x = (int)round(cGraphics::cMouse::x3dStart/10); x < round(cGraphics::cMouse::x3d/10); x++)
						{
							for(y = (int)round(cGraphics::cMouse::z3dStart/10); y < (int)round(cGraphics::cMouse::z3d/10); y++)
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
						int texture = cGraphics::worldContainer->settings.texturestart;

						int x = (int)cGraphics::cMouse::x3d / 10;
						int y = (int)cGraphics::cMouse::z3d / 10;
						for(int yy = 0; yy < cGraphics::textureGridSizeY; yy++)
						{
							for(int xx = 0; xx < cGraphics::textureGridSizeX; xx++)
							{
								int xxx=0,yyy=0;
								
								if		(cGraphics::worldContainer->settings.textureRot == 0)	{ yyy = yy;										xxx = xx;										}
								else if (cGraphics::worldContainer->settings.textureRot == 1)	{ yyy = xx;										xxx = cGraphics::textureBrush[0].size()-1-yy;		}
								else if (cGraphics::worldContainer->settings.textureRot == 2)	{ yyy = cGraphics::textureBrush.size()-1-yy;		xxx = cGraphics::textureBrush[0].size()-1-xx;		}
								else if	(cGraphics::worldContainer->settings.textureRot == 3)	{ yyy = cGraphics::textureBrush.size()-1-xx;		xxx = yy;										}
								
								if( (int)cGraphics::textureBrush.size() <= yyy ||
									(int)cGraphics::textureBrush[0].size() <= xxx)
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
										if(cGraphics::worldContainer->settings.textureRot == 0)
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
										else if (cGraphics::worldContainer->settings.textureRot == 1)
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
										else if (cGraphics::worldContainer->settings.textureRot == 2)
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
										else if (cGraphics::worldContainer->settings.textureRot == 3)
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
				cGraphics::worldContainer->settings.texturestart--;
				if (cGraphics::worldContainer->settings.texturestart < 0)
					cGraphics::worldContainer->settings.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				cGraphics::worldContainer->settings.texturestart++;
				if (cGraphics::worldContainer->settings.texturestart >= (int)cGraphics::world->textures.size())
					cGraphics::worldContainer->settings.texturestart--;
				break;
			case SDLK_SPACE:
				if(event.key.keysym.mod & KMOD_CTRL)
				{
					std::vector<std::vector<bool> > newBrush(cGraphics::textureBrush[0].size(), std::vector<bool> (cGraphics::textureBrush.size()));
					for(unsigned int y = 0; y < cGraphics::textureBrush.size(); y++)
					{
						for(unsigned int x = 0; x < cGraphics::textureBrush[y].size(); x++)
						{
							newBrush[cGraphics::textureBrush[0].size()-1-x][y] = cGraphics::textureBrush[y][x];
						}						
					}
					cGraphics::textureBrush = newBrush;
				}
				else
				cGraphics::worldContainer->settings.textureRot = (cGraphics::worldContainer->settings.textureRot+1)%4;
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
				cGraphics::worldContainer->settings.fliph = !cGraphics::worldContainer->settings.fliph;
				break;
			case SDLK_v:
				cGraphics::worldContainer->settings.flipv = !cGraphics::worldContainer->settings.flipv;
				break;
			case SDLK_MINUS:
				cGraphics::textureBrushSize = max(cGraphics::textureBrushSize-1,1);
				break;
			case SDLK_EQUALS:
				cGraphics::textureBrushSize = min(cGraphics::textureBrushSize+1, 16);
				break;
			case SDLK_a:
				if(event.key.keysym.mod & KMOD_CTRL)
					MenuCommand_selectAll(NULL);
				break;
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
