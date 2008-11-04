#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "menu.h"
#include "menucommands.h"
#include "windows/lightwindow.h"
#include "windows/lightoverviewwindow.h"
#include <wm/windowcheckbox.h>
#include <wm/windowinputbox.h>

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern bool	doneaction;
extern float oldmousex, oldmousey;
extern int movement;
extern bool lbuttondown;

extern cMenu* popupmenu;
extern TiXmlDocument favoritelights;

double mouseclickx, mouseclicky, mouseclickz;


void addmenustuff(cMenu* m, TiXmlElement* el)
{
	while(el != NULL)
	{
		if(strcmp(el->Value(), "light") == 0)
		{
			std::string keys;
			TiXmlNode* e = el;
			while(e != NULL)
			{
				TiXmlNode* pel = e->Parent();
				if(pel != NULL)
				{
					TiXmlNode* child = pel->FirstChild();
					for(int i = 0; child != NULL; i++)
					{
						if(child == e)
						{
							char buf[20];
							sprintf(buf, "%i|", i);
							keys += buf;
							break;
						}
						child=child->NextSibling();
					}
				}
				e = e->Parent();
			}

			
			cMenu* mm;
			ADDMENUITEMDATA(mm,m,el->FirstChildElement("name")->FirstChild()->Value(),		&MenuCommand_addfavorite, keys); //new
			
		}
		else
		{
			cMenu* mm;
			ADDMENU(mm,		m, std::string(el->Value()) + ".....",		m->x + 150,m->y); // File
			addmenustuff(mm, el->FirstChildElement());
		}

		el = el->NextSiblingElement();
	}

}


int cProcessManagement::lightedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (cGraphics::world->lights.size() == 0)
					break;
				if(cGraphics::objectStartDrag)
				{
					if(doneaction)
					{
						cGraphics::worldContainer->undoStack->push(new cUndoChangeLight(cGraphics::worldContainer->settings.selectedObject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.x = mouse3dx / 5;
						cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.x = floor(cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.x * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
							cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.z = floor(cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.z * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsety) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsety/(cGraphics::worldContainer->settings.gridSize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.y += (mouseY-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.y = floor(cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].pos.y * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
						cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject].range += (mouseY-oldmousey);
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(cGraphics::world->lights.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < cGraphics::world->lights.size(); i++)
				{
					cVector3 d = cGraphics::world->lights[i].pos;
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
				cGraphics::objectStartDrag = cGraphics::worldContainer->settings.selectedObject == minobj;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{

				if (SDL_GetModState() & KMOD_CTRL)
				{
					cLight l;
					char buf[100];
					sprintf(buf, "Light%i", rand());
					l.name = buf;
					l.color.x = 0;
					l.color.y = 0;
					l.color.z = 0;
					l.pos = cVector3(mouse3dx/5, mouse3dy+10, mouse3dz/5);
					l.todo = std::string(buf, 40);
					l.todo2 = 192;
					l.maxLightIncrement = 256;
					l.range = 200;
					l.lightFalloff = 1;

					cGraphics::worldContainer->settings.selectedObject = cGraphics::world->lights.size();
					cGraphics::world->lights.push_back(l);
					cGraphics::worldContainer->undoStack->push(new cUndoNewLight());
				}
				else
				{
					if (cGraphics::world->lights.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < cGraphics::world->lights.size(); i++)
					{
						cVector3 d = cGraphics::world->lights[i].pos;
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
					cGraphics::worldContainer->settings.selectedObject = minobj;
				}
				cWindow* w = cWM::getWindow(WT_LIGHTOVERVIEW);
				if(w != NULL)
				{
					w->userfunc(NULL);
					cLightOverViewWindow::cLightOverViewTree* tree = (cLightOverViewWindow::cLightOverViewTree*)w->objects["list"];
					tree->getObject(cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject]);
				}

			}
			if(event.button.button == SDL_BUTTON_RIGHT && movement < 3)
			{
				if(movement < 3)
				{
					mouseclickx = mouse3dx;
					mouseclicky = mouse3dy;
					mouseclickz = mouse3dz;

					popupmenu = new cMenu();
					popupmenu->parent = NULL;
					popupmenu->drawStyle = 1;
					popupmenu->x = (int)mouseX;
					popupmenu->y = (int)mouseY;
					popupmenu->w = 150;
					popupmenu->opened = true;
					cMenuItem* mm;
					cMenu* favs;
					ADDMENUITEM(mm,popupmenu,"Deselect light",		&MenuCommand_deselectlight);
					ADDMENUITEM(mm,popupmenu,"Properties",		&MenuCommand_properties);
					ADDMENU(favs,		popupmenu, "Favorites",		popupmenu->x + 150,200);
					favs->y = popupmenu->y;
					favs->x = popupmenu->x + popupmenu->w;
					favs->w = 200;

					ADDMENUITEM(mm,popupmenu,"Disable Shadows",		&MenuCommand_light_disableshadow);
					ADDMENUITEM(mm,popupmenu,"Snap to floor",		&MenuCommand_light_snaptofloor);
					ADDMENUITEM(mm,popupmenu,"Set height",			&MenuCommand_light_setheight);
					ADDMENUITEM(mm,popupmenu,"Set as sunlight",		&MenuCommand_new);


					addmenustuff(favs, favoritelights.FirstChildElement("lights")->FirstChildElement());

				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				if (cGraphics::worldContainer->settings.selectedObject > -1 && cGraphics::worldContainer->settings.selectedObject < (int)cGraphics::world->lights.size())
				{
					cGraphics::worldContainer->undoStack->push(new cUndoLightDelete(cGraphics::worldContainer->settings.selectedObject));
					cGraphics::world->lights.erase(cGraphics::world->lights.begin() + cGraphics::worldContainer->settings.selectedObject);
					cGraphics::worldContainer->settings.selectedObject = -1;
					cWindow* w = cWM::getWindow(WT_LIGHTOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);

				
				}
				break;
			case SDLK_RETURN:
				if (cGraphics::worldContainer->settings.selectedObject != -1)
				{
					if(cGraphics::worldContainer->settings.selectedObject < (int)cGraphics::world->lights.size())
					{
						cLight* l = &cGraphics::world->lights[cGraphics::worldContainer->settings.selectedObject];

						cWindow* w = new cLightWindow();
						((cWindowStringInputBox*)w->objects["name"])->stringetje = &l->name;
						((cWindowFloatInputBox*)w->objects["posx"])->floatje = &l->pos.x;
						((cWindowFloatInputBox*)w->objects["posy"])->floatje = &l->pos.y;
						((cWindowFloatInputBox*)w->objects["posz"])->floatje = &l->pos.z;
						((cWindowFloatInputBox*)w->objects["colorr"])->floatje = &l->color.x;
						((cWindowFloatInputBox*)w->objects["colorg"])->floatje = &l->color.y;
						((cWindowFloatInputBox*)w->objects["colorb"])->floatje = &l->color.z;
						((cWindowFloatInputBox*)w->objects["intensity"])->floatje = &l->todo2;
						((cWindowFloatInputBox*)w->objects["range"])->floatje = &l->range;
						((cWindowFloatInputBox*)w->objects["maxlightincrement"])->floatje = &l->maxLightIncrement;
						((cWindowFloatInputBox*)w->objects["lightfalloff"])->floatje = &l->lightFalloff;
						((cWindowBoolCheckBox*)w->objects["castshadow"])->boolvalue = &l->givesShadow;

						//((cEffectWindow*)w)->undo = new cUndoChangeEffect(cGraphics::worldContainer->settings.selectedObject);
						cWM::addWindow(w);
					}
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
