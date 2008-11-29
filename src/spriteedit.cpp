#include <common.h>
#include "graphics.h"
#include "undo.h"
#include "sprite.h"
#include "windows/spritewindow.h"
#include "settings.h"

extern bool doneAction;


int cProcessManagement::spriteedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(cGraphics::cMouse::lbuttondown)
			{
				if (cGraphics::world->sprites.size() == 0)
					break;
				if(cGraphics::objectStartDrag)
				{
					if(doneAction)
					{
						//cGraphics::worldContainer->undostack->push(new cUndoChangeSprite(cGraphics::worldContainer->settings.selectedObject));
						doneAction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.x = cGraphics::cMouse::x3d / 5;
						cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.z = cGraphics::cMouse::z3d / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.x = floor(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.x * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
							cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.z = floor(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.z * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsety) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsety/(cGraphics::worldContainer->settings.gridSize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.y += (cGraphics::cMouse::y-cGraphics::cMouse::yOld);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.y = floor(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->pos.y * (cGraphics::worldContainer->settings.gridSize/2.0f) + 0.5-cGraphics::worldContainer->settings.gridoffsetx) / (cGraphics::worldContainer->settings.gridSize/2.0f) + cGraphics::worldContainer->settings.gridoffsetx/(cGraphics::worldContainer->settings.gridSize/2.0f);
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
				if(cGraphics::world->sprites.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < cGraphics::world->sprites.size(); i++)
				{
					cVector3 d = cGraphics::world->sprites[i]->pos;
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
		}
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && cWM::movement < 3)
			{

				if (SDL_GetModState() & KMOD_CTRL)
				{
					Log(3,0,"Sprite click");
					cSprite* sprite = new cSprite();
					std::string sexes[] = { "여", "남" };
					std::string bodies[] = { "검사","마법사","궁수","성직자","상인","도둑","기사","프리스트","위저드","제철공","헌터","어세신","페코페코_기사","크루세이더","몽크","세이지","로그","연금술사","신페코크루세이더","결혼","슈퍼노비스","건너","닌자","산타","검사","마법사","궁수","성직자","상인","도둑","로드나이트","하이프리","하이위저드","화이트스미스","스나이퍼","어쌔신크로스","로드페코","팔라딘","챔피온","프로페서","스토커","크리에이터","클라운","집시","페코팔라딘","검사","마법사","궁수","성직자","상인","도둑","기사","프리스트","위저드","제철공","헌터","어세신","페코페코_기사","크루세이더","몽크","세이지","로그","연금술사","구페코크루세이더","슈퍼노비스","태권소년","권성","권성융합","소울링커","성직자","기사","세이지","초보자" };

					int sex = rand() % 2;
					int bodyid = rand() % (sizeof(bodies)/sizeof(std::string));

					sprite->loadBody(cSettings::roDir + "data\\sprite\\인간족\\몸통\\" + sexes[sex] + "\\" + bodies[bodyid] + "_" + sexes[sex]);
					int headid = 1+ rand() % 22;
					char buf[20];
					sprintf(buf, "%i", headid);
					sprite->loadHead(cSettings::roDir + "data\\sprite\\인간족\\머리통\\" + sexes[sex] + "\\" + buf + "_" + sexes[sex]);
					
					sprite->pos = cVector3(cGraphics::cMouse::x3d/5, cGraphics::cMouse::y3d, cGraphics::cMouse::z3d/5);
					cGraphics::world->sprites.push_back(sprite);

					Log(3,0,"Sprite click");
					//cGraphics::worldContainer->undostack->push(new cUndoNewSprite());
				}
				else
				{
					if (cGraphics::world->sprites.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < cGraphics::world->sprites.size(); i++)
					{
						cVector3 d = cGraphics::world->sprites[i]->pos;
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
					cGraphics::worldContainer->settings.selectedObject = minobj;
				}
			}
			else if(event.button.button == SDL_BUTTON_RIGHT && cWM::movement < 3)
			{
				cGraphics::worldContainer->settings.selectedObject = -1;
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				if (cGraphics::worldContainer->settings.selectedObject > -1 && cGraphics::worldContainer->settings.selectedObject < (int)cGraphics::world->sprites.size())
				{
					//cGraphics::worldContainer->undostack->push(new cUndoSpriteDelete(cGraphics::worldContainer->settings.selectedObject));
					delete cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject];
					cGraphics::world->sprites.erase(cGraphics::world->sprites.begin() + cGraphics::worldContainer->settings.selectedObject);
					cGraphics::worldContainer->settings.selectedObject = -1;
				}
				break;
			case SDLK_RETURN:
				if (cGraphics::worldContainer->settings.selectedObject != -1 && cWM::getWindow(WT_SPRITE) == NULL)
				{
					if(cGraphics::worldContainer->settings.selectedObject >= (int)cGraphics::world->sprites.size())
						break;

					cWindow* w = new cSpriteWindow();

					if(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->head)
						((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadHead(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->head->fileName);
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->loadBody(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->body->fileName);

					for(unsigned int i = 0; i < cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras.size(); i++)
					{
						if(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras[i] != NULL)
						{
							((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->setExtra(i, cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras[i]->fileName);
						}
					}
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->action = cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->action-1;
					w->objects["actionbutton"]->onClick();
					((cSpriteWindow::cWindowSprite*)w->objects["spritewindow"])->sprite->direction = cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->direction;

					((cWindowTabPanel*)w->objects["tabpanel"])->tabchange(-1);

					cWM::addWindow(w);

/*					cWindow* w = new cLightWindow(&cWM::texture, &cWM::font);
					w->objects["posx"]->SetInt(3,(int)&l->pos.x);
					w->objects["posy"]->SetInt(3,(int)&l->pos.y);
					w->objects["posz"]->SetInt(3,(int)&l->pos.z);
					w->objects["colorr"]->SetInt(3,(int)&l->color.x);
					w->objects["colorg"]->SetInt(3,(int)&l->color.y);
					w->objects["colorb"]->SetInt(3,(int)&l->color.z);
					w->objects["intensity"]->SetInt(3,(int)&l->todo2);
					//((cEffectWindow*)w)->undo = new cUndoChangeEffect(cGraphics::worldContainer->settings.selectedObject);
					cWM::addWindow(w);*/
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

