#include "wm.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "../graphics.h"
#include "messagewindow.h"
#include "confirmwindow.h"
extern cGraphics Graphics;
extern cWindow* draggingwindow;
extern string fontname;

int cWM::draw()
{
	CleanWindows();
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	int size = windows.size()-1;
	for(int i = size; i >= 0; i--)
	{
		cWindow* w = windows[i];
		if (w->isenabled())
		{
			if (i == 0)
			{
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				w->istopwindow(true);
			}
			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				w->istopwindow(false);
			}
			if(w->isvisible() && w->isenabled())
				w->draw();
		}
		if (windows[i] != w)
			break;	
	}
	return 1;
}


int cWM::init()
{
	texture.Load("data\\window.tga");
	font.load("data\\fonts\\"+fontname+".tga");

	focus = 0;
	Log(3,0,"Window Manager initialized");
	return 1;
}

void cWM::save()
{

	for(int i = 0; i < (int)windows.size(); i++)
	{
		if(windows[i]->savewindow)
			windows[i]->save();
	}

//////////////////////////////////////////////////////////////////////////////////////////////
}

cWM::~cWM()
{
	for(int i = 0; i < (int)windows.size(); i++)
		delete(windows[i]);
	windows.clear();
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
			if ((w->px() <= windows[i]->px() && windows[i]->px() <= w->px()+w->pw()) ||
				(windows[i]->px() <= w->px() && w->px() <= windows[i]->px()+windows[i]->pw()))
			{
				if (abs(w->py() - (windows[i]->py()+windows[i]->ph())) < SNAPPINGDIST)
				{
					if (w->resizing())
					{
						w->resizeto(w->pw(), w->ph()+(w->py()-(windows[i]->py()+windows[i]->ph())));
						w->moveto(w->px(), windows[i]->py() + windows[i]->ph());
					}
					else
						w->moveto(w->px(), windows[i]->py()+windows[i]->ph()-w->ph2()+w->ph());
				}
				if (abs((w->py()+w->ph()) - windows[i]->py()) < SNAPPINGDIST)
				{
					if (w->resizing())
						w->resizeto(w->pw(), windows[i]->py()-w->py());
					else
						w->moveto(w->px(), windows[i]->py()-w->ph2());
				}
				
			}
			if ((w->py() <= windows[i]->py() && windows[i]->py() <= w->py()+w->ph()) ||
				(windows[i]->py() <= w->py() && w->py() <= windows[i]->py()+windows[i]->ph()))
			{
				if (abs(w->px() - (windows[i]->px()+windows[i]->pw())) < SNAPPINGDIST)
				{
					if (w->resizing())
					{
						w->resizeto(w->pw()+(w->px()-(windows[i]->px()+windows[i]->pw())), w->ph());
						w->moveto(windows[i]->px() + windows[i]->pw(), w->py());
					}
					else
						w->moveto(windows[i]->px()+windows[i]->pw(), w->py());
				}
				if (abs((w->px()+w->pw()) - windows[i]->px()) < SNAPPINGDIST)
				{
					if (w->resizing())
						w->resizeto(windows[i]->px()-w->px(), w->ph());
					else
						w->moveto(windows[i]->px()-w->pw(), w->py());
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
			if (windows[i]->isvisible())
			{
				cWindow* w = windows[i];
				for(int ii = i; ii > -1; ii--)
					windows[ii] = windows[ii-1];
				windows[0] = w;
				break;
			}
		}
	}
}

int cWM::closewindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
	{
		cWindow* w = windows[i];
		if (w != NULL)

			if (w->isenabled() && w->windowtype() == wt)
				w->close();
	}
	return 0;

}

bool cWM::onchar(char c)
{
	if (windows.size() > 0)
		if (windows[0]->isenabled() && windows[0]->isvisible())
			return windows[0]->onchar(c);
	return false;
}

bool cWM::onkeydown(int key)
{
	if(windows.size() > 0)
		if (windows[0]->isenabled() && windows[0]->isvisible())
			return windows[0]->onkeydown(key);
	return false;
}
bool cWM::onkeyup(int key)
{
	if(windows.size() > 0)
	{
		bool parsekey = true;

		if (parsekey && windows[0]->isvisible() && windows[0]->isenabled())
			return windows[0]->onkeyup(key);
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


void cWM::MessageBox(string message)
{
	cWindow* w = new cMessageWindow();
	w->init(&texture, &font);
	w->objects["text"]->SetText(0, message);
	w->show();
	addwindow(w);
}


cWindow* cWM::getwindow(WINDOW_TYPE wt)
{
	for(int i = 0; i < (int)windows.size(); i++)
		if (windows[i]->isenabled() && windows[i]->windowtype() == wt)
			return windows[i];
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


void cWM::delwindow(cWindow* w, bool savewindow)
{
	w->savewindow = savewindow;
	w->disable();
	w->hide();
}



void cWM::CleanWindows()
{
	vector<cWindow*>::iterator i;
	for(i = windows.begin(); i != windows.end(); i++)
	{
		if (!(*i)->isenabled())
		{
			if(draggingwindow == (*i))
				draggingwindow = NULL;

			Log(3,0,"Deleting window %s", (*i)->gettitle().c_str());
			if ((*i) != NULL)
			{
				if( (*i)->savewindow)
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
			i, windows[i]->gettitle().c_str(), windows[i]->isenabled() ? "yes":"no", windows[i]->isvisible() ? "yes":"no", windows[i]->px(), windows[i]->py(), windows[i]->pw(), windows[i]->ph());
	}
}


void cWM::ConfirmWindow(string title, cConfirmWindow::cConfirmWindowCaller* caller)
{
	cWindow* w = new cConfirmWindow(caller);
	w->init(&texture, &font);
	w->objects["text"]->SetText(0, title);
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
	for(int i = 0; i < windows.size(); i++)
		windows[i]->selectedobject = NULL;
}
