#include "window.h"
#include <graphics.h>

extern cGraphics Graphics;
extern cWindowObject* draggingobject;

#include "windowinputbox.h"
#include "windowlabel.h"
#include "windowcheckbox.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


#ifndef __NOXML__
#include <tinyxml/tinyxml.h>
extern TiXmlDocument	config;
extern string			configfile;
#endif

void cWindow::draw()
{
	glTranslatef(x, y, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texid());
	glBegin(GL_QUADS);
		glTexCoord2f(0,				496.0f/512.0f);		glVertex2d(0,  h-16);
		glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3, h-16);
		glTexCoord2f(3/512.0f,		1);					glVertex2d(3, h);
		glTexCoord2f(0,				1);					glVertex2d(0,  h);

		glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3,  h-16);
		glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3, h-16);
		glTexCoord2f(509/512.0f,	1);					glVertex2d(w-3, h);
		glTexCoord2f(3/512.0f,		1);					glVertex2d(3,  h);

		glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3,  h-16);
		glTexCoord2f(1,				496.0f/512.0f);		glVertex2d(w, h-16);
		glTexCoord2f(1,				1);					glVertex2d(w, h);
		glTexCoord2f(509/512.0f,	1);					glVertex2d(w-3,  h);


		if (!rolledup)
		{
			glTexCoord2f(0,				493.0f/512.0f);		glVertex2d(0,  h-19);
			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3, h-19);
			glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3, h-16);
			glTexCoord2f(0,				496.0f/512.0f);		glVertex2d(0,  h-16);

			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3,  h-19);
			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3, h-19);
			glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3, h-16);
			glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3,  h-16);

			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3,  h-19);
			glTexCoord2f(1,				493.0f/512.0f);		glVertex2d(w, h-19);
			glTexCoord2f(1,				496.0f/512.0f);		glVertex2d(w, h-16);
			glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3,  h-16);


			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3, 4);
			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3, h-19);
			glTexCoord2f(0,				493.0f/512.0f);		glVertex2d(0,  h-19);

			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3,  4);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3, 4);
			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3, h-19);
			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3,  h-19);

			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3,  4);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(1,				493.0f/512.0f);		glVertex2d(w, h-19);
			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3,  h-19);



			glTexCoord2f(0,				470.0f/512.0f);		glVertex2d(0,  0);
			glTexCoord2f(3/512.0f,		470.0f/512.0f);		glVertex2d(3, 0);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3, 4);
			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);

			glTexCoord2f(3/512.0f,		470.0f/512.0f);		glVertex2d(3,  0);
			glTexCoord2f(509/512.0f,	470.0f/512.0f);		glVertex2d(w-3, 0);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3, 4);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3,  4);

			glTexCoord2f(509/512.0f,	470.0f/512.0f);		glVertex2d(w-3,  0);
			glTexCoord2f(1,				470.0f/512.0f);		glVertex2d(w, 0);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3,  4);
		}
	glEnd();
	map<string, cWindowObject*, less<string> >::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second;
		if ((!rolledup && !checkborders) || (rolledup && o->realy2() > h-190) || (!rolledup && checkborders && o->realy() < h && o->realy() > 0 && o->realx() > 0 && o->realx() < w))
		{
			o->draw();
		}
	}
/*
	if(selectedobject != NULL)
	{
		glLineWidth(1.0f);
		glColor3f(1,0,0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex2f(selectedobject->realx(), selectedobject->realy());
			glVertex2f(selectedobject->realx(), selectedobject->realy()+selectedobject->ph());
			glVertex2f(selectedobject->realx()+selectedobject->pw(), selectedobject->realy()+selectedobject->ph());
			glVertex2f(selectedobject->realx()+selectedobject->pw(), selectedobject->realy());
		glEnd();
		glColor3f(1,1,1);
	}
*/
	font->print(0,0,0,x+5,y+h-17,title.c_str());
	glTranslatef(-x, -y, 0);
}


bool cWindow::inwindow()
{
	if (!visible) return false;
	if (mousex > x && mousex < x+w &&
		(Graphics.h()-mousey) > py() && (Graphics.h()-mousey) < py()+ph())
		return true;
	return false;
}
//change cursor?


bool cWindow::drag()
{
	if (!visible) return false;
	if (resizable)
	{
		if (resizingxy && (mousex-x) > minw)
			resizeto((int)mousex - x, h);
		else if (resizingxy)
			resizeto(minw, h);

		if (resizingyx && ((py()+h)-(Graphics.h()-mousey)) > minh)
		{
			resizeto(w, (py()+h)-(Graphics.h()-(int)mousey));
			y = (Graphics.h()-(int)mousey);
		}
		else if (resizingyx)
		{
			y = (y+h) - minh;
			resizeto(w, minh);
		}

		if (resizingx && (w+x-(int)mousex) > minh)
		{
			resizeto(w + x - (int)mousex, h);
			x = (int)mousex;
		}
		else if (resizingx)
		{
			x = x+w-minw;
			resizeto(minw, h);
		}

		if (resizingy && ((Graphics.h()-mousey)-py2() > minh))
		{
			resizeto(w, (Graphics.h()-(int)mousey) - py2());
		}
		else if (resizingy)
		{
			resizeto(w, minh);
		}
	}

	if (movable && !resizing())
	{
		x=(int)mousex-(int)dragoffsetx;
		y=(Graphics.h()-(int)mousey)-(int)dragoffsety;
		if (abs(x) < SNAPPINGDIST) 
			x = 0;
		if (abs(py()) < SNAPPINGDIST)
			y = -ph2()+ph();
		if (abs(Graphics.w()-(x+w)) < SNAPPINGDIST)
			x=Graphics.w()-w;
		if (abs(Graphics.h()-(y+ph2())) < SNAPPINGDIST)
			y=Graphics.h()-h;
	}
	return true;
}

void cWindow::click()
{
//	xx-=x;
//	yy-=y;
	if (!visible) return;

	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if (i->second->inobject() && i->second->selectable)
		{
			i->second->click();
			selectedobject = i->second;
			break;
		}
	}

}


int cWindow::getcursor()
{
	if (!visible) return 0;


	objectlist::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		if (i->second->inobject() && i->second->cursortype != 0)
			return i->second->cursortype;
	}
	if (resizable && !rolledup)
	{
		int hresize = 0;
		int vresize = 0;
		if(mousex < x+w && mousex > x+w - DRAGBORDER)
			hresize = 1;
		if((Graphics.h()-mousey) > y && (Graphics.h()-mousey) < y + DRAGBORDER)
			vresize = 1;
		if(mousex > x && mousex < x + DRAGBORDER)
			hresize = 2;
		if((Graphics.h()-mousey) < y+h && (Graphics.h()-mousey) > y+h - DRAGBORDER)
			vresize = 2;

		if (hresize == 0)
		{
			if (vresize == 1)
				return 3;
			else if (vresize == 2)
				return 3;
		}
		else if (hresize == 1)
		{
			if (vresize == 0)
				return 2;
			else if (vresize == 1)
				return 4;
			else if (vresize == 2)
				return 5;
		}
		else if (hresize == 2)
		{
			if (vresize == 0)
				return 2;
			else if (vresize == 1)
				return 5;
			else if (vresize == 2)
				return 4;
		}
	}

	return 0;
}

bool cWindow::onborder()
{
	if (!visible) return false;
	if (resizable && !rolledup)
	{
		if(mousex < x+w && mousex > x+w - DRAGBORDER)
			return true;
		if((Graphics.h()-mousey) > y && (Graphics.h()-mousey) < y + DRAGBORDER)
			return true;
		if(mousex > x && mousex < x + DRAGBORDER)
			return true;
		if((Graphics.h()-mousey) < y+h && (Graphics.h()-mousey) > y+h - DRAGBORDER)
			return true;
	}

	return false;
}

cWindowObject* cWindow::inobject()
{
	if (!visible) return false;
	objectlist::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second->inobject();
		if (o != NULL && i->second->selectable)
			return o;
	}
	return NULL;
}


void cWindow::center()
{
	x = (Graphics.w()/2)-(w/2);
	y = (Graphics.h()/2)-(h/2);
}

bool cWindow::onchar(char c)
{
	if (selectedobject != NULL)
		return selectedobject->onchar(c);
	else if (objects.find(defaultobject) != objects.end())
		return objects[defaultobject]->onchar(c);
	return false;
}
bool cWindow::onkeydown(int c)
{
	if (selectedobject != NULL)
		if(selectedobject->onkeydown(c))
			return true;
	if (c == SDLK_RETURN)
	{
		if (objects.find(defaultobject) != objects.end())
			return objects[defaultobject]->onkeydown(c);
		else if (selectedobject != NULL)
			return selectedobject->onkeydown(c);

	}
	else if (c == SDLK_TAB)
	{
		int selcount = 0;
		objectlist::iterator i;
		objectlist::iterator ii = objects.end();
		for(i = objects.begin(); i != objects.end(); i++)
			if (i->second->selectable)
				selcount++;

		if (selcount > 0)
		{
			if (!(SDL_GetModState() & KMOD_SHIFT))
			{
				do
				{
					for(i = objects.begin(); i != objects.end(); i++)
						if (i->second == selectedobject)
							ii = i;
					ii++;
					if (ii == objects.end())
						ii = objects.begin();
					selectedobject = ii->second;
				} while (!selectedobject->selectable);
			}
			else
			{
				cWindowObject* oldselection = selectedobject;
				do
				{
					objectlist::iterator last;
					for(i = objects.begin(); i != objects.end(); i++)
					{
						if (i != objects.end())
							last = i;
						if (i->second == selectedobject)
							ii = i;
					}
					if (ii == objects.begin())
						ii = last;
					else
						ii--;
					selectedobject = ii->second;
				} while (!selectedobject->selectable && oldselection != selectedobject);
			}
			if (selectedobject->type == OBJECT_INPUTBOX)
			{
				selectedobject->SetInt(1, selectedobject->GetText(0).length());
				selectedobject->SetInt(2, 0);

			}
		}
		return true;
	}
	else if (objects.find(defaultobject) != objects.end())
		return objects[defaultobject]->onkeydown(c);
	return false;
}
bool cWindow::onkeyup(int c)
{
	if (selectedobject != NULL)
	{
		return selectedobject->onkeyup(c);
	}
	else if (objects.find(defaultobject) != objects.end())
		return objects[defaultobject]->onkeyup(c);
	return false;

}

void cWindow::close(bool force)
{
	if (closetype == HIDE && !force)
	{
		hide();
		return;
	}

	Graphics.WM.delwindow(this);
	hide();
}


void cWindow::doubleclick()
{
//	xx-=x;
//	yy-=y;
	if (!visible) return;

	cWindowObject* o = NULL;
	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if (i->second->inobject() && i->second->selectable)
			o = i->second;
	}
	if (o != NULL)
	{
		o->doubleclick();
		selectedobject = o;
	}

}

void cWindow::rightclick()
{
//	xx-=x;
//	yy-=y;
	if (!visible) return;

	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second->inobject();
		if (o != NULL && i->second->selectable)
		{
			selectedobject = o;
			o->rightclick();
			break;
		}
	}

}


cWindowObject* cWindow::addlabel(string name, int x, int y, string text)
{
	cWindowObject* o = new cWindowLabel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(x,y);
	o->resizeto(Graphics.WM.font.textlen(text), 12);
	o->SetText(0,text);
	objects[name] = o;
	return o;
}


cWindowObject* cWindow::addinputbox(string name, int x, int y, int w, string text)
{
	cWindowObject* o = new cWindowInputBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(x,y);
	o->resizeto(w, 20);
	o->SetText(0,text);
	objects[name] = o;
	return o;
}

cWindowObject* cWindow::addcheckbox(string name, int x, int y, bool checked)
{
	cWindowObject* o = new cWindowCheckBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(x,y);
	o->SetInt(0, checked ? 1 : 0);
	objects[name] = o;
	return o;
}


void cWindow::holddragover()
{
	cWindowObject* o = inobject();
	if(o != NULL)
		o->holddragover();	
}

void cWindow::dragover()
{
	cWindowObject* o = inobject();
	if(o != NULL)
		o->dragover();	
}

void cWindow::scrollup()
{
	if (!visible) return;

	for(objectlist::reverse_iterator i = objects.rbegin(); i != objects.rend(); i++)
	{
		cWindowObject* o = i->second->inobject();
		if (o != NULL && i->second->selectable)
		{
			i->second->scrollup();
			break;
		}
	}
}

void cWindow::scrolldown()
{
	if (!visible) return;

	for(objectlist::reverse_iterator i = objects.rbegin(); i != objects.rend(); i++)
	{
		cWindowObject* o = i->second->inobject();
		if (o != NULL)
		{
			cWindowObject* oo = i->second;
			i->second->scrolldown();
			break;
		}
	}
}


void cWindow::save()
{
#ifndef __NOXML__
	if(saveprops != "")
	{
		char* elements[] = {"x","y","h","w" };

		if(config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str()) == NULL)
			config.FirstChildElement("settings")->InsertEndChild(TiXmlElement(saveprops.c_str()));

		for(int i = 0; i < 4; i++)
		{
			if(config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement(elements[i]) == NULL)
			{
				config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->InsertEndChild(TiXmlElement(elements[i]));
				config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement(elements[i])->InsertEndChild(TiXmlText(""));
			}
		}

		char buf[10];
		sprintf(buf, "%i", x);
		config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement("x")->FirstChild()->SetValue(buf);

		sprintf(buf, "%i", y);
		config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement("y")->FirstChild()->SetValue(buf);

		sprintf(buf, "%i", h);
		config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement("h")->FirstChild()->SetValue(buf);

		sprintf(buf, "%i", w);
		config.FirstChildElement("settings")->FirstChildElement(saveprops.c_str())->FirstChildElement("w")->FirstChild()->SetValue(buf);

		config.SaveFile(configfile.c_str());

	}
#endif
}

void cWindow::initprops(string s)
{
#ifndef __NOXML__
	saveprops = s;
	if(config.FirstChildElement("settings")->FirstChildElement(s.c_str()))
	{
		if(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("x"))
			x = atoi(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("x")->FirstChild()->Value());
		if(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("y"))
			y = atoi(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("y")->FirstChild()->Value());
		if(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("h"))
			h = atoi(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("h")->FirstChild()->Value());
		if(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("w"))
			w = atoi(config.FirstChildElement("settings")->FirstChildElement(s.c_str())->FirstChildElement("w")->FirstChild()->Value());
	}
#endif
}