#ifndef __WINDOWFRAME_H__
#define __WINDOWFRAME_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowFrame : public cWindowObject
{
	bool drawinner;
public:
	cWindowFrame(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("frame"))
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		type = OBJECT_FRAME;
		drawinner = false;
	}
	virtual void draw(int,int,int,int);
	void SetInt(int,int);
};




#endif
