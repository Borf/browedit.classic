#include <common.h>
#include "graphics.h"
#include "undo.h"

extern float mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern double mouse3dxstart, mouse3dystart, mouse3dzstart;
#include "wm/areacopywindow.h"


int cProcessManagement::globalheightedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(mousex < Graphics.w()-256)
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

					for(x = (int)floor(mouse3dxstart/10); x < floor(mouse3dx/10); x++)
					{
						for(y = (int)floor(mouse3dzstart/10); y < (int)floor(mouse3dz/10); y++)
						{
							if (x >= 0 && x < Graphics.world.width && y >= 0 && y < Graphics.world.height)
							{
								Graphics.world.cubes[y][x].selected = !alt;
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
			case SDLK_PAGEDOWN:
				{
					undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
					for(int x = 0; x < Graphics.world.width; x++)
					{
						for(int y = 0; y < Graphics.world.height; y++)
						{
							if(!Graphics.world.cubes[y][x].selected)
								continue;
							Graphics.world.cubes[y][x].cell1 += 1;
							Graphics.world.cubes[y][x].cell2 += 1;
							Graphics.world.cubes[y][x].cell3 += 1;
							Graphics.world.cubes[y][x].cell4 += 1;
						}
					}
					for(int y = 0; y < Graphics.world.gattiles.size(); y++)
					{
						for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
						{
							if(!Graphics.world.cubes[y/2][x/2].selected)
								continue;
							Graphics.world.gattiles[y][x].cell1 += 1;
							Graphics.world.gattiles[y][x].cell2 += 1;
							Graphics.world.gattiles[y][x].cell3 += 1;
							Graphics.world.gattiles[y][x].cell4 += 1;
						}
					}
				}
				break;
			case SDLK_PAGEUP:
				{
					undostack.push(new cUndoHeightEdit(0,0,Graphics.world.width, Graphics.world.height));
					for(int x = 0; x < Graphics.world.width; x++)
					{
						for(int y = 0; y < Graphics.world.height; y++)
						{
							if(!Graphics.world.cubes[y][x].selected)
								continue;
							Graphics.world.cubes[y][x].cell1 -= 1;
							Graphics.world.cubes[y][x].cell2 -= 1;
							Graphics.world.cubes[y][x].cell3 -= 1;
							Graphics.world.cubes[y][x].cell4 -= 1;
						}
					}
					for(int y = 0; y < Graphics.world.gattiles.size(); y++)
					{
						for(int x = 0; x < Graphics.world.gattiles[y].size(); x++)
						{
							if(!Graphics.world.cubes[y/2][x/2].selected)
								continue;
							Graphics.world.gattiles[y][x].cell1 -= 1;
							Graphics.world.gattiles[y][x].cell2 -= 1;
							Graphics.world.gattiles[y][x].cell3 -= 1;
							Graphics.world.gattiles[y][x].cell4 -= 1;
						}
					}
				}
				break;
			case SDLK_c:
				if(Graphics.WM.getwindow(WT_AREACOPY) != NULL)
					return 0;
				Graphics.WM.addwindow(new cAreaCopyWindow(Graphics.WM.texture, &Graphics.WM.font) );
				break;

			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
