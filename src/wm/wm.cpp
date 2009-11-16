#include "wm.h"
#include "window.h"

#include <common.h>
#include <font.h>
#include <texture.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <graphics.h>
#include <windows/messagewindow.h>
#include <windows/confirmwindow.h>
#include <windows/inputwindow.h>
#include <windows/xmlwindow.h>
#include <filesystem.h>
#include <texturecache.h>
extern void mainloop();




std::vector<cWindow*>	cWM::windows; //vector of windows, 0 = topwindow
cTexture*				cWM::texture;
cFont*					cWM::font;
TiXmlDocument			cWM::skin;
float					cWM::color[4];
float					cWM::colorBlur[4];
int						cWM::focus;
cWindow*				cWM::draggingWindow = NULL;
cWindowObject*			cWM::draggingObject = NULL;



int cWM::draw()
{
	unsigned int i;
	int ii;
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//glTranslatef(0,0,9000);
	cleanWindows();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glColor4fv(colorBlur);
	glEnable(GL_BLEND);
	int size = windows.size()-1;
	int topwindow = 0;
	for(i = 0; i < windows.size(); i++)
	{
		cWindow* w = windows[i];
		if (w->isEnabled())
		{
			if(w->isVisible() && w->isEnabled())
			{
				topwindow = i;
				break;
			}
		}
	}
	

	for(ii = size; ii >= 0; ii--)
	{
		cWindow* w = windows[ii];
		if(w->isAlwaysOnTop())
			continue;
		if (w->isEnabled())
		{
			if (ii == topwindow)
			{
				glColor4fv(color);
				w->istopwindow(true);
			}
			else
			{
				if(w->canBeResized())
					glColor4fv(colorBlur);
				else
					glColor4fv(color);
				w->istopwindow(false);
			}
			if(w->isVisible() && w->isEnabled())
				w->draw();
		}
		if (windows[ii] != w)
			break;	
	}
	for(ii = size; ii >= 0; ii--)
	{
		cWindow* w = windows[ii];
		if(!w->isAlwaysOnTop())
			continue;
		if (w->isEnabled())
		{
			if (ii == topwindow)
			{
				glColor4fv(color);
				w->istopwindow(true);
			}
			else
			{
				if(w->canBeResized())
					glColor4fv(colorBlur);
				else
					glColor4fv(color);
				w->istopwindow(false);
			}
			if(w->isVisible() && w->isEnabled())
				w->draw();
		}
		if (windows[ii] != w)
			break;	
	}
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	//glTranslatef(0,0,-9000);
	return 1;
}


int cWM::init(std::string sSkin)
{
	skin = cFileSystem::getXml(sSkin);
	texture = cTextureCache::load(skin.FirstChildElement("skin")->FirstChildElement("texture")->FirstChild()->Value());
	font = new cFont();
	font->load(skin.FirstChildElement("skin")->FirstChildElement("font")->FirstChild()->Value());

	std::string c = skin.FirstChildElement("skin")->FirstChildElement("color")->FirstChild()->Value();
	color[0] = hex2dec(c.substr(0,2))/255.0f;
	color[1] = hex2dec(c.substr(2,2))/255.0f;
	color[2] = hex2dec(c.substr(4,2))/255.0f;
	color[3] = hex2dec(c.substr(6,2))/255.0f;

	c = skin.FirstChildElement("skin")->FirstChildElement("colorblurred")->FirstChild()->Value();
	colorBlur[0] = hex2dec(c.substr(0,2))/255.0f;
	colorBlur[1] = hex2dec(c.substr(2,2))/255.0f;
	colorBlur[2] = hex2dec(c.substr(4,2))/255.0f;
	colorBlur[3] = hex2dec(c.substr(6,2))/255.0f;

	focus = 0;
	Log(3,0,"Window Manager initialized");
	return 1;
}

void cWM::save()
{

	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->saveWindow)
			windows[i]->save();
	}

//////////////////////////////////////////////////////////////////////////////////////////////
}

void cWM::unLoad()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		windows[i]->save();
		delete(windows[i]);
	}
	windows.clear();
	if(texture)	
	{
		cTextureCache::unload(texture);
		texture = NULL;
	}
	if(font)
	{
		delete font;
		font = NULL;
	}
}

cWM::~cWM()
{
	unLoad();
}

void cWM::click(bool b)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->inWindow() && (!windows[0]->modality() || i == 0))
		{
			//move windows[i] to windows[0].
			cWindow* w = windows[i];
			if (i != 0)
			{
				for(int ii = i-1; ii >= 0; ii--)
				{
					windows[ii]->istopwindow(false);
					windows[ii+1] = windows[ii];
				}
				windows[0] = w;
				w->istopwindow(true);
			}
			if (b)
				w->onClick();
			break;
		}
	}

}

cWindow* cWM::inWindow()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->isEnabled() && windows[i]->inWindow() && (i == 0 || !windows[0]->modality()))
			return windows[i];
	}
	return NULL;
}


void cWM::drag(cWindow* w)
{//makes sure window w snaps to other windows....
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->isEnabled() && windows[i] != w)
		{
			if ((w->getX() <= windows[i]->getX() && windows[i]->getX() <= w->getX()+w->getWidth()) ||
				(windows[i]->getX() <= w->getX() && w->getX() <= windows[i]->getX()+windows[i]->getWidth()))
			{
				if (abs(w->getY() - (windows[i]->getY()+windows[i]->getHeight())) < SNAPPINGDIST)
				{
					if (w->isResizing())
					{
						w->resizeTo(w->getWidth(), w->getHeight()+(w->getY()-(windows[i]->getY()+windows[i]->getHeight())));
						w->moveTo(w->getX(), windows[i]->getY() + windows[i]->getHeight());
					}
					else
						w->moveTo(w->getX(), windows[i]->getY()+windows[i]->getHeight()-w->ph2()+w->getHeight());
				}
				if (abs((w->getY()+w->getHeight()) - windows[i]->getY()) < SNAPPINGDIST)
				{
					if (w->isResizing())
						w->resizeTo(w->getWidth(), windows[i]->getY()-w->getY());
					else
						w->moveTo(w->getX(), windows[i]->getY()-w->ph2());
				}
				
			}
			if ((w->getY() <= windows[i]->getY() && windows[i]->getY() <= w->getY()+w->getHeight()) ||
				(windows[i]->getY() <= w->getY() && w->getY() <= windows[i]->getY()+windows[i]->getHeight()))
			{
				if (abs(w->getX() - (windows[i]->getX()+windows[i]->getWidth())) < SNAPPINGDIST)
				{
					if (w->isResizing())
					{
						w->resizeTo(w->getWidth()+(w->getX()-(windows[i]->getX()+windows[i]->getWidth())), w->getHeight());
						w->moveTo(windows[i]->getX() + windows[i]->getWidth(), w->getY());
					}
					else
						w->moveTo(windows[i]->getX()+windows[i]->getWidth(), w->getY());
				}
				if (abs((w->getX()+w->getWidth()) - windows[i]->getX()) < SNAPPINGDIST)
				{
					if (w->isResizing())
						w->resizeTo(windows[i]->getX()-w->getX(), w->getHeight());
					else
						w->moveTo(windows[i]->getX()-w->getWidth(), w->getY());
				}

			}

		}

	}

}

void cWM::showWindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->getWindowType() == wt)
			windows[i]->show();
	}
}

bool cWM::existsWindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->getWindowType() == wt)
			return true;
	}
	return false;
}


void cWM::toggleWindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->getWindowType() == wt)
		{
			windows[i]->togglevis();
			if (windows[i]->isVisible())
			{
				cWindow* w = windows[i];
				for(int ii = i; ii > 0; ii--)
					windows[ii] = windows[ii-1];
				windows[0] = w;
				break;
			}
		}
	}
}

int cWM::closeWindow(WINDOW_TYPE wt, bool force)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		cWindow* w = windows[i];
		if (w != NULL)

			if (w->isEnabled() && w->getWindowType() == wt)
				w->close(force);
	}
	return 0;

}

bool cWM::onChar(char c, bool shift)
{
	if (windows.size() > 0)
		if (windows[0]->isEnabled() && windows[0]->isVisible())
			return windows[0]->onChar(c, shift);
	return false;
}

bool cWM::onKeyDown(int key, bool shift)
{
	if(windows.size() == 0)
		return false;

/*	if (key == SDLK_RETURN && windows[0]->isEnabled() && windows[0]->defaultobject == "")
	{
		cWindow* w = getWindow(WT_CHAT);
		if (w != NULL)
		{
			int i;
			for(i = 0; i < (int)windows.size(); i++)
				if (windows[i]->getWindowType() == WT_CHAT) break;
			

			

			if (windows[0]->getWindowType() == WT_CHAT && w->selectedobject != w->objects["inputbox"])
			{
				w->selectedobject = w->objects["inputbox"];
				// selects
				w->selectedobject->SetInt(1, w->selectedobject->GetText(0).length());
				w->selectedobject->SetInt(2, 0);
				return true;
			}
			else if(windows[0]->defaultobject == "" && i != 0)
			{
				for(int ii = i; ii > 0; ii--)
				{
					windows[ii]->istopwindow(false);
					windows[ii] = windows[ii-1];
				}
				windows[0]->istopwindow(false);
				windows[0] = w;
				w->istopwindow(true);
				w->selectedobject = w->objects["inputbox"];
				w->selectedobject->SetInt(1, w->selectedobject->GetText(0).length());
				w->selectedobject->SetInt(2, 0);
				return true;
			}

		}
	}

	if (SDL_GetModState() & KMOD_CTRL)
	{
		cWindow* w = Graphics.WM.getWindow(WT_MAIN);
		if (w != NULL)
		{
			if (key == '1')
				w->objects["Status"]->onClick();
			else if (key == '2')
				w->objects["Equip"]->onClick();
			else if (key == '3')
				w->objects["Skills"]->onClick();
			else if (key == '4')
				w->objects["Items"]->onClick();
			return true;
		}
	}*/

	
	if(windows.size() > 0)
		if (windows[0]->isEnabled() && windows[0]->isVisible())
			return windows[0]->onKeyDown(key, shift);
	return false;
}
bool cWM::onKeyUp(int key, bool shift)
{
	if(windows.size() > 0)
	{
		bool parsekey = true;

		if (parsekey && windows[0]->isVisible() && windows[0]->isEnabled())
			return windows[0]->onKeyUp(key, shift);
	}
	return false;
}

void cWM::onDoubleClick()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->inWindow() && (!windows[0]->modality() || i == 0))
		{
			//move windows[i] to windows[0].
			cWindow* w = windows[i];
			if (i != 0)
			{
				for(int ii = 0; ii < i; ii++)
				{
					windows[ii]->istopwindow(false);
					windows[ii+1] = windows[ii];
				}
				windows[0] = w;
				w->istopwindow(true);
			}
			w->onDoubleClick();
			break;
		}
	}
}

void cWM::onRightClick()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->inWindow() && (!windows[0]->modality() || i == 0))
		{
			//move windows[i] to windows[0].
			cWindow* w = windows[i];
			if (i != 0)
			{
				for(int ii = 0; ii < i; ii++)
				{
					windows[ii]->istopwindow(false);
					windows[ii+1] = windows[ii];
				}
				windows[0] = w;
				w->istopwindow(true);
			}
			w->onRightClick();
			break;
		}
	}
}


void cWM::showMessage(std::string message)
{
	cWindow* w = new cMessageWindow();
	w->objects["text"]->setText(0, message);
	w->show();
	addWindow(w);
}


cWindow* cWM::getWindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isEnabled() && windows[i]->getWindowType() == wt)
		{
			return windows[i];
		}
	}
	return NULL;
}


void cWM::addWindow(cWindow* w)
{
	w->enable();
	windows.resize(windows.size()+1);
	for(int ii = windows.size()-1; ii > 0; ii--)
		windows[ii] = windows[ii-1];
	windows[0] = w;
}



void cWM::cleanWindows()
{
	std::vector<cWindow*>::iterator i;
	for(i = windows.begin(); i != windows.end(); i++)
	{
		if (!(*i)->isEnabled() && (*i)->currentColor[3] == 0)
		{
			if(draggingWindow == (*i))
				draggingWindow = NULL;

			Log(3,0,"Deleting window %s", (*i)->getTitle().c_str());
			if ((*i) != NULL)
			{
				if( (*i)->saveWindow)
					(*i)->save();
			}
			delete((*i));
			windows.erase(i);
			Log(3,0,"Closed a window");
			cleanWindows();
			return;
		}
	}
}

void cWM::printDebug()
{
	Log(3,0,"----------------------");
	for(int i = 0; i < (int)windows.size(); i++)
	{
		Log(3,0,"Window %i\nTitle: %s\nEnabled: %s\nVisible: %s\npos: (%i,%i)\nsize: (%i,%i)\n",
			i, windows[i]->getTitle().c_str(), windows[i]->isEnabled() ? "yes":"no", windows[i]->isVisible() ? "yes":"no", windows[i]->getX(), windows[i]->getY(), windows[i]->getWidth(), windows[i]->getHeight());
	}
}


void cWM::confirmWindow(std::string title, cConfirmWindowCaller* caller)
{
	cWindow* w = new cConfirmWindow(caller);
	w->objects["text"]->setText(0, title);
	w->show();
	addWindow(w);
}

/*
void confirmitemdrop(cWindow* caller, bool okclicked)
{
	if (okclicked)
	{
		char buffer[12];
		memcpy(buffer, 
		ServerConnection.SendData(PKT_IDENTIFY_CHARSERV,NULL,0);
	}
}
*/

void cWM::defocus()
{
	for(unsigned int i = 0; i < windows.size(); i++)
		windows[i]->selectedObject = NULL;
}

cWindow* cWM::inputWindow(std::string title, cInputWindowCaller* caller)
{
	cWindow* w = new cInputWindow(caller);
	w->init(texture, font);
	w->objects["text"]->setText(0, title);
	w->objects["input"]->setText(0,"");
	w->show();
	addWindow(w);
	return w;
}

cWindow* cWM::xmlWindow(std::string src)
{
	TiXmlDocument layout = cFileSystem::getXml(src);
	cWindow* w = new cXmlWindow(layout);
	addWindow(w);
	return w;
}


std::string cWM::inputWindow(std::string title, std::string defaulttext)
{
	class cDefaultInputWindowCaller : public cInputWindowCaller
	{
	public:
		bool* b;
		std::string* dat;

		cDefaultInputWindowCaller(bool* bb, std::string* str)
		{
			b = bb;
			dat = str;
		}
		void Ok()
		{
			*dat = data;
			*b = true;
		}
		void Cancel()
		{
			*dat = "";
			*b = true;
		}
	};

	bool b = false;
	std::string data = "";

	cWindow* w = inputWindow(title, new cDefaultInputWindowCaller(&b, &data));
	w->objects["input"]->setText(0, defaulttext);
	while(!b)
	{
		mainloop();
		Sleep(1);
	}

	return data;
}


bool cWM::confirmWindow(std::string title)
{
	class cDefaultConfirmWindowCaller : public cConfirmWindowCaller
	{
	public:
		int* b;
		cDefaultConfirmWindowCaller(int* bb)
		{
			b = bb;
		}
		void Ok()
		{
			*b = 1;
		}
		void Cancel()
		{
			*b = -1;
		}
	};

	int b = 0;

	confirmWindow(title, new cDefaultConfirmWindowCaller(&b));
	while(b == 0)
	{
		mainloop();
		Sleep(1);
	}

	return b > 0;
}


