#include "wm.h"

#include <common.h>
#include <font.h>
#include <texture.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <graphics.h>
#include <windows/messagewindow.h>
#include <windows/confirmwindow.h>
#include <windows/xmlwindow.h>
extern cGraphics Graphics;
extern cFileSystem fs;
extern cWindow* draggingwindow;

extern void mainloop();

int cWM::draw()
{
	unsigned int i;
	int ii;
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//glTranslatef(0,0,9000);
	CleanWindows();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glColor4fv(colorblur);
	glEnable(GL_BLEND);
	int size = windows.size()-1;
	int topwindow = 0;
	for(i = 0; i < windows.size(); i++)
	{
		cWindow* w = windows[i];
		if (w->isenabled())
		{
			if(w->isVisible() && w->isenabled())
			{
				topwindow = i;
				break;
			}
		}
	}
	

	for(ii = size; ii >= 0; ii--)
	{
		cWindow* w = windows[ii];
		if(w->isalwaysontop())
			continue;
		if (w->isenabled())
		{
			if (ii == topwindow)
			{
				glColor4fv(color);
				w->istopwindow(true);
			}
			else
			{
				if(w->canbetransparent())
					glColor4fv(colorblur);
				else
					glColor4fv(color);
				w->istopwindow(false);
			}
			if(w->isVisible() && w->isenabled())
				w->draw();
		}
		if (windows[ii] != w)
			break;	
	}
	for(ii = size; ii >= 0; ii--)
	{
		cWindow* w = windows[ii];
		if(!w->isalwaysontop())
			continue;
		if (w->isenabled())
		{
			if (ii == topwindow)
			{
				glColor4fv(color);
				w->istopwindow(true);
			}
			else
			{
				if(w->canbetransparent())
					glColor4fv(colorblur);
				else
					glColor4fv(color);
				w->istopwindow(false);
			}
			if(w->isVisible() && w->isenabled())
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
	skin = fs.getXml(sSkin);
	texture = TextureCache.load(skin.FirstChildElement("skin")->FirstChildElement("texture")->FirstChild()->Value());
	font = new cFont();
	font->load(skin.FirstChildElement("skin")->FirstChildElement("font")->FirstChild()->Value());

	std::string c = skin.FirstChildElement("skin")->FirstChildElement("color")->FirstChild()->Value();
	color[0] = hex2dec(c.substr(0,2))/255.0f;
	color[1] = hex2dec(c.substr(2,2))/255.0f;
	color[2] = hex2dec(c.substr(4,2))/255.0f;
	color[3] = hex2dec(c.substr(6,2))/255.0f;

	c = skin.FirstChildElement("skin")->FirstChildElement("colorblurred")->FirstChild()->Value();
	colorblur[0] = hex2dec(c.substr(0,2))/255.0f;
	colorblur[1] = hex2dec(c.substr(2,2))/255.0f;
	colorblur[2] = hex2dec(c.substr(4,2))/255.0f;
	colorblur[3] = hex2dec(c.substr(6,2))/255.0f;

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

void cWM::unload()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		windows[i]->save();
		delete(windows[i]);
	}
	windows.clear();
	if(texture)	
		texture->unLoad();
}

cWM::~cWM()
{
	unload();
}

void cWM::click(bool b)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->inwindow() && (!windows[0]->modality() || i == 0))
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
				w->click();
			break;
		}
	}

}

cWindow* cWM::inwindow()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->isenabled() && windows[i]->inwindow() && (i == 0 || !windows[0]->modality()))
			return windows[i];
	}
	return NULL;
}


void cWM::drag(cWindow* w)
{//makes sure window w snaps to other windows....
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->isenabled() && windows[i] != w)
		{
			if ((w->getX() <= windows[i]->getX() && windows[i]->getX() <= w->getX()+w->getWidth()) ||
				(windows[i]->getX() <= w->getX() && w->getX() <= windows[i]->getX()+windows[i]->getWidth()))
			{
				if (abs(w->getY() - (windows[i]->getY()+windows[i]->getHeight())) < SNAPPINGDIST)
				{
					if (w->resizing())
					{
						w->resizeTo(w->getWidth(), w->getHeight()+(w->getY()-(windows[i]->getY()+windows[i]->getHeight())));
						w->moveto(w->getX(), windows[i]->getY() + windows[i]->getHeight());
					}
					else
						w->moveto(w->getX(), windows[i]->getY()+windows[i]->getHeight()-w->ph2()+w->getHeight());
				}
				if (abs((w->getY()+w->getHeight()) - windows[i]->getY()) < SNAPPINGDIST)
				{
					if (w->resizing())
						w->resizeTo(w->getWidth(), windows[i]->getY()-w->getY());
					else
						w->moveto(w->getX(), windows[i]->getY()-w->ph2());
				}
				
			}
			if ((w->getY() <= windows[i]->getY() && windows[i]->getY() <= w->getY()+w->getHeight()) ||
				(windows[i]->getY() <= w->getY() && w->getY() <= windows[i]->getY()+windows[i]->getHeight()))
			{
				if (abs(w->getX() - (windows[i]->getX()+windows[i]->getWidth())) < SNAPPINGDIST)
				{
					if (w->resizing())
					{
						w->resizeTo(w->getWidth()+(w->getX()-(windows[i]->getX()+windows[i]->getWidth())), w->getHeight());
						w->moveto(windows[i]->getX() + windows[i]->getWidth(), w->getY());
					}
					else
						w->moveto(windows[i]->getX()+windows[i]->getWidth(), w->getY());
				}
				if (abs((w->getX()+w->getWidth()) - windows[i]->getX()) < SNAPPINGDIST)
				{
					if (w->resizing())
						w->resizeTo(windows[i]->getX()-w->getX(), w->getHeight());
					else
						w->moveto(windows[i]->getX()-w->getWidth(), w->getY());
				}

			}

		}

	}

}

void cWM::showwindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->windowtype() == wt)
			windows[i]->show();
	}
}

bool cWM::existswindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->windowtype() == wt)
			return true;
	}
	return false;
}


void cWM::togglewindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->windowtype() == wt)
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

int cWM::closewindow(WINDOW_TYPE wt, bool force)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		cWindow* w = windows[i];
		if (w != NULL)

			if (w->isenabled() && w->windowtype() == wt)
				w->close(force);
	}
	return 0;

}

bool cWM::onchar(char c, bool shift)
{
	if (windows.size() > 0)
		if (windows[0]->isenabled() && windows[0]->isVisible())
			return windows[0]->onChar(c, shift);
	return false;
}

bool cWM::onkeydown(int key, bool shift)
{
	if(windows.size() == 0)
		return false;

/*	if (key == SDLK_RETURN && windows[0]->isenabled() && windows[0]->defaultobject == "")
	{
		cWindow* w = getwindow(WT_CHAT);
		if (w != NULL)
		{
			int i;
			for(i = 0; i < (int)windows.size(); i++)
				if (windows[i]->windowtype() == WT_CHAT) break;
			

			

			if (windows[0]->windowtype() == WT_CHAT && w->selectedobject != w->objects["inputbox"])
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
		cWindow* w = Graphics.WM.getwindow(WT_MAIN);
		if (w != NULL)
		{
			if (key == '1')
				w->objects["Status"]->click();
			else if (key == '2')
				w->objects["Equip"]->click();
			else if (key == '3')
				w->objects["Skills"]->click();
			else if (key == '4')
				w->objects["Items"]->click();
			return true;
		}
	}*/

	
	if(windows.size() > 0)
		if (windows[0]->isenabled() && windows[0]->isVisible())
			return windows[0]->onKeyDown(key, shift);
	return false;
}
bool cWM::onkeyup(int key, bool shift)
{
	if(windows.size() > 0)
	{
		bool parsekey = true;

		if (parsekey && windows[0]->isVisible() && windows[0]->isenabled())
			return windows[0]->onKeyUp(key, shift);
	}
	return false;
}

void cWM::doubleclick()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->inwindow() && (!windows[0]->modality() || i == 0))
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
			w->doubleclick();
			break;
		}
	}
}

void cWM::rightclick()
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->inwindow() && (!windows[0]->modality() || i == 0))
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
			w->rightclick();
			break;
		}
	}
}


void cWM::ShowMessage(std::string message)
{
	cWindow* w = new cMessageWindow(texture, font,skin);
	w->objects["text"]->setText(0, message);
	w->show();
	addwindow(w);
}


cWindow* cWM::getwindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		if (windows[i]->isenabled() && windows[i]->windowtype() == wt)
		{
			return windows[i];
		}
	}
	return NULL;
}


void cWM::addwindow(cWindow* w)
{
	w->enable();
	windows.resize(windows.size()+1);
	for(int ii = windows.size()-1; ii > 0; ii--)
		windows[ii] = windows[ii-1];
	windows[0] = w;
}



void cWM::CleanWindows()
{
	std::vector<cWindow*>::iterator i;
	for(i = windows.begin(); i != windows.end(); i++)
	{
		if (!(*i)->isenabled() && (*i)->currentColor[3] == 0)
		{
			if(draggingwindow == (*i))
				draggingwindow = NULL;

			Log(3,0,"Deleting window %s", (*i)->gettitle().c_str());
			if ((*i) != NULL)
			{
				if( (*i)->saveWindow)
					(*i)->save();
			}
			delete((*i));
			windows.erase(i);
			Log(3,0,"Closed a window");
			CleanWindows();
			return;
		}
	}
}

void cWM::printdebug()
{
	Log(3,0,"----------------------");
	for(int i = 0; i < (int)windows.size(); i++)
	{
		Log(3,0,"Window %i\nTitle: %s\nEnabled: %s\nVisible: %s\npos: (%i,%i)\nsize: (%i,%i)\n",
			i, windows[i]->gettitle().c_str(), windows[i]->isenabled() ? "yes":"no", windows[i]->isVisible() ? "yes":"no", windows[i]->getX(), windows[i]->getY(), windows[i]->getWidth(), windows[i]->getHeight());
	}
}


void cWM::ConfirmWindow(std::string title, cConfirmWindow::cConfirmWindowCaller* caller)
{
	cWindow* w = new cConfirmWindow(caller, texture, font,skin);
	w->objects["text"]->setText(0, title);
	w->show();
	addwindow(w);
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

cWindow* cWM::InputWindow(std::string title, cInputWindow::cInputWindowCaller* caller)
{
	cWindow* w = new cInputWindow(caller, texture, font,skin);
	w->init(texture, font);
	w->objects["text"]->setText(0, title);
	w->objects["input"]->setText(0,"");
	w->show();
	addwindow(w);
	return w;
}

cWindow* cWM::XmlWindow(std::string src)
{
	TiXmlDocument layout = fs.getXml(src);
	cWindow* w = new cXmlWindow(texture, font, skin, layout);
	addwindow(w);
	return w;
}


std::string cWM::InputWindow(std::string title, std::string defaulttext)
{
	class cDefaultInputWindowCaller : public cInputWindow::cInputWindowCaller
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

	cWindow* w = InputWindow(title, new cDefaultInputWindowCaller(&b, &data));
	w->objects["input"]->setText(0, defaulttext);
	while(!b)
	{
		mainloop();
		Sleep(1);
	}

	return data;
}


bool cWM::ConfirmWindow(std::string title)
{
	class cDefaultConfirmWindowCaller : public cConfirmWindow::cConfirmWindowCaller
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

	ConfirmWindow(title, new cDefaultConfirmWindowCaller(&b));
	while(b == 0)
	{
		mainloop();
		Sleep(1);
	}

	return b > 0;
}


