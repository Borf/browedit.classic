#include <common.h>
#include <wm/window.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"

extern long mouseX;
extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown, rbuttondown,doneaction;
extern cMenu* snaptofloor;
extern float oldmousex, oldmousey;
extern double mouse3dxstart, mouse3dystart, mouse3dzstart;


int cProcessManagement::objectgroupedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if (lbuttondown && !rbuttondown && Graphics.groupeditmode)
			{
				unsigned int i;
				if(doneaction)
				{
					std::vector<int> objectsselected;
					for(i = 0; i < cGraphics::world->models.size(); i++)
						if(cGraphics::world->models[i]->selected)
							objectsselected.push_back(i);
					if (objectsselected.size() > 0)
						undostack.push(new cUndoChangeObjects(objectsselected));
					doneaction = false;
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
						cVector3 diff = cGraphics::world->models[i]->pos - Graphics.selectionCenter;

						cGraphics::world->models[i]->pos.x = (mouse3dx/5.0f) + diff.x;
						cGraphics::world->models[i]->pos.z = (mouse3dz/5.0f) + diff.z;
						if(snaptofloor->ticked)
							cGraphics::world->models[i]->pos.y = -mouse3dy + diff.y;
						
					}
					if(ctrl && !alt)
					{
						cVector2 diff = cVector2(cGraphics::world->models[i]->pos.x - Graphics.selectionCenter.x, cGraphics::world->models[i]->pos.z - Graphics.selectionCenter.z);
						if(!shift)
						{
							cGraphics::world->models[i]->rot.y -= (mouseX - oldmousex)/10.0f;
							diff.rotate((mouseX-oldmousex)/10.0f);
						}
						else
						{
							diff.rotate((mouseX-oldmousex)*9);
							cGraphics::world->models[i]->rot.y -= (mouseX - oldmousex)*9;
						}

						cGraphics::world->models[i]->pos.x = Graphics.selectionCenter.x + diff.x;
						cGraphics::world->models[i]->pos.z = Graphics.selectionCenter.z + diff.y;

					}
					if(alt && !ctrl)
					{
						cVector2 diff = cVector2(cGraphics::world->models[i]->pos.x - Graphics.selectionCenter.x, cGraphics::world->models[i]->pos.z - Graphics.selectionCenter.z);
						diff = diff * (1 + ((mouseX - oldmousex) / 10.0f));
						cGraphics::world->models[i]->pos.x = Graphics.selectionCenter.x + diff.x;
						cGraphics::world->models[i]->pos.z = Graphics.selectionCenter.z + diff.y;

						cGraphics::world->models[i]->scale = cGraphics::world->models[i]->scale * (1+((mouseX - oldmousex) / 10.0f));
					}
				}
				if(!ctrl && !alt)
				{
					int count = 0;
					Graphics.selectionCenter = cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					Graphics.selectionCenter = Graphics.selectionCenter / count;
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT && !Graphics.groupeditmode)
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

				
				unsigned int i;
				if(!ctrl && !alt)
				{
					for(i = 0; i < cGraphics::world->models.size(); i++)
						cGraphics::world->models[i]->selected = false;
				}


				for(i = 0; i < cGraphics::world->models.size(); i++)
				{
					cVector3* pos = &cGraphics::world->models[i]->pos;
					if (pos->x*5 > mouse3dxstart && pos->x*5 < mouse3dx && pos->z*5 > mouse3dzstart && pos->z*5 < mouse3dz)
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
					Graphics.groupeditmode = !Graphics.groupeditmode;
					int count = 0;
					Graphics.selectionCenter = cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					Graphics.selectionCenter = Graphics.selectionCenter / count;
				}
				break;
			case SDLK_RIGHTBRACKET:
				{
					Graphics.groupeditmode = !Graphics.groupeditmode;
					int count = 0;
					Graphics.selectionCenter = cVector3(0,0,0);
					for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=cGraphics::world->models[i]->pos;
						}
					}
					Graphics.selectionCenter = Graphics.selectionCenter / count;
				}
				break;
			case SDLK_d:
				{
					undostack.push(new cUndoNewObjects(cGraphics::world->models.size()));
					int start = cGraphics::world->models.size();
					int i;
					for(i = 0; i < start; i++)
					{
						if (cGraphics::world->models[i]->selected)
						{
							cGraphics::world->models[i]->selected = false;
							cRSMModel* model = new cRSMModel();
							model->load(cGraphics::world->models[i]->filename);
							model->pos = cGraphics::world->models[i]->pos + cVector3(4,0,4);
							model->scale = cGraphics::world->models[i]->scale;
							model->rot = cGraphics::world->models[i]->rot;
							model->selected = true;
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
							cGraphics::world->models[i]->name = buf;
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
					Graphics.selectedObject = -1;
					if (objectsdeleted.size() > 0)
						undostack.push(new cUndoObjectsDelete(objectsdeleted));
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
