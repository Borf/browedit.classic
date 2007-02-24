#ifndef __WINDOWSCROLLPANEL_H__
#define __WINDOWSCROLLPANEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowScrollPanel : public cWindowObject
{
protected:
public:
	vector<cWindowObject*>	objects;
	cWindowObject* draggingobject;
	int innerheight;
	int innerwidth;

	int scrollposx;
	int scrollposy;

	cWindowScrollPanel(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		cursortype = 0;
		selectable = true;
		type = OBJECT_BUTTON;
		innerheight = 1000;
		innerwidth = 1000;

		scrollposx = 0;
		scrollposy = 0;
		draggingobject = NULL;
	}
	~cWindowScrollPanel();
	void draw();
	void drag();
	void click();
	bool onkeyup(int);
	bool onkeydown(int);
	bool onchar(char);
	void doubleclick();
	void rightclick();

	string ppopup();

};




#endif
