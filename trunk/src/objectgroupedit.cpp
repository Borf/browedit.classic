#include <common.h>
#include <wm/window.h>
#include "graphics.h"
#include "undo/objectschange.h"
#include "undo/objectsnew.h"
#include "undo/objectsdelete.h"
#include "menu.h"
#include <wm/wm.h>

extern bool doneAction;
extern cMenu* snaptofloor;


int cProcessManagement::objectgroupedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if (cGraphics::cMouse::lbuttondown && !cGraphics::cMouse::rbuttondown && cGraphics::groupeditmode)
			{
				unsigned int i;
				if(doneAction)
				{
					std::vector<int> objectsselected;
					for(i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							objectsselected.push_back(i);
					if (objectsselected.size() > 0)
						cGraphics::worldContainer->undoStack->push(new cUndoChangeObjects(objectsselected));
					doneAction = false;
				}
				bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
				bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
				bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
				for(i = 0; i < cGraphics::world->models.size(); i++)
				{
					if (!cGraphics::world->models[i]->selected)
						continue;

					if (!ctrl && !alt)
					{
						bEngine::math::cVector3 diff = cGraphics::world->models[i]->pos - cGraphics::selectionCenter;

						if(shift)
						{
							cGraphics::cMouse::x3d = round(cGraphics::cMouse::x3d/5)*5;
							cGraphics::cMouse::z3d = round(cGraphics::cMouse::z3d/5)*5;
						}
						cGraphics::world->models[i]->pos.x = (cGraphics::cMouse::x3d/5.0f) + diff.x;
						cGraphics::world->models[i]->pos.z = (cGraphics::cMouse::z3d/5.0f) + diff.z;
						if(snaptofloor->ticked)
							cGraphics::world->models[i]->pos.y = -cGraphics::cMouse::y3d + diff.y;
						
					}
					if(ctrl && !alt)
					{
						bEngine::math::cVector2 diff = bEngine::math::cVector2(cGraphics::world->models[i]->pos.x - cGraphics::selectionCenter.x, cGraphics::world->models[i]->pos.z - cGraphics::selectionCenter.z);
						if(!shift)
						{
							cGraphics::world->models[i]->rot.y -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/10.0f;
							diff = diff.getRotated((cGraphics::cMouse::x-cGraphics::cMouse::xOld)/10.0f);
						}
						else
						{
							diff = diff.getRotated((cGraphics::cMouse::x-cGraphics::cMouse::xOld)*9);
							cGraphics::world->models[i]->rot.y -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)*9;
						}

						cGraphics::world->models[i]->pos.x = cGraphics::selectionCenter.x + diff.x;
						cGraphics::world->models[i]->pos.z = cGraphics::selectionCenter.z + diff.y;

					}
					if(alt && !ctrl)
					{
						bEngine::math::cVector2 diff = bEngine::math::cVector2(cGraphics::world->models[i]->pos.x - cGraphics::selectionCenter.x, cGraphics::world->models[i]->pos.z - cGraphics::selectionCenter.z);
						diff = diff * (1 + ((cGraphics::cMouse::x - cGraphics::cMouse::xOld) / 10.0f));
						cGraphics::world->models[i]->pos.x = cGraphics::selectionCenter.x + diff.x;
						cGraphics::world->models[i]->pos.z = cGraphics::selectionCenter.z + diff.y;

						cGraphics::world->models[i]->scale = cGraphics::world->models[i]->scale * (1+((cGraphics::cMouse::x - cGraphics::cMouse::xOld) / 10.0f));
					}
				}
				if(!ctrl && !alt)
				{
					int count = 0;
					cGraphics::selectionCenter = bEngine::math::cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							cGraphics::selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					cGraphics::selectionCenter = cGraphics::selectionCenter / count;
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT && !cGraphics::groupeditmode)
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

				
				unsigned int i;
				if(!ctrl && !alt)
				{
					for(i = 0; i < cGraphics::world->models.size(); i++)
						cGraphics::world->models[i]->selected = false;
				}


				for(i = 0; i < cGraphics::world->models.size(); i++)
				{
					bEngine::math::cVector3* pos = &cGraphics::world->models[i]->pos;
					if (pos->x*5 > cGraphics::cMouse::x3dStart && pos->x*5 < cGraphics::cMouse::x3d && pos->z*5 > cGraphics::cMouse::z3dStart && pos->z*5 < cGraphics::cMouse::z3d)
					{
						cGraphics::world->models[i]->selected = !alt;
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
				{
					cGraphics::groupeditmode = !cGraphics::groupeditmode;
					int count = 0;
					cGraphics::selectionCenter = bEngine::math::cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							cGraphics::selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					cGraphics::selectionCenter = cGraphics::selectionCenter / count;
				}
				break;
			case SDLK_RIGHTBRACKET:
				{
					cGraphics::groupeditmode = !cGraphics::groupeditmode;
					int count = 0;
					cGraphics::selectionCenter = bEngine::math::cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							cGraphics::selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					cGraphics::selectionCenter = cGraphics::selectionCenter / count;
				}
				break;
			case SDLK_d:
				{
					cGraphics::worldContainer->undoStack->push(new cUndoNewObjects(cGraphics::world->models.size()));
					int start = cGraphics::world->models.size();
					int i;
					for(i = 0; i < start; i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							cGraphics::world->models[i]->selected = false;
							cRsmModel* model = new cRsmModel(cGraphics::world->models[i]->filename);
							model->pos = cGraphics::world->models[i]->pos + bEngine::math::cVector3(4,0,4);
							model->scale = cGraphics::world->models[i]->scale;
							model->rot = cGraphics::world->models[i]->rot;
							model->selected = true;
							model->lightopacity = cGraphics::world->models[i]->lightopacity;
							cGraphics::world->models.push_back(model);
						}
					}

				}
				break;
			case SDLK_r:
				{

					std::string cat = cWM::inputWindow("Input Category");

					if(cat == "")
						break;

					int ii = 0;
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if(cGraphics::world->models[i]->selected)
						{
							char buf[100];
							sprintf(buf, "%s\\%i", cat.c_str(), ii);
							cGraphics::world->models[i]->name = std::string(buf);
							ii++;
						}
					}
					cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);

					break;
				}
			case SDLK_PAGEDOWN:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.y++;
				}
				break;
			case SDLK_PAGEUP:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.y--;
				}
				break;
			case SDLK_UP:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.z++;
				}
				break;
			case SDLK_DOWN:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.z--;
				}
				break;
			case SDLK_LEFT:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.x--;
				}
				break;
			case SDLK_RIGHT:
				{
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							cGraphics::world->models[i]->pos.x++;
				}
				break;
			case SDLK_BACKSPACE:
				{
					std::vector<cUndoObjectsDelete::cObject> objectsdeleted;
					int idoff = 0;
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							cUndoObjectsDelete::cObject object;
							object.filename = cGraphics::world->models[i]->filename;
							object.pos = cGraphics::world->models[i]->pos;
							object.rot = cGraphics::world->models[i]->rot;
							object.scale = cGraphics::world->models[i]->scale;
							object.id = i-idoff;
							objectsdeleted.push_back(object);
							delete cGraphics::world->models[i];
							cGraphics::world->models.erase(cGraphics::world->models.begin() + i);
							i--;
							idoff--;
						}
					}
					cGraphics::worldContainer->settings.selectedObject = -1;
					if (objectsdeleted.size() > 0)
						cGraphics::worldContainer->undoStack->push(new cUndoObjectsDelete(objectsdeleted));
					cWindow* w = cWM::getWindow(WT_MODELOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);
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
