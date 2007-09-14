#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "sprite.h"
#include "wm/spritewindow.h"

extern float mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown, doneaction;
extern float oldmousey,oldmousex;
extern int movement;
extern string rodir;


cProcessManagement::spriteedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.sprites.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				if(Graphics.objectstartdrag)
				{
					if(doneaction)
					{
						//undostack.push(new cUndoChangeSprite(Graphics.selectedobject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.sprites[Graphics.selectedobject]->pos.x = mouse3dx / 5;
						Graphics.world.sprites[Graphics.selectedobject]->pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sprites[Graphics.selectedobject]->pos.x = floor(Graphics.world.sprites[Graphics.selectedobject]->pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.sprites[Graphics.selectedobject]->pos.z = floor(Graphics.world.sprites[Graphics.selectedobject]->pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.sprites[Graphics.selectedobject]->pos.y += (mousey-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sprites[Graphics.selectedobject]->pos.y = floor(Graphics.world.sprites[Graphics.selectedobject]->pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
						}
					}
					if(!ctrl && alt)
					{
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				if(Graphics.world.sprites.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(int i = 0; i < Graphics.world.sprites.size(); i++)
				{
					cVector3 d = Graphics.world.sprites[i]->pos;
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
		}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{

				if (SDL_GetModState() & KMOD_CTRL)
				{
					cSprite* sprite = new cSprite();
					string sexes[] = { "��", "��" };
					string bodies[] = { "�˻�","������","�ü�","������","����","����","���","������Ʈ","������","��ö��","����","���","��������_���","ũ�缼�̴�","��ũ","������","�α�","���ݼ���","������ũ�缼�̴�","��ȥ","���۳��","�ǳ�","����","��Ÿ","�˻�","������","�ü�","������","����","����","�ε峪��Ʈ","��������","����������","ȭ��Ʈ���̽�","��������","��ؽ�ũ�ν�","�ε�����","�ȶ��","è�ǿ�","�����伭","����Ŀ","ũ��������","Ŭ���","����","�����ȶ��","�˻�","������","�ü�","������","����","����","���","������Ʈ","������","��ö��","����","���","��������_���","ũ�缼�̴�","��ũ","������","�α�","���ݼ���","������ũ�缼�̴�","���۳��","�±Ǽҳ�","�Ǽ�","�Ǽ�����","�ҿ︵Ŀ","������","���","������","�ʺ���" };

					int sex = rand() % 2;
					int bodyid = rand() % (sizeof(bodies)/sizeof(string));
					sprite->loadbody(rodir + "data\\sprite\\�ΰ���\\����\\" + sexes[sex] + "\\" + bodies[bodyid] + "_" + sexes[sex]);
					int headid = 1+ rand() % 22;
					char buf[20];
					sprintf(buf, "%i", headid);
					sprite->loadhead(rodir + "data\\sprite\\�ΰ���\\�Ӹ���\\" + sexes[sex] + "\\" + buf + "_" + sexes[sex]);
					
					sprite->pos = cVector3(mouse3dx/5, mouse3dy, mouse3dz/5);
					Graphics.world.sprites.push_back(sprite);

					//undostack.push(new cUndoNewSprite());
				}
				else
				{
					if (Graphics.world.sprites.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(int i = 0; i < Graphics.world.sprites.size(); i++)
					{
						cVector3 d = Graphics.world.sprites[i]->pos;
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
			}
			else if(event.button.button == SDL_BUTTON_RIGHT && movement < 3)
			{
				Graphics.selectedobject = -1;
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				if (Graphics.selectedobject > -1 && Graphics.selectedobject < Graphics.world.sprites.size())
				{
					//undostack.push(new cUndoSpriteDelete(Graphics.selectedobject));
					Graphics.world.sprites.erase(Graphics.world.sprites.begin() + Graphics.selectedobject);
					Graphics.selectedobject = -1;
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedobject != -1 && Graphics.WM.getwindow(WT_SPRITE) == NULL)
				{
					if(Graphics.selectedobject >= Graphics.world.sprites.size())
						break;
					cSprite* l = Graphics.world.sprites[Graphics.selectedobject];

					cWindow* w = new cSpriteWindow(&Graphics.WM.texture, &Graphics.WM.font);

					if(Graphics.world.sprites[Graphics.selectedobject]->head)
						((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadhead(Graphics.world.sprites[Graphics.selectedobject]->head->filename);
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadbody(Graphics.world.sprites[Graphics.selectedobject]->body->filename);

					for(int i = 0; i < Graphics.world.sprites[Graphics.selectedobject]->extras.size(); i++)
					{
						if(Graphics.world.sprites[Graphics.selectedobject]->extras[i] != NULL)
						{
							((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->setextra(i, Graphics.world.sprites[Graphics.selectedobject]->extras[i]->filename);
						}
					}
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->action = Graphics.world.sprites[Graphics.selectedobject]->action-1;
					w->objects["actionbutton"]->click();
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->direction = Graphics.world.sprites[Graphics.selectedobject]->direction;

					((cWindowTabPanel*)w->objects["tabpanel"])->tabchange(-1);

					Graphics.WM.addwindow(w);

/*					cWindow* w = new cLightWindow(&Graphics.WM.texture, &Graphics.WM.font);
					w->objects["posx"]->SetInt(3,(int)&l->pos.x);
					w->objects["posy"]->SetInt(3,(int)&l->pos.y);
					w->objects["posz"]->SetInt(3,(int)&l->pos.z);
					w->objects["colorr"]->SetInt(3,(int)&l->color.x);
					w->objects["colorg"]->SetInt(3,(int)&l->color.y);
					w->objects["colorb"]->SetInt(3,(int)&l->color.z);
					w->objects["intensity"]->SetInt(3,(int)&l->todo2);
					//((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedobject);
					Graphics.WM.addwindow(w);*/
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