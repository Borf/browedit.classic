#include <common.h>
#include <texture.h>
#include <font.h>
#include "windowobject.h"
#include "window.h"


#include <graphics.h>

extern cGraphics Graphics;
extern cWindowObject* draggingObject;

#include <font.h>
#include "windowinputbox.h"
#include "windowlabel.h"
#include "windowcheckbox.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

#ifndef __NOXML__
#include <tinyxml/tinyxml.h>
extern TiXmlDocument	config;
extern std::string			configfile;
#endif


cWindow::cWindow(cTexture* t, cFont* f, TiXmlDocument &skin)
{
	noTransparency = false;
	visible = false;
	rolledUp = false;
	resizable = true;
	movable = true;
	resizingX = false;
	resizingY = false;
	resizingXY = false;
	resizingYX = false;
	checkBorders = false;
	alwaysOnTop = false;
	minHeight = 100;
	minWidth = 100;
	selectedObject = NULL;
	closeType = CLOSE;
	modal = false;
	enabled = true;
	texture = t;
	font = f;
	saveProperties = "";

	std::string color = skin.FirstChildElement("skin")->FirstChildElement("window")->FirstChildElement("fontcolor")->FirstChild()->Value();
	fontColor[0] = hex2dec(color.substr(0,2)) / 256.0f;
	fontColor[1] = hex2dec(color.substr(2,2)) / 256.0f;
	fontColor[2] = hex2dec(color.substr(4,2)) / 256.0f;

	color = skin.FirstChildElement("skin")->FirstChildElement("window")->FirstChildElement("title")->FirstChildElement("fontcolor")->FirstChild()->Value();
	titleColor[0] = hex2dec(color.substr(0,2)) / 256.0f;
	titleColor[1] = hex2dec(color.substr(2,2)) / 256.0f;
	titleColor[2] = hex2dec(color.substr(4,2)) / 256.0f;

	titleOffX = atoi(skin.FirstChildElement("skin")->FirstChildElement("window")->FirstChildElement("title")->FirstChildElement("xoff")->FirstChild()->Value());
	titleOffY = atoi(skin.FirstChildElement("skin")->FirstChildElement("window")->FirstChildElement("title")->FirstChildElement("yoff")->FirstChild()->Value());


	TiXmlElement* wSkin = skin.FirstChildElement("skin")->FirstChildElement("window");

	skinTopHeight = atoi(wSkin->FirstChildElement("top")->Attribute("height"));
	skinTop =		512 - atoi(wSkin->FirstChildElement("top")->FirstChild()->Value());
	skinBottomHeight = atoi(wSkin->FirstChildElement("bottom")->Attribute("height"));
	skinBottom =		512 - atoi(wSkin->FirstChildElement("bottom")->FirstChild()->Value());
	
	skinLeftWidth = atoi(wSkin->FirstChildElement("left")->Attribute("width"));
	skinLeft =		atoi(wSkin->FirstChildElement("left")->FirstChild()->Value());
	skinRightWidth = atoi(wSkin->FirstChildElement("right")->Attribute("width"));
	skinRight =		atoi(wSkin->FirstChildElement("right")->FirstChild()->Value());

	wSkin = wSkin->FirstChildElement("offsets");
	skinOffLeft =	atoi(wSkin->FirstChildElement("left")->FirstChild()->Value());
	skinOffRight =	atoi(wSkin->FirstChildElement("right")->FirstChild()->Value());
	skinOffTop =	atoi(wSkin->FirstChildElement("top")->FirstChild()->Value());
	skinOffBottom = atoi(wSkin->FirstChildElement("bottom")->FirstChild()->Value());


	currentColor[0] = Graphics.WM.color[0];
	currentColor[1] = Graphics.WM.color[1];
	currentColor[2] = Graphics.WM.color[2];
	currentColor[3] = 0;

}


void cWindow::draw()
{
	GLfloat tempColors[4];
	glGetFloatv(GL_CURRENT_COLOR, tempColors);


	if(!enabled || !visible)
	{
		tempColors[3] = 0;
	}

	for(int ii = 0; ii < 4; ii++)
	{
		if(currentColor[ii] > tempColors[ii])
		{
			currentColor[ii] -= (Graphics.frameticks / 200.0f);
			if(currentColor[ii] < tempColors[ii])
				currentColor[ii] = tempColors[ii];
		}			
		else if(currentColor[ii] < tempColors[ii])
		{
			currentColor[ii] += (Graphics.frameticks / 200.0f);
			if(currentColor[ii] > tempColors[ii])
				currentColor[ii] = tempColors[ii];
		}

	}
	glColor4fv(currentColor);

	glPushMatrix();
	glTranslatef(x, y, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texId());
	glBegin(GL_QUADS);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(0,				h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(skinLeftWidth,	h);
		glTexCoord2f(skinLeft/512.0f,					skinTop/512.0f);						glVertex2d(0,				h);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(w-skinRightWidth,h);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(skinLeftWidth,	h);

		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(w,				h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinTop/512.0f);						glVertex2d(w,				h);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(w-skinRightWidth,h);


		if (!rolledUp)
		{
			glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(0,				skinBottomHeight);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	h-skinTopHeight);
			glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(0,				h-skinTopHeight);

			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
			glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
			glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,h-skinTopHeight);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	h-skinTopHeight);

			glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
			glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(w,				skinBottomHeight);
			glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(w,				h-skinTopHeight);
			glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,h-skinTopHeight);
///////////////			
			glTexCoord2f(skinLeft/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(0,				0);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(skinLeftWidth,	0);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
			glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(0,				skinBottomHeight);

			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(skinLeftWidth,	0);
			glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w-skinRightWidth,0);
			glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);

			glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w-skinRightWidth,0);
			glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w,				0);
			glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(w,				skinBottomHeight);
			glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
		}
	glEnd();

//	glTranslatef(skinOffLeft, skinOffTop,0);

	objectlist::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second;
		if(rolledUp)
		{
			if(o->realy2() >= h-20)
				o->draw();
		}
		else if (
			(!rolledUp && !checkBorders) ||
			(!rolledUp && checkBorders && o->realY() < h && o->realY() > 0 && o->realX() > 0 && o->realX() < w))
		{
			o->draw();
		}
	}
/*
	if(selectedObject != NULL)
	{
		glLineWidth(1.0f);
		glColor3f(1,0,0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex2f(selectedObject->realX(), selectedObject->realY());
			glVertex2f(selectedObject->realX(), selectedObject->realY()+selectedObject->getHeight());
			glVertex2f(selectedObject->realX()+selectedObject->pw(), selectedObject->realY()+selectedObject->getHeight());
			glVertex2f(selectedObject->realX()+selectedObject->pw(), selectedObject->realY());
		glEnd();
		glColor3f(1,1,1);
	}
*/
	font->print(titleColor[0], titleColor[1], titleColor[2],x+titleOffX,y+h-(titleOffY+12),title.c_str());
	glPopMatrix();
	glColor4fv(tempColors);
}


bool cWindow::inwindow()
{
	if (!visible) return false;
	if (mouseX > x && mouseX < x+w &&
		(Graphics.h()-mouseY) > getY() && (Graphics.h()-mouseY) < getY()+getHeight())
		return true;
	return false;
}
//change cursor?


bool cWindow::drag()
{
	if (!visible) return false;
	if (resizable)
	{
		if (resizingXY && (mouseX-x) > minWidth)
			resizeTo((int)mouseX - x, h);
		else if (resizingXY)
			resizeTo(minWidth, h);

		if (resizingYX && ((getY()+h)-(Graphics.h()-mouseY)) > minHeight)
		{
			resizeTo(w, (getY()+h)-(Graphics.h()-(int)mouseY));
			y = (Graphics.h()-(int)mouseY);
		}
		else if (resizingYX)
		{
			y = (y+h) - minHeight;
			resizeTo(w, minHeight);
		}

		if (resizingX && (w+x-(int)mouseX) > minHeight)
		{
			resizeTo(w + x - (int)mouseX, h);
			x = (int)mouseX;
		}
		else if (resizingX)
		{
			x = x+w-minWidth;
			resizeTo(minWidth, h);
		}

		if (resizingY && ((Graphics.h()-mouseY)-py2() > minHeight))
		{
			resizeTo(w, (Graphics.h()-(int)mouseY) - py2());
		}
		else if (resizingY)
		{
			resizeTo(w, minHeight);
		}
	}

	if (movable && !(resizing() && resizable))
	{
		x=(int)mouseX-(int)dragoffsetx;
		y=(Graphics.h()-(int)mouseY)-(int)dragoffsety;
		if (abs(x) < SNAPPINGDIST) 
			x = 0;
		if (abs(getY()) < SNAPPINGDIST)
			y = -ph2()+getHeight();
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

	for(objectlist::reverse_iterator i = objects.rbegin(); i != objects.rend(); i++)
	{
		if (i->second->inObject() && i->second->selectable && i->second->isEnabled())
		{
			i->second->click();
			selectedObject = i->second;
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
		if (i->second->inObject() && i->second->cursorType != 0)
			return i->second->cursorType;
	}
	if (resizable && !rolledUp)
	{
		int hresize = 0;
		int vresize = 0;
		if(mouseX < x+w && mouseX > x+w - DRAGBORDER)
			hresize = 1;
		if((Graphics.h()-mouseY) > y && (Graphics.h()-mouseY) < y + DRAGBORDER)
			vresize = 1;
		if(mouseX > x && mouseX < x + DRAGBORDER)
			hresize = 2;
		if((Graphics.h()-mouseY) < y+h && (Graphics.h()-mouseY) > y+h - DRAGBORDER)
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
	if (resizable && !rolledUp)
	{
		if(mouseX < x+w && mouseX > x+w - DRAGBORDER)
			return true;
		if((Graphics.h()-mouseY) > y && (Graphics.h()-mouseY) < y + DRAGBORDER)
			return true;
		if(mouseX > x && mouseX < x + DRAGBORDER)
			return true;
		if((Graphics.h()-mouseY) < y+h && (Graphics.h()-mouseY) > y+h - DRAGBORDER)
			return true;
	}

	return false;
}

cWindowObject* cWindow::inObject()
{
	if (!visible) return false;
	objectlist::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second->inObject();
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

bool cWindow::onChar(char c,bool shift)
{
	if (selectedObject != NULL)
		return selectedObject->onChar(c, shift);
	else if (objects.find(defaultObject) != objects.end())
		return objects[defaultObject]->onChar(c, shift);
	return false;
}
bool cWindow::onKeyDown(int c,bool shift)
{
	if (selectedObject != NULL)
		if(selectedObject->onKeyDown(c, shift))
			return true;
	if (c == SDLK_RETURN)
	{
		if (objects.find(defaultObject) != objects.end())
			return objects[defaultObject]->onKeyDown(c, shift);
		else if (selectedObject != NULL)
			return selectedObject->onKeyDown(c, shift);

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
			if (!shift)
			{
				do
				{
					for(i = objects.begin(); i != objects.end(); i++)
						if (i->second == selectedObject)
							ii = i;
					if(ii != objects.end())
						ii++;
					if (ii == objects.end())
						ii = objects.begin();
					selectedObject = ii->second;
				} while (!selectedObject->selectable);
			}
			else
			{
				cWindowObject* oldselection = selectedObject;
				do
				{
					objectlist::iterator last;
					for(i = objects.begin(); i != objects.end(); i++)
					{
						if (i != objects.end())
							last = i;
						if (i->second == selectedObject)
							ii = i;
					}
					if (ii == objects.begin())
						ii = last;
					else
						ii--;
					selectedObject = ii->second;
				} while (!selectedObject->selectable && oldselection != selectedObject);
			}
			if (selectedObject->type == OBJECT_INPUTBOX)
			{
				selectedObject->setInt(1, selectedObject->getText(0).length());
				selectedObject->setInt(2, 0);

			}
		}
		return true;
	}
	else if (objects.find(defaultObject) != objects.end())
		return objects[defaultObject]->onKeyDown(c,shift);
	return false;
}
bool cWindow::onKeyUp(int c,bool shift)
{
	if (selectedObject != NULL)
	{
		return selectedObject->onKeyUp(c,shift);
	}
	else if (objects.find(defaultObject) != objects.end())
		return objects[defaultObject]->onKeyUp(c,shift);
	return false;

}

void cWindow::close(bool force)
{
	if (closeType == HIDE && !force)
	{
		hide();
		return;
	}

	save();
	disable();
}


void cWindow::doubleclick()
{
//	xx-=x;
//	yy-=y;
	if (!visible) return;

	cWindowObject* o = NULL;
	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if (i->second->inObject() && i->second->selectable)
			o = i->second;
	}
	if (o != NULL)
	{
		o->doubleClick();
		selectedObject = o;
	}

}

void cWindow::rightclick()
{
//	xx-=x;
//	yy-=y;
	if (!visible) return;

	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		cWindowObject* o = i->second->inObject();
		if (o != NULL && i->second->selectable)
		{
			selectedObject = o;
			o->rightClick();
			break;
		}
	}

}


cWindowObject* cWindow::addLabel(std::string name, int x, int y, std::string text)
{
	cWindowObject* o = new cWindowLabel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(x,y);
	o->resizeTo(Graphics.WM.font->textlen(text), 12);
	o->setText(0,text);
	objects[name] = o;
	return o;
}


cWindowObject* cWindow::addInputBox(std::string name, int x, int y, int w, std::string text, TiXmlDocument &skin)
{
	cWindowObject* o = new cWindowInputBox(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(x,y);
	o->resizeTo(w, 20);
	o->setText(0,text);
	objects[name] = o;
	return o;
}

cWindowObject* cWindow::addCheckBox(std::string name, int x, int y, bool checked, TiXmlDocument &skin)
{
	cWindowObject* o = new cWindowCheckBox(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(x,y);
	o->setInt(0, checked ? 1 : 0);
	objects[name] = o;
	return o;
}


void cWindow::holddragover()
{
	cWindowObject* o = inObject();
	if(o != NULL)
		o->holdDragOver();	
}

void cWindow::dragover()
{
	cWindowObject* o = inObject();
	if(o != NULL)
		o->dragOver();	
}

void cWindow::scrollUp()
{
	if (!visible) return;

	for(objectlist::reverse_iterator i = objects.rbegin(); i != objects.rend(); i++)
	{
		cWindowObject* o = i->second->inObject();
		if (o != NULL && i->second->selectable)
		{
			i->second->scrollUp();
			break;
		}
	}
}

void cWindow::scrollDown()
{
	if (!visible) return;

	for(objectlist::reverse_iterator i = objects.rbegin(); i != objects.rend(); i++)
	{
		cWindowObject* o = i->second->inObject();
		if (o != NULL)
		{
			i->second->scrollDown();
			break;
		}
	}
}


void cWindow::save()
{
#ifndef __NOXML__
	if(saveProperties != "")
	{
		std::pair<char*,bool*> elements[] = {
			std::pair<char*,bool*>("x", &movable),
			std::pair<char*,bool*>("y", &movable),
			std::pair<char*,bool*>("w", &resizable),
			std::pair<char*,bool*>("h", &resizable) };

		if(config.FirstChildElement("config")->FirstChildElement("wm") == NULL)
			config.FirstChildElement("config")->InsertEndChild(TiXmlElement("wm"));
		if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str()) == NULL)
			config.FirstChildElement("config")->FirstChildElement("wm")->InsertEndChild(TiXmlElement(saveProperties.c_str()));

		for(int i = 0; i < 4; i++)
		{
			if(*elements[i].second && config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement(elements[i].first) == NULL)
			{
				config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->InsertEndChild(TiXmlElement(elements[i].first));
				config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement(elements[i].first)->InsertEndChild(TiXmlText(""));
			}
		}

		char buf[10];
		if(movable)
		{
			sprintf(buf, "%i", x);
			config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement("x")->FirstChild()->SetValue(buf);

			sprintf(buf, "%i", y);
			config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement("y")->FirstChild()->SetValue(buf);
		}
		if(resizable)
		{
			sprintf(buf, "%i", h);
			config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement("h")->FirstChild()->SetValue(buf);

			sprintf(buf, "%i", w);
			config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(saveProperties.c_str())->FirstChildElement("w")->FirstChild()->SetValue(buf);
		}
		config.SaveFile(configfile.c_str());

	}
#endif
}

void cWindow::initprops(std::string s)
{
#ifndef __NOXML__
	saveProperties = s;
	if(!config.FirstChildElement("config"))
		return;
	if(!config.FirstChildElement("config")->FirstChildElement("wm"))
		return;

	if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str()))
	{
		if(movable)
		{
			if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("x"))
				x = atoi(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("x")->FirstChild()->Value());
			if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("y"))
				y = atoi(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("y")->FirstChild()->Value());
		}
		if(resizable)
		{
			if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("h"))
				h = atoi(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("h")->FirstChild()->Value());
			if(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("w"))
				w = atoi(config.FirstChildElement("config")->FirstChildElement("wm")->FirstChildElement(s.c_str())->FirstChildElement("w")->FirstChild()->Value());
		}

		if(movable)
		{
			if(x > Graphics.w())
				x = Graphics.w() - w;
			if(y > Graphics.h())
				y = Graphics.h() - h;
			if(x < 0)
				x = 0;
			if(y < 0)
				y = 0;
		}
	}
#endif
}

cWindow::~cWindow()
{
	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if(draggingObject == i->second)
			draggingObject = NULL;
		delete i->second;
	}
	
	objects.clear();
	// delete objects
}