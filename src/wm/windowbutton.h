#ifndef __WINDOWBUTTON_H__
#define __WINDOWBUTTON_H__

#include "windowobject.h"
#include <string>
#include <tinyxml/tinyxml.h>
//using namespace std;

class cWindowButton : public cWindowObject
{
protected:
	std::string text;


public:
	cWindowButton(cWindow* parent, TiXmlDocument &skin);
	virtual ~cWindowButton() {}
	virtual void draw(int,int,int,int);
	virtual void click() = 0;
	virtual bool onKeyDown(int, bool);

	void setText(int id, std::string txt);


};


#include "window.h"

class cWindowCloseButton : public cWindowObject
{
public:
	cWindowCloseButton(cWindow* parent, TiXmlDocument &skin);
	void click()
	{
		parent->close();
	}
};

class cWindowRollupButton : public cWindowObject
{
public:
	cWindowRollupButton(cWindow* parent, TiXmlDocument &skin);
	void click();
};



#endif
