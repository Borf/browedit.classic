#ifndef __WINDOWMAINBUTTON_H__
#define __WINDOWMAINBUTTON_H__

#include "windowobject.h"
#include "windowbutton.h"
#include "window.h"

class cWindowMainButton : public cWindowButton
{
protected:
	std::string text;
public:
	cWindowMainButton(cWindow* parent, Json::Value &skin);
	void draw(int,int,int,int);
	void onClick();
	void setText(int, std::string);
	std::string getText(int);
};


#endif
