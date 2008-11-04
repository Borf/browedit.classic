#include <common.h>
#include "graphics.h"
#include "undo.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern double mouse3dxstart, mouse3dystart, mouse3dzstart;
#include "windows/areacopywindow.h"


int cProcessManagement::globalheightedit_process_events(SDL_Event &event)
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

					for(x = (int)floor(mouse3dxstart/10); x < floor(mouse3dx/10); x++)
					{
						for(y = (int)floor(mouse3dzstart/10); y < (int)floor(mouse3dz/10); y++)
						{
							if (x >= 0 && x < cGraphics::world->width && y >= 0 && y < cGraphics::world->height)
							{
								cGraphics::world->cubes[y][x].selected = !alt;
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
					cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
					for(int x = 0; x < cGraphics::world->width; x++)
					{
						for(int y = 0; y < cGraphics::world->height; y++)
						{
							if(!cGraphics::world->cubes[y][x].selected)
								continue;
							cGraphics::world->cubes[y][x].cell1 += 1;
							cGraphics::world->cubes[y][x].cell2 += 1;
							cGraphics::world->cubes[y][x].cell3 += 1;
							cGraphics::world->cubes[y][x].cell4 += 1;
						}
					}
					for(unsigned int y = 0; y < cGraphics::world->gattiles.size(); y++)
					{
						for(unsigned int x = 0; x < cGraphics::world->gattiles[y].size(); x++)
						{
							if(!cGraphics::world->cubes[y/2][x/2].selected)
								continue;
							cGraphics::world->gattiles[y][x].cell1 += 1;
							cGraphics::world->gattiles[y][x].cell2 += 1;
							cGraphics::world->gattiles[y][x].cell3 += 1;
							cGraphics::world->gattiles[y][x].cell4 += 1;
						}
					}
				}
				break;
			case SDLK_PAGEUP:
				{
					cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(0,0,cGraphics::world->width, cGraphics::world->height));
					for(int x = 0; x < cGraphics::world->width; x++)
					{
						for(int y = 0; y < cGraphics::world->height; y++)
						{
							if(!cGraphics::world->cubes[y][x].selected)
								continue;
							cGraphics::world->cubes[y][x].cell1 -= 1;
							cGraphics::world->cubes[y][x].cell2 -= 1;
							cGraphics::world->cubes[y][x].cell3 -= 1;
							cGraphics::world->cubes[y][x].cell4 -= 1;
						}
					}
					for(unsigned int y = 0; y < cGraphics::world->gattiles.size(); y++)
					{
						for(unsigned int x = 0; x < cGraphics::world->gattiles[y].size(); x++)
						{
							if(!cGraphics::world->cubes[y/2][x/2].selected)
								continue;
							cGraphics::world->gattiles[y][x].cell1 -= 1;
							cGraphics::world->gattiles[y][x].cell2 -= 1;
							cGraphics::world->gattiles[y][x].cell3 -= 1;
							cGraphics::world->gattiles[y][x].cell4 -= 1;
						}
					}
				}
				break;
			case SDLK_c:
				if(cWM::getWindow(WT_AREACOPY) != NULL)
					return 0;
				cWM::addWindow(new cAreaCopyWindow() );
				break;

			default:
				break;
			}

			break;
		}
		
			
	}



	return true;


}
