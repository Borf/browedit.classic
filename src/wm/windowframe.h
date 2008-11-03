#ifndef __WINDOWFRAME_H__
#define __WINDOWFRAME_H__

#include "windowobject.h"
#include <string>
#include <wm/wm.h>
//using namespace std;

class cWindowFrame : public cWindowObject
{
	bool drawinner;
public:
	cWindowFrame(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
	virtual void draw(int,int,int,int);
	void setInt(int,int);
};




#endif
