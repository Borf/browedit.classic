#ifndef __WINDOWINPUTBOX_H__
#define __WINDOWINPUTBOX_H__

#include "windowobject.h"
#include <string>
using namespace std;

class cWindowInputBox : public cWindowObject
{
protected:
	string text;
	int cursor;
	int startindex;
	string mask;
	bool editable;
	int selectionstart;
public:
	cWindowInputBox(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		text = "Default";
		startindex = 0;
		cursor = 0;
		editable = true;		
		selectionstart = cursor;
		selectable = true;
		type = OBJECT_INPUTBOX;
	}
	virtual ~cWindowInputBox() {}
	virtual void draw();
	virtual void click();
	bool onkeydown(int);
	bool onchar(char);

	void SetText(int, string);
	void SetInt(int, int);
	int GetInt(int);
	string GetText(int);
};




#endif
