#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern double mouse3dxstart, mouse3dystart, mouse3dzstart;
#include "wm/areacopywindow.h"


int cProcessManagement::texturepaintedit_process_events(SDL_Event &event)
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
			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
