#ifndef __WINDOWLABEL_H__
#define __WINDOWLABEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowLabel : public cWindowObject
{
protected:
	string text;
	string lasttext;
	vector<string> lines;
public:
	cWindowLabel(cWindow* parent) : cWindowObject(parent)
	{
		w = 30;
		h = 100;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		text = "Default text that is very long and will wrap";
		selectable = false;
		type = OBJECT_LABEL;
	}
	virtual ~cWindowLabel() {}
	virtual void draw();
	void SetText(int, string);
	string GetText(int);

	int GetInt(int);

};




#endif
