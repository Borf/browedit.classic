#ifndef __WINDOWTABPANEL_H__
#define __WINDOWTABPANEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowTabPanel : public cWindowObject
{
protected:
public:
	vector<string>	tabs;
	int selectedtab;

	cWindowTabPanel(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		cursortype = 0;
		selectable = true;
		type = OBJECT_TABPANEL;
		selectedtab = 0;
		tabs.push_back("tab1");
		tabs.push_back("tab2");
		tabs.push_back("tab3");
		tabs.push_back("tab4");
		tabs.push_back("tab5");
	}
	void draw(int,int,int,int);
	void click();

	int GetInt(int id)
	{
		return selectedtab;
	}

	virtual void tabchange(int ) {};

};




#endif
