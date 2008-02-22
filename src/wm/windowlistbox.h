#ifndef __WINDOWLISTBOX_H__
#define __WINDOWLISTBOX_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowListBox : public cWindowObject
{
protected:
	int				liststart;
	int				selected;
	bool			showselection;
	bool			showscroll;
public:
	vector<string>	values;
	vector<int>		properties;
	cWindowListBox(cWindow* parent) : cWindowObject(parent)
	{
		w = 280;
		h = 100;
		x = 5;
		y = 20;
		alignment = ALIGN_TOPLEFT;
		liststart = 0;
		selected = 0;
		showselection = true;
		showscroll = true;
		type = OBJECT_LISTBOX;
	}
	virtual ~cWindowListBox() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	bool onkeydown(int, bool);
//	void onchar(char);

	void SetText(int, string);

	void drag();
	void doubleclick();
	void SetInt(int, intptr_t);
	intptr_t GetInt(int);
	string GetText(int);
	void scrollup();
	void scrolldown();
};




#endif
