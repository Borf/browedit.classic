#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include "wm/objectwindow.h"
#include "wm/modeloverviewwindow.h"

#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)
MENUCOMMAND(model);

extern float mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown,rbuttondown;
extern bool	doneaction;
extern cMenu* snaptofloor;
extern float oldmousex, oldmousey;
extern int movement;
extern cMenu* currentobject;
extern cMenu* models;

int cProcessManagement::objectedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if (lbuttondown && !rbuttondown)
			{
				int minobj = 0;
				float mindist = 999999;
				if(Graphics.objectstartdrag && Graphics.selectedobject != -1)
				{
					if (doneaction)
					{
						undostack.push(new cUndoChangeObject(Graphics.selectedobject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.models[Graphics.selectedobject]->pos.x = mouse3dx / 5;
						if(snaptofloor->ticked)
							Graphics.world.models[Graphics.selectedobject]->pos.y = -mouse3dy;
						Graphics.world.models[Graphics.selectedobject]->pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.models[Graphics.selectedobject]->pos.x = floor(Graphics.world.models[Graphics.selectedobject]->pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.models[Graphics.selectedobject]->pos.z = floor(Graphics.world.models[Graphics.selectedobject]->pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.models[Graphics.selectedobject]->rot.x += mousey - oldmousey;
						Graphics.world.models[Graphics.selectedobject]->rot.y += mousex - oldmousex;
						if (SDL_GetModState() & KMOD_SHIFT)
						{

							Graphics.world.models[Graphics.selectedobject]->rot.x = floor((Graphics.world.models[Graphics.selectedobject]->rot.x+22.5) / 45) * 45;
							Graphics.world.models[Graphics.selectedobject]->rot.y = floor((Graphics.world.models[Graphics.selectedobject]->rot.y+22.5) / 45) * 45;
						}
					}
					if(!ctrl && alt)
					{
 						Graphics.world.models[Graphics.selectedobject]->scale.x += (mousex - oldmousex)/10.0;
						Graphics.world.models[Graphics.selectedobject]->scale.y += (mousex - oldmousex)/10.0;
						Graphics.world.models[Graphics.selectedobject]->scale.z += (mousex - oldmousex)/10.0;
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				int minobj = 0;
				float mindist = 999999;
				for(int i = 0; i < Graphics.world.models.size(); i++)
				{
					cVector3 d = Graphics.world.models[i]->pos;
					d.x = d.x;
					
					d.x -= mouse3dx/5;
					d.z -= mouse3dz/5;
					d.y = 0;

					if(mindist > d.Magnitude())
					{
						mindist = d.Magnitude();
						minobj = i;
					}
				}
				Graphics.objectstartdrag = Graphics.selectedobject == minobj;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{
				if (SDL_GetModState() & KMOD_CTRL && Graphics.previewmodel != NULL)
				{
					cRSMModel* model = new cRSMModel();
					model->load(Graphics.previewmodel->filename);
					model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
					model->scale = cVector3(1,1,1);
					model->rot = cVector3(0,0,0);
					char buf[100];
					sprintf(buf, "%s-%i", Graphics.previewmodel->rofilename.c_str(), rand()%100);
					Graphics.world.models.push_back(model);
					Graphics.selectedobject = Graphics.world.models.size()-1;
					undostack.push(new cUndoNewObject());
				}
				else
				{
					int minobj = 0;
					float mindist = 999999;
					for(int i = 0; i < Graphics.world.models.size(); i++)
					{
						cVector3 d = Graphics.world.models[i]->pos;
						d.x = d.x;
						
						d.x -= mouse3dx/5;
						d.z -= mouse3dz/5;
						d.y = 0;

						if(mindist > d.Magnitude())
						{
							mindist = d.Magnitude();
							minobj = i;
						}
					}
					Graphics.selectedobject = minobj;
				}
				cWindow* w = Graphics.WM.getwindow(WT_MODELOVERVIEW);
				if(w != NULL)
				{
					w->userfunc(NULL);
					if(Graphics.selectedobject >= 0 && Graphics.selectedobject < Graphics.world.models.size()-1)
					{
						cModelOverViewWindow::cModelOverViewTree* tree = (cModelOverViewWindow::cModelOverViewTree*)w->objects["list"];
						tree->getobject(Graphics.world.models[Graphics.selectedobject]);
					}
				}
			}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.z+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.z+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.z+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_DOWN:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.z-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.z-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.z-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_RIGHT:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.x+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.x+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.x+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_LEFT:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.x-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.x-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.x-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_MINUS:
				if (Graphics.previewmodel != NULL)
					Graphics.previewmodel->scale = Graphics.previewmodel->scale * 0.9f;
				break;
			case SDLK_EQUALS:
				if (Graphics.previewmodel != NULL)
					Graphics.previewmodel->scale = Graphics.previewmodel->scale * 1.1f;
				break;
			case SDLK_LEFTBRACKET:
				if (currentobject != NULL)
				{
					currentobject = currentobject->parent->getprev(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_RIGHTBRACKET:
				if (currentobject != NULL)
				{
					currentobject = currentobject->parent->getnext(currentobject);
					MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_h:
				if (Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					if(SDL_GetModState() & KMOD_ALT)
						Graphics.world.models[Graphics.selectedobject]->scale.y = -	Graphics.world.models[Graphics.selectedobject]->scale.y;
					else
						Graphics.world.models[Graphics.selectedobject]->scale.x = -	Graphics.world.models[Graphics.selectedobject]->scale.x;
				}
				break;
			case SDLK_v:
				if (Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					Graphics.world.models[Graphics.selectedobject]->scale.z = -	Graphics.world.models[Graphics.selectedobject]->scale.z;
				}
				break;
			case SDLK_PAGEDOWN:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.y+=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.y+=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.y+=0.1 + shift ? 0.1 : 0;
				}						
				break;
			case SDLK_PAGEUP:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					bool shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
					if (!ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->pos.y-=0.1 + (shift ? 0.4 : 0);
					if (ctrl && !alt)
						Graphics.world.models[Graphics.selectedobject]->rot.y-=1 + (shift ? 44 : 0);
					if (!ctrl && alt)
						Graphics.world.models[Graphics.selectedobject]->scale.y-=0.1 + shift ? 0.1 : 0;
				}
				break;
			case SDLK_c:
				if (Graphics.selectedobject != -1)
				{
					Graphics.clipboardrot = Graphics.world.models[Graphics.selectedobject]->rot;
					Graphics.clipboardscale = Graphics.world.models[Graphics.selectedobject]->scale;
					Graphics.clipboardfile = Graphics.world.models[Graphics.selectedobject]->filename;
					Graphics.clipboardy = Graphics.world.models[Graphics.selectedobject]->pos.y;
					currentobject = models->finddata("data\\model\\" + Graphics.world.models[Graphics.selectedobject]->rofilename);
					if(currentobject != NULL)
						MenuCommand_model((cMenuItem*)currentobject);
				}
				break;
			case SDLK_p:
				if (Graphics.clipboardfile != "")
				{
					if (SDL_GetModState() & KMOD_CTRL)
					{
						Graphics.world.models[Graphics.selectedobject]->pos.y = Graphics.clipboardy;
					}
					else
					{
						cRSMModel* model = new cRSMModel();
						model->load(Graphics.clipboardfile);
						model->pos = cVector3(mouse3dx/5, -mouse3dy, mouse3dz/5);
						if (SDL_GetModState() & KMOD_SHIFT)
							model->pos.y = Graphics.clipboardy;
						model->scale = Graphics.clipboardscale;
						model->rot = Graphics.clipboardrot;
						Graphics.world.models.push_back(model);
						Graphics.selectedobject = Graphics.world.models.size()-1;
						undostack.push(new cUndoNewObject());
					}
				}
				break;
			case SDLK_BACKSPACE:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.models.size())
				{
					undostack.push(new cUndoObjectDelete(Graphics.selectedobject));
					delete Graphics.world.models[Graphics.selectedobject];
					Graphics.world.models.erase(Graphics.world.models.begin() + Graphics.selectedobject);
					Graphics.selectedobject = -1;
					cWindow* w = Graphics.WM.getwindow(WT_MODELOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);
				}
				break;
			case SDLK_i:
				if (Graphics.selectedobject != -1)
				{
					cRSMModel* m = Graphics.world.models[Graphics.selectedobject];
					Log(3,0,"Objects: %i", Graphics.selectedobject);
					Log(3,0,"Pos: %f,%f,%f", m->pos.x, m->pos.y, m->pos.z);
					Log(3,0,"scale: %f,%f,%f", m->scale.x, m->scale.y, m->scale.z);
					Log(3,0,"rot: %f,%f,%f", m->rot.x, m->rot.y, m->rot.z);
					Log(3,0,"nr of submeshes: %i", m->meshes.size());
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedobject != -1)
				{
					cRSMModel* o = Graphics.world.models[Graphics.selectedobject];
					cMenuItem* menuitem = (cMenuItem*)models->finddata("data\\model\\" + o->rofilename);

					cWindow* w = new cObjectWindow(Graphics.WM.texture, &Graphics.WM.font);
					if (menuitem != NULL)
						w->objects["objectmenu"]->SetText(0,menuitem->data2);
					w->objects["posx"]->SetInt(3,(intptr_t)&o->pos.x);
					w->objects["posy"]->SetInt(3,(intptr_t)&o->pos.y);
					w->objects["posz"]->SetInt(3,(intptr_t)&o->pos.z);
					w->objects["rotx"]->SetInt(3,(intptr_t)&o->rot.x);
					w->objects["roty"]->SetInt(3,(intptr_t)&o->rot.y);
					w->objects["rotz"]->SetInt(3,(intptr_t)&o->rot.z);
					w->objects["scalex"]->SetInt(3,(intptr_t)&o->scale.x);
					w->objects["scaley"]->SetInt(3,(intptr_t)&o->scale.y);
					w->objects["scalez"]->SetInt(3,(intptr_t)&o->scale.z);
					w->objects["objectname"]->SetInt(3, (intptr_t)&o->name);
					((cObjectWindow*)w)->undo = new cUndoChangeObject(Graphics.selectedobject);

					Graphics.WM.addwindow(w);
				}
				break;
			case SDLK_r:
				if (Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeObject(Graphics.selectedobject));
					Graphics.world.models[Graphics.selectedobject]->rot = cVector3(0,0,0);
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
