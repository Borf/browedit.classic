#ifndef __WINDOWCHECKBOX_H__
#define __WINDOWCHECKBOX_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowCheckBox : public cWindowObject
{
protected:
	bool value;	
public:
	cWindowCheckBox(cWindow* parent) : cWindowObject(parent)
	{
		w = 12;
		h = 12;
		x = 40;
		y = 40;
		alignment = ALIGN_CENTER;
		value = true;
		type = OBJECT_CHECKBOX;
	}
	virtual void draw(int,int,int,int);
	virtual void click();
	bool onchar(char, bool);
	void SetInt(int, intptr_t);
	intptr_t GetInt(int);
	string GetText(int);
};



class cWindowBoolCheckBox : public cWindowCheckBox
{
	bool* boolvalue;
public:
	cWindowBoolCheckBox(cWindow* parent) : cWindowCheckBox(parent)
	{
		alignment = ALIGN_TOPLEFT;
		boolvalue = NULL;
	}
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
	{
		if(boolvalue != NULL)
			*boolvalue = value;
		cWindowCheckBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	}

	void SetInt(int id, intptr_t val)
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
