#include <common.h>
#include "graphics.h"
#include "undo/effectchange.h"
#include "undo/effectdelete.h"
#include "menu.h"
#include "windows/effectwindow.h"

extern cMenuItem* selectedeffect;

int cProcessManagement::effectedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(cGraphics::cMouse::lbuttondown)
			{
				if (cGraphics::world->effects.size() == 0)
					break;
				if(cGraphics::objectStartDrag)
				{
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.x = cGraphics::cMouse::x3d / 5.0f;
						cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.z = cGraphics::cMouse::z3d / 5.0f;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.x = floor(cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.x * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
							cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.z = floor(cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.z * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsety) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsety/(cGraphics::worldContainer->settings.gridSize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.y += (cGraphics::cMouse::y-cGraphics::cMouse::yOld);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.y = floor(cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].pos.y * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(cGraphics::world->effects.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < cGraphics::world->effects.size(); i++)
				{
					cVector3 d = cGraphics::world->effects[i].pos;
					d.x = d.x;
					
					d.x -= cGraphics::cMouse::x3d/5;
					d.z -= cGraphics::cMouse::z3d/5;
					d.y = 0;

					if(mindist > d.magnitude())
					{
						mindist = d.magnitude();
						minobj = i;
					}
				}
				cGraphics::objectStartDrag = cGraphics::worldContainer->settings.selectedObject == minobj;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && cWM::movement < 3)
			{
				if (SDL_GetModState() & KMOD_CTRL)
				{
					if (selectedeffect == NULL)
						break;
					cEffect e;
					char buf[100];
					sprintf(buf, "obj%i", rand());
					e.category = "\0\0\0\0";
					e.loop = 40;
					e.pos = cVector3(cGraphics::cMouse::x3d/5, -cGraphics::cMouse::y3d-10, cGraphics::cMouse::z3d/5);
					e.name = buf;
					e.readablename = selectedeffect->title;
					e.type = atoi(selectedeffect->data.c_str());
					e.todo1 = 1;
					e.todo2 = 1;
					e.todo3 = 1;
					e.rot = cVector3(0,0,0);
					e.scale = cVector3(1,1,1);
					e.todo10 = 1; // seems to be linked to 11
					e.todo11 = 1; // seems to be linked to 10
					e.todo12 = 0; // seems to be always 0
					e.todo13 = 0; // seems to be always 0

					cGraphics::world->effects.push_back(e);
				}
				else
				{
					if (cGraphics::world->effects.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < cGraphics::world->effects.size(); i++)
					{
						cVector3 d = cGraphics::world->effects[i].pos;
						d.x = d.x;
						
						d.x -= cGraphics::cMouse::x3d/5.0f;
						d.z -= cGraphics::cMouse::z3d/5.0f;
						d.y = 0;

						if(mindist > d.magnitude())
						{
							mindist = d.magnitude();
							minobj = i;
						}
					}
					cGraphics::worldContainer->settings.selectedObject = minobj;
					char buf[100];
					sprintf(buf, "%i", cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].type);
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_PAGEDOWN:
				if (cGraphics::worldContainer->settings.selectedObject != -1)
				{
					cGraphics::worldContainer->undoStack->push(new cUndoChangeEffect(cGraphics::worldContainer->settings.selectedObject));
					cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].loop--;
				}
				break;
			case SDLK_PAGEUP:
				if (cGraphics::worldContainer->settings.selectedObject != -1)
				{
					cGraphics::worldContainer->undoStack->push(new cUndoChangeEffect(cGraphics::worldContainer->settings.selectedObject));
					cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject].loop++;
				}
				break;
			case SDLK_BACKSPACE:
				if (cGraphics::worldContainer->settings.selectedObject > -1 && cGraphics::worldContainer->settings.selectedObject < (int)cGraphics::world->effects.size())
				{
					cGraphics::worldContainer->undoStack->push(new cUndoEffectDelete(cGraphics::worldContainer->settings.selectedObject));
					cGraphics::world->effects.erase(cGraphics::world->effects.begin() + cGraphics::worldContainer->settings.selectedObject);
					cGraphics::worldContainer->settings.selectedObject = -1;
				}
				break;
			case SDLK_RETURN:
				if (cGraphics::worldContainer->settings.selectedObject != -1 && cGraphics::worldContainer->settings.selectedObject < (int)cGraphics::world->effects.size())
				{
					cEffect* o = &cGraphics::world->effects[cGraphics::worldContainer->settings.selectedObject];

					cWindow* w = new cEffectWindow();
					((cWindowFloatInputBox*)w->objects["posx"])->floatje = &o->pos.x;
					((cWindowFloatInputBox*)w->objects["posy"])->floatje = &o->pos.y;
					((cWindowFloatInputBox*)w->objects["posz"])->floatje = &o->pos.z;
					((cWindowFloatInputBox*)w->objects["rotx"])->floatje = &o->rot.x;
					((cWindowFloatInputBox*)w->objects["roty"])->floatje = &o->rot.y;
					((cWindowFloatInputBox*)w->objects["rotz"])->floatje = &o->rot.z;
					((cWindowFloatInputBox*)w->objects["scalex"])->floatje = &o->scale.x;
					((cWindowFloatInputBox*)w->objects["scaley"])->floatje = &o->scale.y;
					((cWindowFloatInputBox*)w->objects["scalez"])->floatje = &o->scale.z;
					((cWindowFloatInputBox*)w->objects["looptime"])->floatje = &o->loop;

					w->objects["objectname"]->setText(0, o->readablename);
					((cEffectWindow*)w)->undo = new cUndoChangeEffect(cGraphics::worldContainer->settings.selectedObject);
					cWM::addWindow(w);
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
