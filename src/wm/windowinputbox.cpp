#include "windowinputbox.h"
#include "window.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL/SDL.h>

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

#include <font.h>
#include <common.h>
#include <graphics.h>

void cWindowInputBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realX();
	yy = realY();

	cWindowObject::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);

	glDisable(GL_TEXTURE_2D);

	std::string temptext = text;
	if (mask != "")
	{
		char buf[255];
		ZeroMemory(buf, 255);
		memset(buf, (char)mask[0], text.length());
		text = buf;
	}

	std::string t = text;
	if (startindex > (int)t.length())
		startindex = 0;
	t = t.substr(startindex);
	int l = t.size();
	while(l > 0 && parent->font->textLen(t.substr(0, l)) > w-5)
		l--;
	t.resize(l);


	float len = 0;
	float xoff = 0;
	if (cursor > selectionstart)
	{
		len = parent->font->textLen(text.substr(max(selectionstart, startindex), cursor-selectionstart-(startindex > selectionstart ? startindex : 0)));
		xoff = parent->font->textLen(text.substr(startindex, selectionstart-startindex))+5;
		if (selectionstart < startindex)
			xoff = 5;
	}
	if (cursor < selectionstart)
	{
 		len = parent->font->textLen(text.substr(cursor, min(selectionstart-cursor, (int)t.length()-(cursor-startindex))));
		xoff = parent->font->textLen(text.substr(startindex, cursor-startindex))+5;
	}

	if (focussed)
	{
		glColor3f(0.7f,0.7f,colors[3]);
		glBegin(GL_QUADS);
			glVertex2d(xx+xoff, yy+3);
			glVertex2d(xx+xoff+len, yy+3);
			glVertex2d(xx+xoff+len,yy+h-3);
			glVertex2d(xx+xoff, yy+h-3);
		glEnd();
	}
	glColor4f(0,0,0,colors[3]);

	if (mask == "")
		parent->font->print(fontcolor[0], fontcolor[1], fontcolor[2], parent->getX()+xx+5, parent->getY()+yy+2, "%s", t.c_str());
	else
	{
		parent->font->print(fontcolor[0], fontcolor[1], fontcolor[2], parent->getX()+xx+5, parent->getY()+yy+2, t.c_str());
	}
	if (editable && focussed && (SDL_GetTicks() % 1000) > 500)
		parent->font->print(fontcolor[0], fontcolor[1], fontcolor[2],parent->getX()+2+xx+parent->font->textLen(t.substr(0, cursor-startindex)), parent->getY()+yy+2, "|");

	text = temptext;

	glColor4fv(colors);
}


void cWindowInputBox::onClick()
{
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int i = startindex;
	while(xx > 0 && i <= (int)text.length())
	{
		xx -= parent->font->width[(int)text[i]-32];
		i++;
	}

	selectionstart = (cursor = min(max(i-1, 0), (int)text.length()));
}


void cWindowInputBox::onDoubleClick()
{
	selectionstart = text.length();
	cursor = 0;
	while(cursor < startindex)
		startindex = max(0, cursor-4);
	while(parent->font->textLen(text.substr(startindex, cursor-startindex)) > w-10)
		startindex = min(startindex + 4, (int)text.length());
	if (selectionstart > (int)text.length())
		selectionstart = cursor;
}

bool cWindowInputBox::onKeyDown(int keyid, bool shift)
{
	if (editable)
	{
		bool ret = false;
		if (keyid == SDLK_LEFT)
		{
			if (cursor > 0)
				cursor--;
			if (!shift)
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_RIGHT)
		{
			if (cursor < (int)text.length())
				cursor++;
			if (!shift)
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_END)
		{
			cursor = text.length();
			if (!shift)
				selectionstart = cursor;
			ret = true;
		}
		if (keyid == SDLK_HOME)
		{
			cursor = 0;
			if (!shift)
				selectionstart = cursor;
			ret = true;
		}		

		if (keyid == SDLK_BACKSPACE)//backspace
		{
			if (cursor != selectionstart)
			{
				text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
				cursor = min(selectionstart, cursor);
				selectionstart = cursor;
			}
			else if (cursor != 0)
			{
				text = text.substr(0, cursor-1) + text.substr(cursor);
				cursor--;
			}
			selectionstart = cursor;
			onChange();
			ret = true;
		}
		if (keyid == SDLK_DELETE)//delete
		{
			if (cursor != selectionstart)
			{
				text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
				cursor = min(selectionstart, cursor);
			}
			if (cursor != (int)text.length())
			{
				text = text.substr(0, cursor) + text.substr(cursor+1);
			}
			onChange();
			ret = true;
		}
		while(cursor < startindex)
			startindex = max(0, cursor-4);
		while(parent->font->textLen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());
		if (selectionstart > (int)text.length())
			selectionstart = cursor;
		if (ret)
			return true;

	}		
	return false;
}

bool cWindowInputBox::onChar(char keyid, bool shift)
{
	if (keyid > 27 && editable)
	{
		text = text.substr(0, min(cursor, selectionstart)) + text.substr(max(cursor, selectionstart), 10000);
		cursor = min(selectionstart, cursor);
		text = text.substr(0, cursor) + (char)keyid + text.substr(cursor);
		cursor++;
		while(cursor < startindex)
			startindex = max(0, cursor-4);
		while(parent->font->textLen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());

		while(parent->font->textLen(text.substr(startindex, cursor-startindex)) > w-10)
			startindex = min(startindex + 4, (int)text.length());

		if (!shift)
			selectionstart = cursor;
		selectionstart = cursor;
		onChange();
		return true;
	}
	selectionstart = cursor;
	return false;
}


void cWindowInputBox::setText(int i, std::string t)
{
	if (i == 0)
		text = t;
	if (i == 1)
		mask = t;
}

void cWindowInputBox::setInt(int id, int val)
{
	if (id == 0)
		editable = (val == 1);
	if (id == 1)
		cursor = val;
	if (id == 2)
		selectionstart = val;
}

std::string cWindowInputBox::getText(int id)
{
	return text;
}

int cWindowInputBox::getInt(int id)
{
	if (id == 0)
		return editable ? 1 : 0;
	if (id == 1)
		return cursor;
	if (id == 2)
		return selectionstart;
	return cursor;
}

cWindowInputBox::cWindowInputBox( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent, skin->FirstChildElement("skin")->FirstChildElement("input"))
{
	w = 100;
	h = 20;
	x = 10;
	y = 10;
	alignment = ALIGN_CENTER;
	text = "Default";
	startindex = 0;
	cursor = 0;
	editable = true;		
	selectionstart = cursor;
	selectable = true;
	type = OBJECT_INPUTBOX;
	
	if(skin->FirstChildElement("skin")->FirstChildElement("input")->FirstChildElement("defaultheight"))
	{
		h = atoi(skin->FirstChildElement("skin")->FirstChildElement("input")->FirstChildElement("defaultheight")->FirstChild()->Value());
	}
}

cWindowFloatInputBox::cWindowFloatInputBox( cWindow* parent, TiXmlDocument* skin ) : cWindowInputBox(parent, skin)
{
	type = OBJECT_FLOATINPUTBOX;
	alignment = ALIGN_TOPLEFT;
	resizeTo(70,20);
	firstTime = true;
}

void cWindowFloatInputBox::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	if(*floatje != lastvalue || firstTime)
	{
		firstTime = false;
		char buf[100];
		sprintf(buf, "%f", *floatje);
		while(buf[strlen(buf)-1] == '0')
			buf[strlen(buf)-1] = '\0';
		if(buf[strlen(buf)-1] == '.')
			buf[strlen(buf)-1] = '\0';
		text = buf;
		lastvalue = *floatje;
	}
	cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}

void cWindowFloatInputBox::setInt( int id, int val )
{
	cWindowInputBox::setInt(id,val);
	if (id == 3)
	{
		Log(3,0,"Use of depricated method, do not use!");
		floatje = (float*)val;
	}
}

bool cWindowFloatInputBox::onKeyDown( int keyid, bool shift )
{
	bool ret = cWindowInputBox::onKeyDown(keyid, shift);
	if (keyid == SDLK_RETURN)
	{
		*floatje = (float)atof(text.c_str());
		ret = true;
	}
	return ret;
}

cWindowStringInputBox::cWindowStringInputBox( cWindow* parent, TiXmlDocument* skin ) : cWindowInputBox(parent,skin)
{
	type = OBJECT_FLOATINPUTBOX;
	alignment = ALIGN_TOPLEFT;
	resizeTo(70,20);
	firstTime = true;
}

void cWindowStringInputBox::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	if(*stringetje != lastvalue || firstTime)
	{
		firstTime = false;
		char buf[255];
		sprintf(buf, "%s", stringetje->c_str());
		while(buf[strlen(buf)-1] == '0')
			buf[strlen(buf)-1] = '\0';
		if(buf[strlen(buf)-1] == '.')
			buf[strlen(buf)-1] = '\0';
		text = buf;
		lastvalue = *stringetje;
	}
	cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}

bool cWindowStringInputBox::onKeyDown( int keyid, bool shift )
{
	bool ret = cWindowInputBox::onKeyDown(keyid, shift);
	if (keyid == SDLK_RETURN)
	{
		*stringetje = text;
		ret = true;
	}
	return ret;
}

void cWindowStringInputBox::setInt( int id, int val )
{
	cWindowInputBox::setInt(id,val);
	if (id == 3)
		stringetje = (std::string*)val;
}

void cWindowLongInputBox::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	if(*longje != lastvalue || firstdraw)
	{
		firstdraw = false;
		char buf[100];
		sprintf(buf, "%i", *longje);
		while(buf[strlen(buf)-1] == '0')
			buf[strlen(buf)-1] = '\0';
		if(buf[strlen(buf)-1] == '.')
			buf[strlen(buf)-1] = '\0';
		text = buf;
		lastvalue = *longje;
	}
	cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}

cWindowLongInputBox::cWindowLongInputBox( cWindow* parent, TiXmlDocument* skin ) : cWindowInputBox(parent, skin)
{
	type = OBJECT_FLOATINPUTBOX;
	alignment = ALIGN_TOPLEFT;
	resizeTo(70,20);
	firstdraw = true;
}

bool cWindowLongInputBox::onKeyDown( int keyid, bool shift )
{
	bool ret = cWindowInputBox::onKeyDown(keyid, shift);
	if (keyid == SDLK_RETURN)
	{
		*longje = atol(text.c_str());
		ret = true;
	}
	return ret;
}

void cWindowLongInputBox::setInt( int id, int val )
{
	cWindowInputBox::setInt(id,val);
	if (id == 3)
	{
		Log(3,0,"Use of depricated method, do not use!");
		longje = (long*)val;
	}
}