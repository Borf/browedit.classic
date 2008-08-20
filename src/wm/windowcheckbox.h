#ifndef __WINDOWCHECKBOX_H__
#define __WINDOWCHECKBOX_H__

#include "windowobject.h"
#include <string>
//using namespace std;

class cWindowCheckBox : public cWindowObject
{
protected:
	bool value;
	
	int skinCheckedLeft;
	int skinCheckedTop;
	int skinUncheckedLeft;
	int skinUncheckedTop;


public:
	cWindowCheckBox(cWindow* parent, TiXmlDocument &skin);
	virtual void draw(int,int,int,int);
	virtual void click();
	bool onChar(char, bool);
	void setInt(int, int);
	int getInt(int);
	std::string getText(int);
};



class cWindowBoolCheckBox : public cWindowCheckBox
{
	bool firstTime;
public:
	bool* boolvalue;
	cWindowBoolCheckBox(cWindow* parent, TiXmlDocument &skin);
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
	void setInt(int id, int val);
};




#endif
