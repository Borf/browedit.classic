#ifndef __WINDOWBUTTON_H__
#define __WINDOWBUTTON_H__

#include "windowobject.h"
#include <string>
#include <wm/wm.h>
//using namespace std;

class cWindowButton : public cWindowObject
{
protected:
	std::string text;


public:
	cWindowButton(cWindow* parent, Json::Value &skin = cWM::skin);
	virtual ~cWindowButton() {}
	virtual void draw(int,int,int,int);
	virtual void onClick() = 0;
	virtual bool onKeyDown(int, bool);

	void setText(int id, std::string txt);


};


class cWindowToggleButton : public cWindowButton
{
public:
	bool pushed;
	bool* linkedValue;

	cWindowToggleButton(cWindow* parent, Json::Value &skin = cWM::skin);
	cWindowToggleButton(cWindow* parent, bool* linked);
	virtual void onClick();
	virtual void draw(int,int,int,int);
};

class cWindowCloseButton : public cWindowObject
{
public:
	cWindowCloseButton(cWindow* parent, Json::Value &skin = cWM::skin);
	void onClick();
};

class cWindowRollupButton : public cWindowObject
{
public:
	cWindowRollupButton(cWindow* parent, Json::Value &skin = cWM::skin);
	void onClick();
};



#endif
