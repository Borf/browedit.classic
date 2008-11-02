#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"

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
					for(i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							objectsselected.push_back(i);
					if (objectsselected.size() > 0)
						undostack.push(new cUndoChangeObjects(objectsselected));
					doneaction = false;
				}
				bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
				bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
				bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
				for(i = 0; i < Graphics.world.models.size(); i++)
				{
					if (!Graphics.world.models[i]->selected)
						continue;

					if (!ctrl && !alt)
					{
						cVector3 diff = Graphics.world.models[i]->pos - Graphics.selectionCenter;

						Graphics.world.models[i]->pos.x = (mouse3dx/5.0f) + diff.x;
						Graphics.world.models[i]->pos.z = (mouse3dz/5.0f) + diff.z;
						if(snaptofloor->ticked)
							Graphics.world.models[i]->pos.y = -mouse3dy + diff.y;
						
					}
					if(ctrl && !alt)
					{
						cVector2 diff = cVector2(Graphics.world.models[i]->pos.x - Graphics.selectionCenter.x, Graphics.world.models[i]->pos.z - Graphics.selectionCenter.z);
						if(!shift)
						{
							Graphics.world.models[i]->rot.y -= (mouseX - oldmousex)/10.0f;
							diff.rotate((mouseX-oldmousex)/10.0f);
						}
						else
						{
							diff.rotate((mouseX-oldmousex)*9);
							Graphics.world.models[i]->rot.y -= (mouseX - oldmousex)*9;
						}

						Graphics.world.models[i]->pos.x = Graphics.selectionCenter.x + diff.x;
						Graphics.world.models[i]->pos.z = Graphics.selectionCenter.z + diff.y;

					}
					if(alt && !ctrl)
					{
						cVector2 diff = cVector2(Graphics.world.models[i]->pos.x - Graphics.selectionCenter.x, Graphics.world.models[i]->pos.z - Graphics.selectionCenter.z);
						diff = diff * (1 + ((mouseX - oldmousex) / 10.0f));
						Graphics.world.models[i]->pos.x = Graphics.selectionCenter.x + diff.x;
						Graphics.world.models[i]->pos.z = Graphics.selectionCenter.z + diff.y;

						Graphics.world.models[i]->scale = Graphics.world.models[i]->scale * (1+((mouseX - oldmousex) / 10.0f));
					}
				}
				if(!ctrl && !alt)
				{
					int count = 0;
					Graphics.selectionCenter = cVector3(0,0,0);
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=Graphics.world.models[i]->pos;
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
					for(i = 0; i < Graphics.world.models.size(); i++)
						Graphics.world.models[i]->selected = false;
				}


				for(i = 0; i < Graphics.world.models.size(); i++)
				{
					cVector3* pos = &Graphics.world.models[i]->pos;
					if (pos->x*5 > mouse3dxstart && pos->x*5 < mouse3dx && pos->z*5 > mouse3dzstart && pos->z*5 < mouse3dz)
					{
						Graphics.world.models[i]->selected = !alt;
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
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=Graphics.world.models[i]->pos;
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
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							count++;
							Graphics.selectionCenter+=Graphics.world.models[i]->pos;
						}
					}
					Graphics.selectionCenter = Graphics.selectionCenter / count;
				}
				break;
			case SDLK_d:
				{
					undostack.push(new cUndoNewObjects(Graphics.world.models.size()));
					int start = Graphics.world.models.size();
					int i;
					for(i = 0; i < start; i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							Graphics.world.models[i]->selected = false;
							cRSMModel* model = new cRSMModel();
							model->load(Graphics.world.models[i]->filename);
							model->pos = Graphics.world.models[i]->pos + cVector3(4,0,4);
							model->scale = Graphics.world.models[i]->scale;
							model->rot = Graphics.world.models[i]->rot;
							model->selected = true;
							Graphics.world.models.push_back(model);
						}
					}

				}
				break;
			case SDLK_r:
				{

					std::string cat = cWM::InputWindow("Input Category");

					if(cat == "")
						break;

					int ii = 0;
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
					{
						if(Graphics.world.models[i]->selected)
						{
							char buf[100];
							sprintf(buf, "%s\\%i", cat.c_str(), ii);
							Graphics.world.models[i]->name = buf;
							ii++;
						}
					}
					cWindow* w = cWM::getwindow(WT_MODELOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);

					break;
				}
			case SDLK_PAGEDOWN:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.y++;
				}
				break;
			case SDLK_PAGEUP:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.y--;
				}
				break;
			case SDLK_UP:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.z++;
				}
				break;
			case SDLK_DOWN:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.z--;
				}
				break;
			case SDLK_LEFT:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.x--;
				}
				break;
			case SDLK_RIGHT:
				{
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
						if(Graphics.world.models[i]->selected)
							Graphics.world.models[i]->pos.x++;
				}
				break;
			case SDLK_BACKSPACE:
				{
					std::vector<cUndoObjectsDelete::cObject> objectsdeleted;
					int idoff = 0;
					for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
					{
						if (Graphics.world.models[i]->selected)
						{
							cUndoObjectsDelete::cObject object;
							object.filename = Graphics.world.models[i]->filename;
							object.pos = Graphics.world.models[i]->pos;
							object.rot = Graphics.world.models[i]->rot;
							object.scale = Graphics.world.models[i]->scale;
							object.id = i-idoff;
							objectsdeleted.push_back(object);
							delete Graphics.world.models[i];
							Graphics.world.models.erase(Graphics.world.models.begin() + i);
							i--;
							idoff--;
						}
					}
					Graphics.selectedObject = -1;
					if (objectsdeleted.size() > 0)
						undostack.push(new cUndoObjectsDelete(objectsdeleted));
					cWindow* w = cWM::getwindow(WT_MODELOVERVIEW);
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
