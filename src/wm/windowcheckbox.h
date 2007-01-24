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
	cWindowCheckBox()
	{
		w = 12;
		h = 12;
		x = 40;
		y = 40;
		alignment = ALIGN_CENTER;
		value = true;
		type = OBJECT_CHECKBOX;
	}
	virtual void draw();
	virtual void click();
	bool onchar(char);
	void SetInt(int, int);
	int GetInt(int);
};




#endif
