#include <common.h>
#include "graphics.h"
#include "undo.h"
#include <wm/soundwindow.h>
#include <wm/soundselectwindow.h>

extern long mousestartx, mousestarty;
extern double mouse3dx, mouse3dy, mouse3dz;
extern cGraphics Graphics;
extern cUndoStack undostack;
extern float oldmousex, oldmousey;
extern bool lbuttondown;
extern int movement;

int cProcessManagement::soundedit_process_events(SDL_Event &event)
{
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(lbuttondown)
			{
				if (Graphics.world.sounds.size() == 0)
					break;
				if(Graphics.objectstartdrag)
				{
					bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
					bool alt = (SDL_GetModState() & KMOD_ALT) != 0;
					if (!ctrl && !alt)
					{
						Graphics.world.sounds[Graphics.selectedobject].pos.x = mouse3dx / 5;
						Graphics.world.sounds[Graphics.selectedobject].pos.z = mouse3dz / 5;
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sounds[Graphics.selectedobject].pos.x = floor(Graphics.world.sounds[Graphics.selectedobject].pos.x * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
							Graphics.world.sounds[Graphics.selectedobject].pos.z = floor(Graphics.world.sounds[Graphics.selectedobject].pos.z * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsety) / (Graphics.gridsize/2.0f) + Graphics.gridoffsety/(Graphics.gridsize/2.0f);
						}
					}
					if(ctrl && !alt)
					{
						Graphics.world.sounds[Graphics.selectedobject].pos.y += (mousey-oldmousey);
						if (SDL_GetModState() & KMOD_SHIFT)
						{
							Graphics.world.sounds[Graphics.selectedobject].pos.y = floor(Graphics.world.sounds[Graphics.selectedobject].pos.y * (Graphics.gridsize/2.0f) + 0.5-Graphics.gridoffsetx) / (Graphics.gridsize/2.0f) + Graphics.gridoffsetx/(Graphics.gridsize/2.0f);
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
				Graphics.objectstartdrag = Graphics.selectedobject == minobj;
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
					}
					else
						Graphics.WM.addwindow(new cSoundSelectWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin, cVector3(mouse3dx, mouse3dy, mouse3dz)));
					

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
					Graphics.selectedobject = minobj;
				}
			}
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				if (Graphics.selectedobject != -1)
				{
					cSound* o = &Graphics.world.sounds[Graphics.selectedobject];

					cWindow* w = new cSoundWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin);

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
					((cWindowStringInputBox*)w->objects["objectfile"])->stringetje = &o->filename;
					((cWindowFloatInputBox*)w->objects["unknown1"])->floatje = &o->unknown1;
					((cWindowFloatInputBox*)w->objects["unknown2"])->floatje = &o->unknown2;
					((cWindowFloatInputBox*)w->objects["unknown3"])->floatje = &o->unknown2;
					((cWindowFloatInputBox*)w->objects["unknown4"])->floatje = &o->unknown4;
					((cWindowFloatInputBox*)w->objects["unknown5"])->floatje = &o->unknown5;
					w->objects["unknown6"]->SetText(0, o->unknown6);
					w->objects["unknown6"]->SetInt(0,0);

					((cWindowFloatInputBox*)w->objects["unknown7"])->floatje = &o->unknown7;
					((cWindowFloatInputBox*)w->objects["unknown8"])->floatje = &o->unknown8;
					
//					((cObjectWindow*)w)->undo = new cUndoChangeObject(Graphics.selectedobject);

					Graphics.WM.addwindow(w);
				}
				break;
			case SDLK_SPACE:
				if (Graphics.selectedobject != -1)
				{
					static bool playing = false;

					if(!playing)
					{
						playing = true;

						cSound* o = &Graphics.world.sounds[Graphics.selectedobject];
						Mix_Chunk *sample;
						cFile* pFile = fs.open(rodir+"data/wav/" + o->filename);
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

			
			default:
				break;
			}

			break;
		}
			
	}



	return true;


}
