#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
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
				if(mouseX < Graphics.w()-256)
				{
					if(Graphics.textureTool == TOOL_SELECTAREA)
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
							for(x = 0; x < Graphics.world.width; x++)
							{
								for(y = 0; y < Graphics.world.height; y++)
									Graphics.world.cubes[y][x].selected = false;
							}
						}

						for(x = (int)round(mouse3dxstart/10); x < round(mouse3dx/10); x++)
						{
							for(y = (int)round(mouse3dzstart/10); y < (int)round(mouse3dz/10); y++)
							{
								if (x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
								{
									Graphics.world.cubes[y][x].selected = !alt;
								}
							}
						}
					}
					else if (Graphics.textureTool == TOOL_BRUSH)
					{
						int texture = Graphics.texturestart;

						int x = (int)mouse3dx / 10;
						int y = (int)mouse3dz / 10;
						for(int yy = 0; yy < Graphics.textureGridSizeY; yy++)
						{
							for(int xx = 0; xx < Graphics.textureGridSizeX; xx++)
							{
								int xxx,yyy;
								int xi = 1;
								int yi = 1;
								
								if		(Graphics.textureRot == 0)	{ yyy = yy;										xxx = xx;										}
								else if (Graphics.textureRot == 1)	{ yyy = xx;										xxx = Graphics.textureBrush[0].size()-1-yy;		}
								else if (Graphics.textureRot == 2)	{ yyy = Graphics.textureBrush.size()-1-yy;		xxx = Graphics.textureBrush[0].size()-1-xx;		}
								else if	(Graphics.textureRot == 3)	{ yyy = Graphics.textureBrush.size()-1-xx;		xxx = yy;										}
								
								if( Graphics.textureBrush.size() <= yyy ||
									Graphics.textureBrush[0].size() <= xxx)
									continue;
								
								if(Graphics.textureBrush[yyy][xxx])
								{
									if(y-yy >= 0 && y-yy < Graphics.world.height && x+xx >= 0 && x+xx < Graphics.world.width)
									{
										cCube* c = &Graphics.world.cubes[y-yy][x+xx];
										cTile t;
										memset(t.color,255,3);
										t.lightmap = -1;
										t.texture = texture;
//todo: make this code nicer and less repetiditiditive
										if(Graphics.textureRot == 0)
										{
											t.u1 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v1 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u3 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v3 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u2 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v2 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u4 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v4 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
										}
										else if (Graphics.textureRot == 1)
										{
											t.u1 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v1 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u3 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v3 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u2 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v2 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u4 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v4 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
										}
										else if (Graphics.textureRot == 2)
										{
											t.u1 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v1 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u3 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v3 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u2 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v2 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u4 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v4 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
										}
										else if (Graphics.textureRot == 3)
										{
											t.u1 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v1 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u3 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v3 = (yyy+1+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u2 = (xxx+1+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v2 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
											t.u4 = (xxx+Graphics.textureBrushOffset.x) * 1/ Graphics.textureGridSizeX;
											t.v4 = (yyy+Graphics.textureBrushOffset.y) / Graphics.textureGridSizeY;
										}
										c->tileUp = Graphics.world.tiles.size();
										Graphics.world.tiles.push_back(t);
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
				Graphics.texturestart--;
				if (Graphics.texturestart < 0)
					Graphics.texturestart = 0;
				break;
			case SDLK_RIGHTBRACKET:
				Graphics.texturestart++;
				if (Graphics.texturestart >= Graphics.world.textures.size())
					Graphics.texturestart--;
				break;
			case SDLK_SPACE:
				if(event.key.keysym.mod & KMOD_CTRL)
				{
					std::vector<std::vector<bool> > newBrush(Graphics.textureBrush[0].size(), std::vector<bool> (Graphics.textureBrush.size()));
					for(int y = 0; y < Graphics.textureBrush.size(); y++)
					{
						for(int x = 0; x < Graphics.textureBrush[y].size(); x++)
						{
							newBrush[Graphics.textureBrush[0].size()-1-x][y] = Graphics.textureBrush[y][x];
						}						
					}
					Graphics.textureBrush = newBrush;
				}
				else
				Graphics.textureRot = (Graphics.textureRot+1)%4;
				break;
			case SDLK_LEFT:
				Graphics.textureBrushOffset.x--;
				if(Graphics.textureBrushOffset.x < -Graphics.textureGridSizeX)
					Graphics.textureBrushOffset.x = -Graphics.textureGridSizeX;
				break;
			case SDLK_RIGHT:
				Graphics.textureBrushOffset.x++;
				if(Graphics.textureBrushOffset.x > Graphics.textureGridSizeX)
					Graphics.textureBrushOffset.x = Graphics.textureGridSizeX;
				break;
			case SDLK_UP:
				Graphics.textureBrushOffset.y--;
				if(Graphics.textureBrushOffset.y < -Graphics.textureGridSizeY)
					Graphics.textureBrushOffset.y = -Graphics.textureGridSizeY;
				break;
			case SDLK_DOWN:
				Graphics.textureBrushOffset.y++;
				if(Graphics.textureBrushOffset.y > Graphics.textureGridSizeY)
					Graphics.textureBrushOffset.y = Graphics.textureGridSizeY;
				break;
			case SDLK_h:
				Graphics.fliph = !Graphics.fliph;
				break;
			case SDLK_v:
				Graphics.flipv = !Graphics.flipv;
				break;
			case SDLK_MINUS:
				Graphics.textureBrushSize = max(Graphics.textureBrushSize-1,1);
				break;
			case SDLK_EQUALS:
				Graphics.textureBrushSize = min(Graphics.textureBrushSize+1, 16);
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
