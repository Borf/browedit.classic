#ifndef __WINDOWRGBPICKER_H__
#define __WINDOWRGBPICKER_H__

#include "windowobject.h"
#include <string>
//using namespace std;

class cWindowRGBPicker : public cWindowObject
{
protected:
	float r,g,b;
public:
	cWindowRGBPicker(cWindow* parent);
	virtual ~cWindowRGBPicker() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void drag();



};




#endif
