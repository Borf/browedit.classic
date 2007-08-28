#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include "wm/effectwindow.h"

extern float mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern float oldmousex, oldmousey;
extern int movement;
extern cMenuItem* selectedeffect;
extern cMenu* effectsmenu;
extern bool lbuttondown;

cProcessManagement::effectedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.effects.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				if(Graphics.objectstartdrag)
				{
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.effects[Graphics.selectedobject].pos.x = mouse3dx / 5;
						Graphics.world.effects[Graphics.selectedobject].pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.effects[Graphics.selectedobject].pos.x = floor(Graphics.world.effects[Graphics.selectedobject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.effects[Graphics.selectedobject].pos.z = floor(Graphics.world.effects[Graphics.selectedobject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.effects[Graphics.selectedobject].pos.y += (mousey-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.effects[Graphics.selectedobject].pos.y = floor(Graphics.world.effects[Graphics.selectedobject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(Graphics.world.effects.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(int i = 0; i < Graphics.world.effects.size(); i++)
				{
					cVector3 d = Graphics.world.effects[i].pos;
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
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
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
					e.pos = cVector3(mouse3dx/5, mouse3dy+10, mouse3dz/5);
					e.name = buf;
					e.readablename = selectedeffect->title;
					e.type = atoi(selectedeffect->data.c_str());
					e.todo1 = 1;
					e.todo2 = 1;
					e.todo3 = 1;
					e.rotation = cVector3(0,0,0);
					e.scale = cVector3(1,1,1);
					e.todo10 = 1; // seems to be linked to 11
					e.todo11 = 1; // seems to be linked to 10
					e.todo12 = 0; // seems to be always 0
					e.todo13 = 0; // seems to be always 0

					Graphics.world.effects.push_back(e);
				}
				else
				{
					if (Graphics.world.effects.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(int i = 0; i < Graphics.world.effects.size(); i++)
					{
						cVector3 d = Graphics.world.effects[i].pos;
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
					char buf[100];
					sprintf(buf, "%i", Graphics.world.effects[Graphics.selectedobject].type);
					cMenu* m = effectsmenu->finddata(buf);
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_PAGEDOWN:
				if (Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeEffect(Graphics.selectedobject));
					Graphics.world.effects[Graphics.selectedobject].loop--;
				}
				break;
			case SDLK_PAGEUP:
				if (Graphics.selectedobject != -1)
				{
					undostack.push(new cUndoChangeEffect(Graphics.selectedobject));
					Graphics.world.effects[Graphics.selectedobject].loop++;
				}
				break;
			case SDLK_BACKSPACE:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.effects.size())
				{
					undostack.push(new cUndoEffectDelete(Graphics.selectedobject));
					Graphics.world.effects.erase(Graphics.world.effects.begin() + Graphics.selectedobject);
					Graphics.selectedobject = -1;
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedobject != -1)
				{
					cEffect* o = &Graphics.world.effects[Graphics.selectedobject];

					cWindow* w = new cEffectWindow(&Graphics.WM.texture, &Graphics.WM.font);
					w->objects["posx"]->SetInt(3,(int)&o->pos.x);
					w->objects["posy"]->SetInt(3,(int)&o->pos.y);
					w->objects["posz"]->SetInt(3,(int)&o->pos.z);
					w->objects["rotx"]->SetInt(3,(int)&o->rotation.x);
					w->objects["roty"]->SetInt(3,(int)&o->rotation.y);
					w->objects["rotz"]->SetInt(3,(int)&o->rotation.z);
					w->objects["scalex"]->SetInt(3,(int)&o->scale.x);
					w->objects["scaley"]->SetInt(3,(int)&o->scale.y);
					w->objects["scalez"]->SetInt(3,(int)&o->scale.z);
					w->objects["looptime"]->SetInt(3,(int)&o->loop);
					w->objects["objectname"]->SetText(0, o->readablename);
					((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedobject);
					Graphics.WM.addwindow(w);
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