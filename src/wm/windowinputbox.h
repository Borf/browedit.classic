#ifndef __WINDOWINPUTBOX_H__
#define __WINDOWINPUTBOX_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowInputBox : public cWindowObject
{
protected:
	string text;
	int cursor;
	int startindex;
	string mask;
	bool editable;
	int selectionstart;

public:
	cWindowInputBox(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("input"))
	{
		w = 100;
		h = 25;
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
	
	}
	virtual ~cWindowInputBox() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	bool onkeydown(int,bool);
	bool onchar(char,bool);

	void SetText(int, string);
	void SetInt(int, int);
	int GetInt(int);
	string GetText(int);
};


class cWindowFloatInputBox : public cWindowInputBox
{
	float lastvalue;
public:
	float* floatje;
	cWindowFloatInputBox(cWindow* parent, TiXmlDocument &skin) : cWindowInputBox(parent, skin)
	{
		type = OBJECT_FLOATINPUTBOX;
		alignment = ALIGN_TOPLEFT;
		resizeto(70,20);
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		if(*floatje != lastvalue)
		{
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

	void SetInt(int id, int val)
	{
		cWindowInputBox::SetInt(id,val);
		if (id == 3)
		{
			Log(3,0,"Use of depricated method, do not use!");
			floatje = (float*)val;
		}
	}
	bool onkeydown(int keyid, bool shift)
	{
		bool ret = cWindowInputBox::onkeydown(keyid, shift);
		if (keyid == SDLK_RETURN)
		{
			*floatje = atof(text.c_str());
			ret = true;
		}
		return ret;
	}
};

class cWindowStringInputBox : public cWindowInputBox
{
	string lastvalue;
public:
	string* stringetje;
	cWindowStringInputBox(cWindow* parent, TiXmlDocument &skin) : cWindowInputBox(parent,skin)
	{
		type = OBJECT_FLOATINPUTBOX;
		alignment = ALIGN_TOPLEFT;
		resizeto(70,20);
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		if(*stringetje != lastvalue)
		{
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

	void SetInt(int id, int val)
	{
		cWindowInputBox::SetInt(id,val);
		if (id == 3)
			stringetje = (string*)val;
	}
	bool onkeydown(int keyid, bool shift)
	{
		bool ret = cWindowInputBox::onkeydown(keyid, shift);
		if (keyid == SDLK_RETURN)
		{
			Log(3,0,"Use of depricated method, do not use!");
			*stringetje = text;
			ret = true;
		}
		return ret;
	}
};

#endif
