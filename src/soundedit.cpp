#include <common.h>
#include "graphics.h"
#include "undo.h"
#include <SDL/SDL_mixer.h>
#include <windows/soundoverviewwindow.h>
#include <windows/soundwindow.h>
#include <windows/soundselectwindow.h>
#include <windows/objectwindow.h>

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern float oldmousex, oldmousey;
extern bool lbuttondown;
extern int movement;
extern bool doneaction;
extern cFileSystem fs;
extern std::string rodir;
extern void mainloop();

int cProcessManagement::soundedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.sounds.size() == 0)
					break;
				if(Graphics.objectStartDrag)
				{
					if(doneaction)
					{
						undostack.push(new cUndoChangeSound(Graphics.selectedObject));
						doneaction = false;
					}
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.sounds[Graphics.selectedObject].pos.x = mouse3dx / 5;
						Graphics.world.sounds[Graphics.selectedObject].pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sounds[Graphics.selectedObject].pos.x = floor(Graphics.world.sounds[Graphics.selectedObject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.sounds[Graphics.selectedObject].pos.z = floor(Graphics.world.sounds[Graphics.selectedObject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.sounds[Graphics.selectedObject].pos.y += (mouseY-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sounds[Graphics.selectedObject].pos.y = floor(Graphics.world.sounds[Graphics.selectedObject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
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
				if(Graphics.world.sounds.size() == 0)
					break;
				int minobj = 0;
				float mindist = 999999;
				for(unsigned int i = 0; i < Graphics.world.sounds.size(); i++)
				{
					cVector3 d = Graphics.world.sounds[i].pos;
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
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT && movement < 3)
			{
				if (SDL_GetModState() & KMOD_CTRL)
				{
					cWindow* w = Graphics.WM.getwindow(WT_SOUNDSELECT);
					if(w)
					{
						w->show();
						((cSoundSelectWindow*)w)->newPos = cVector3(mouse3dx, mouse3dy, mouse3dz);
						((cSoundSelectWindow*)w)->selectedSound = NULL;
					}
					else
						Graphics.WM.addwindow(new cSoundSelectWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin, cVector3(mouse3dx, mouse3dy, mouse3dz)));
					

				}
				else
				{
					if (Graphics.world.sounds.size() == 0)
						break;
					int minobj = 0;
					float mindist = 999999;
					for(unsigned int i = 0; i < Graphics.world.sounds.size(); i++)
					{
						cVector3 d = Graphics.world.sounds[i].pos;
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

					cWindow* w = Graphics.WM.getwindow(WT_SOUNDOVERVIEW);
					if(w != NULL)
					{
						w->userfunc(NULL);
						cSoundOverViewWindow::cSoundOverViewTree* tree = (cSoundOverViewWindow::cSoundOverViewTree*)w->objects["list"];
						tree->getObject(Graphics.world.sounds[Graphics.selectedObject]);
					}
				
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				if (Graphics.selectedObject != -1)
				{
					cSound* o = &Graphics.world.sounds[Graphics.selectedObject];

					cWindow* w = new cSoundWindow(Graphics.WM.texture, Graphics.WM.font, Graphics.WM.skin);

					((cWindowStringInputBox*)w->objects["objectname"])->stringetje = &o->name;


					((cWindowFloatInputBox*)w->objects["posx"])->floatje = &o->pos.x;
					((cWindowFloatInputBox*)w->objects["posy"])->floatje = &o->pos.y;
					((cWindowFloatInputBox*)w->objects["posz"])->floatje = &o->pos.z;
					((cWindowFloatInputBox*)w->objects["rotx"])->floatje = &o->rotation.x;
					((cWindowFloatInputBox*)w->objects["roty"])->floatje = &o->rotation.y;
					((cWindowFloatInputBox*)w->objects["rotz"])->floatje = &o->rotation.z;
					((cWindowFloatInputBox*)w->objects["scalex"])->floatje = &o->scale.x;
					((cWindowFloatInputBox*)w->objects["scaley"])->floatje = &o->scale.y;
					((cWindowFloatInputBox*)w->objects["scalez"])->floatje = &o->scale.z;
					((cWindowStringInputBox*)w->objects["objectname"])->stringetje = &o->name;
					((cWindowStringInputBox*)w->objects["objectfile"])->stringetje = &o->fileName;
					((cWindowFloatInputBox*)w->objects["repeatDelay"])->floatje = &o->repeatDelay;
					((cWindowFloatInputBox*)w->objects["unknown2"])->floatje = &o->unknown2;
					((cWindowLongInputBox*)w->objects["unknown3"])->longje = &o->unknown3;
					((cWindowLongInputBox*)w->objects["unknown4"])->longje = &o->unknown4;
					((cWindowFloatInputBox*)w->objects["unknown5"])->floatje = &o->unknown5;
//					w->objects["unknown6"]->SetText(0, string(o->unknown6,8));
//					w->objects["unknown6"]->SetInt(0,0);

					((cSoundWindow::cWindowSelectFileButton*)w->objects["filebutton"])->selectedSound = o;

					((cWindowFloatInputBox*)w->objects["unknown7"])->floatje = &o->unknown7;
					((cWindowFloatInputBox*)w->objects["unknown8"])->floatje = &o->unknown8;
					
					((cObjectWindow*)w)->undo = new cUndoChangeSound(Graphics.selectedObject);

					Graphics.WM.addwindow(w);
				}
				break;
			case SDLK_SPACE:
				if (Graphics.selectedObject != -1)
				{
					if(Graphics.world.sounds.size() >= Graphics.selectedObject)
						break;
					static bool playing = false;

					if(!playing)
					{
						playing = true;

						cSound* o = &Graphics.world.sounds[Graphics.selectedObject];
						Mix_Chunk *sample;
						cFile* pFile = fs.open(rodir+"data/wav/" + o->fileName);
						sample=Mix_QuickLoad_WAV((BYTE*)pFile->data);
						Mix_Volume(-1,MIX_MAX_VOLUME);
						Mix_PlayChannel(0, sample, 0);
						while(Mix_Playing(-1) > 0 && playing)
						{
							mainloop();
						}
						if(!playing)
							Mix_HaltChannel(-1);
						Mix_FreeChunk(sample);
						pFile->close();
					}
					playing = false;

					break;
				}
			case SDLK_BACKSPACE:
				if (Graphics.selectedObject > -1 && Graphics.selectedObject < (int)Graphics.world.sounds.size())
				{
					undostack.push(new cUndoSoundDelete(Graphics.selectedObject));
					Graphics.world.sounds.erase(Graphics.world.sounds.begin() + Graphics.selectedObject);
					Graphics.selectedObject = -1;
					cWindow* w = Graphics.WM.getwindow(WT_SOUNDOVERVIEW);
					if(w != NULL)
						w->userfunc(NULL);
				
				}

			
			default:
				break;
			}

			break;
		}
			
	}



	return true;


}
