#ifndef __WINDOWPROGRESSBAR_H__
#define __WINDOWPROGRESSBAR_H__

#include "windowobject.h"
#include <string>
#include <vector>
#include <texture.h>
using namespace std;

class cWindowProgressBar: public cWindowObject
{
protected:
	int min;
	int max;
	int value;
	bool gradcolors;
public:
	cWindowProgressBar(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 10;
		x = 5;
		y = 20;
		alignment = ALIGN_TOPLEFT;
		type = OBJECT_PROGRESSBAR;
		min = 0;
		max = 100;
		value = 33;
		gradcolors = true;
	}
	virtual ~cWindowProgressBar() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual string ppopup();

	void SetInt(int, int);
	int GetInt(int);
};




#endif
