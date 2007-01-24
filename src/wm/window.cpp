#include "window.h"
#include "../graphics.h"

extern float mousex, mousey;
extern cGraphics Graphics;
extern cWindowObject* draggingobject;

#include "windowinputbox.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


void cWindow::draw()
{
	glTranslatef(x, y, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texid());
	glBegin(GL_QUADS);
		glTexCoord2f(0,				496.0f/512.0f);		glVertex2d(0,  h-16);
		glTexCoord2f(1,				496.0f/512.0f);		glVertex2d(w, h-16);
		glTexCoord2f(1,				1);					glVertex2d(w, h);
		glTexCoord2f(0,				1);					glVertex2d(0,  h);
		if (!rolledup)
		{
			glTexCoord2f(0,				493.0f/512.0f);		glVertex2d(0,  h-19);
			glTexCoord2f(1,				493.0f/512.0f);		glVertex2d(w, h-19);
			glTexCoord2f(1,				496.0f/512.0f);		glVertex2d(w, h-16);
			glTexCoord2f(0,				496.0f/512.0f);		glVertex2d(0,  h-16);

			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(1,				493.0f/512.0f);		glVertex2d(w, h-19);
			glTexCoord2f(0,				493.0f/512.0f);		glVertex2d(0,  h-19);

			glTexCoord2f(0,				470.0f/512.0f);		glVertex2d(0,  0);
			glTexCoord2f(1,				470.0f/512.0f);		glVertex2d(w, 0);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);
		}
	glEnd();
	map<string, cWindowObject*, less<string> >::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second;
		if (!rolledup || o->realy2() > h-19)
		{
			//if(i->second->realy() > 0)
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
			resizeto(mousex - x, h);
		else if (resizingxy)
			resizeto(minw, h);

		if (resizingyx && ((py()+h)-(Graphics.h()-mousey)) > minh)
		{
			resizeto(w, (py()+h)-(Graphics.h()-mousey));
			y = (Graphics.h()-mousey);
		}
		else if (resizingyx)
		{
			y = (y+h) - minh;
			resizeto(w, minh);
		}

		if (resizingx && (w+x-mousex) > minh)
		{
			resizeto(w + x - mousex, h);
			x = mousex;
		}
		else if (resizingx)
		{
			x = x+w-minw;
			resizeto(minw, h);
		}

		if (resizingy && ((Graphics.h()-mousey)-py2() > minh))
		{
			resizeto(w, (Graphics.h()-mousey) - py2());
		}
		else if (resizingy)
		{
			resizeto(w, minh);
		}
	}

	if (movable && !resizing())
	{
		x=mousex-dragoffsetx;
		y=(Graphics.h()-mousey)-dragoffsety;
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

cWindowObject* cWindow::inobject()
{
	if (!visible) return false;
	objectlist::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		if (i->second->inobject() && i->second->selectable)
			return i->second;
	}
	return false;
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
		return selectedobject->onkeydown(c);
	else if (objects.find(defaultobject) != objects.end())
		return objects[defaultobject]->onkeydown(c);
	return false;
}
bool cWindow::onkeyup(int c)
{
	if (c == SDLK_TAB)
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
	else if (c == SDLK_RETURN)
	{
		if (objects.find(defaultobject) != objects.end())
			return objects[defaultobject]->onkeyup(c);
		else if (selectedobject != NULL)
			return selectedobject->onkeyup(c);

	}
	else if (selectedobject != NULL)
	{
		return selectedobject->onkeyup(c);
	}
	else if (objects.find(defaultobject) != objects.end())
		return objects[defaultobject]->onkeyup(c);
	return false;

}

void cWindow::close()
{
	if (closetype == HIDE)
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
		if (i->second->inobject() && i->second->selectable)
		{
			i->second->rightclick();
			selectedobject = i->second;
			break;
		}
	}

}
