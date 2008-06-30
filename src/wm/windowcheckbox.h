#ifndef __WINDOWCHECKBOX_H__
#define __WINDOWCHECKBOX_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowCheckBox : public cWindowObject
{
protected:
	bool value;
	
	int skinCheckedLeft;
	int skinCheckedTop;
	int skinUncheckedLeft;
	int skinUncheckedTop;


public:
	cWindowCheckBox(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent)
	{
		x = 40;
		y = 40;
		alignment = ALIGN_CENTER;
		value = true;
		type = OBJECT_CHECKBOX;

		TiXmlElement* bSkin = skin.FirstChildElement("skin")->FirstChildElement("checkbox");
		
		w = atoi(bSkin->FirstChildElement("width")->FirstChild()->Value());
		h = atoi(bSkin->FirstChildElement("height")->FirstChild()->Value());

		skinCheckedLeft = atoi(bSkin->FirstChildElement("checked")->FirstChildElement("left")->FirstChild()->Value());
		skinCheckedTop = 512-atoi(bSkin->FirstChildElement("checked")->FirstChildElement("top")->FirstChild()->Value());
		skinUncheckedLeft = atoi(bSkin->FirstChildElement("unchecked")->FirstChildElement("left")->FirstChild()->Value());
		skinUncheckedTop = 512-atoi(bSkin->FirstChildElement("unchecked")->FirstChildElement("top")->FirstChild()->Value());
		

	}
	virtual void draw(int,int,int,int);
	virtual void click();
	bool onchar(char, bool);
	void SetInt(int, int);
	int GetInt(int);
	string GetText(int);
};



class cWindowBoolCheckBox : public cWindowCheckBox
{
	bool firstTime;
public:
	bool* boolvalue;
	cWindowBoolCheckBox(cWindow* parent, TiXmlDocument &skin) : cWindowCheckBox(parent,skin)
	{
		alignment = ALIGN_TOPLEFT;
		boolvalue = NULL;
		firstTime = true;
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		if(boolvalue != NULL && firstTime)
		{
			value = *boolvalue;
			firstTime = false;
		}
		*boolvalue = value;
		cWindowCheckBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	}

	void SetInt(int id, int val)
	{
		cWindowCheckBox::SetInt(id,val);
		if (id == 3)
		{
			boolvalue = (bool*)val;
			value  = *boolvalue;
		}
	}
};




#endif
