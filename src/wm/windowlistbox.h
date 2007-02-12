#ifndef __WINDOWLISTBOX_H__
#define __WINDOWLISTBOX_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowListBox : public cWindowObject
{
protected:
	vector<string>	values;
	int				liststart;
	int				selected;
	vector<int>		properties;
	bool			showselection;
	bool			showscroll;
public:
	cWindowListBox()
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
	virtual void draw();
	virtual void click();
	bool onkeydown(int);
//	void onchar(char);

	void SetText(int, string);

	void drag();
	void doubleclick();
	void SetInt(int, int);
	int GetInt(int);
};




#endif
