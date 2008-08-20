#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "sprite.h"
#include "wm/spritewindow.h"

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern bool lbuttondown, doneaction;
extern float oldmousey,oldmousex;
extern int movement;
extern string rodir;


int cProcessManagement::spriteedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.sprites.size() == 0)
					break;
				if(Graphics.objectStartDrag)
				{
					if(doneaction)
					{
						//undostack.push(new cUndoChangeSprite(Graphics.selectedObject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.sprites[Graphics.selectedObject]->pos.x = mouse3dx / 5;
						Graphics.world.sprites[Graphics.selectedObject]->pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sprites[Graphics.selectedObject]->pos.x = floor(Graphics.world.sprites[Graphics.selectedObject]->pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.sprites[Graphics.selectedObject]->pos.z = floor(Graphics.world.sprites[Graphics.selectedObject]->pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.sprites[Graphics.selectedObject]->pos.y += (mousey-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sprites[Graphics.selectedObject]->pos.y = floor(Graphics.world.sprites[Graphics.selectedObject]->pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
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
				for(unsigned int i = 0; i < Graphics.world.sprites.size(); i++)
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
				Graphics.objectStartDrag = Graphics.selectedObject == minobj;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{

				if (SDL_GetModState() & KMOD_CTRL)
				{
					Log(3,0,"Sprite click");
					cSprite* sprite = new cSprite();
					string sexes[] = { "여", "남" };
					string bodies[] = { "검사","마법사","궁수","성직자","상인","도둑","기사","프리스트","위저드","제철공","헌터","어세신","페코페코_기사","크루세이더","몽크","세이지","로그","연금술사","신페코크루세이더","결혼","슈퍼노비스","건너","닌자","산타","검사","마법사","궁수","성직자","상인","도둑","로드나이트","하이프리","하이위저드","화이트스미스","스나이퍼","어쌔신크로스","로드페코","팔라딘","챔피온","프로페서","스토커","크리에이터","클라운","집시","페코팔라딘","검사","마법사","궁수","성직자","상인","도둑","기사","프리스트","위저드","제철공","헌터","어세신","페코페코_기사","크루세이더","몽크","세이지","로그","연금술사","구페코크루세이더","슈퍼노비스","태권소년","권성","권성융합","소울링커","성직자","기사","세이지","초보자" };

					int sex = rand() % 2;
					int bodyid = rand() % (sizeof(bodies)/sizeof(string));

					sprite->loadBody(rodir + "data\\sprite\\인간족\\몸통\\" + sexes[sex] + "\\" + bodies[bodyid] + "_" + sexes[sex]);
					int headid = 1+ rand() % 22;
					char buf[20];
					sprintf(buf, "%i", headid);
					sprite->loadHead(rodir + "data\\sprite\\인간족\\머리통\\" + sexes[sex] + "\\" + buf + "_" + sexes[sex]);
					
					sprite->pos = cVector3(mouse3dx/5, mouse3dy, mouse3dz/5);
					Graphics.world.sprites.push_back(sprite);

					Log(3,0,"Sprite click");
					//undostack.push(new cUndoNewSprite());
				}
				else
				{
					if (Graphics.world.sprites.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < Graphics.world.sprites.size(); i++)
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
					Graphics.selectedObject = minobj;
				}
			}
			else if(event.button.button == SDL_BUTTON_RIGHT && movement < 3)
			{
				Graphics.selectedObject = -1;
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)Graphics.world.sprites.size())
				{
					//undostack.push(new cUndoSpriteDelete(Graphics.selectedObject));
					delete Graphics.world.sprites[Graphics.selectedObject];
					Graphics.world.sprites.erase(Graphics.world.sprites.begin() + Graphics.selectedObject);
					Graphics.selectedObject = -1;
				}
				break;
			case SDLK_RETURN:
				if (Graphics.selectedObject != -1 && Graphics.WM.getwindow(WT_SPRITE) == NULL)
				{
					if(Graphics.selectedObject >= (int)Graphics.world.sprites.size())
						break;

					cWindow* w = new cSpriteWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin);

					if(Graphics.world.sprites[Graphics.selectedObject]->head)
						((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadHead(Graphics.world.sprites[Graphics.selectedObject]->head->fileName);
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadBody(Graphics.world.sprites[Graphics.selectedObject]->body->fileName);

					for(unsigned int i = 0; i < Graphics.world.sprites[Graphics.selectedObject]->extras.size(); i++)
					{
						if(Graphics.world.sprites[Graphics.selectedObject]->extras[i] != NULL)
						{
							((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->setExtra(i, Graphics.world.sprites[Graphics.selectedObject]->extras[i]->fileName);
						}
					}
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->action = Graphics.world.sprites[Graphics.selectedObject]->action-1;
					w->objects["actionbutton"]->click();
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->direction = Graphics.world.sprites[Graphics.selectedObject]->direction;

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
					//((cEffectWindow*)w)->undo = new cUndoChangeEffect(Graphics.selectedObject);
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

