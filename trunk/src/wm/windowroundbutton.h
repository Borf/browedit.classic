#ifndef __WINDOWROUNDBUTTON_H__
#define __WINDOWROUNDBUTTON_H__

#include "windowobject.h"
#include "window.h"

class cWindowRoundButton : public cWindowObject
{
public:
	cWindowRoundButton(cWindow* parent);
	void draw(int,int,int,int);
	void onClick();
	cWindowObject* inObject();
};

#endif
