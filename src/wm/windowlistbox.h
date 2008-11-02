#ifndef __WINDOWLISTBOX_H__
#define __WINDOWLISTBOX_H__

#include "windowobject.h"
#include <string>
#include <tinyxml/tinyxml.h>
#include <vector>
//using namespace std;

class cWindowListBox : public cWindowObject
{
protected:
	int				liststart;
	int				selected;
	bool			showselection;
	bool			showscroll;

	int skinBarWidth;
	int skinButtonUpLeft;
	int skinButtonUpTop;
	int skinButtonUpHeight;
	int skinButtonDownLeft;
	int skinButtonDownTop;
	int skinButtonDownHeight;

	int	skinBarTopHeight;
	int skinBarTop;
	int skinBarBottomHeight;
	int skinBarBottom;
	int skinBarCenterHeight;

	int skinBarBackTop;
	int skinBarBackHeight;
	int skinBarBackLeft;

	float selectColor[3];
	float selectFontColor[3];


	int skinBarLeft;


public:
	std::vector<std::string>	values;
	std::vector<int>			properties;
	cWindowListBox(cWindow* parent, TiXmlDocument* skin = NULL);
	virtual ~cWindowListBox() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void onChange(int) {};
	bool onKeyDown(int, bool);
//	void onchar(char);

	void setText(int, std::string);

	void drag();
	void doubleClick();
	void setInt(int, int);
	int getInt(int);
	std::string getText(int);
	void scrollUp();
	void scrollDown();
};

template <class T> class cWindowDataListBox : public cWindowListBox
{
public:
	std::vector<T>	data;
	cWindowDataListBox(cWindow* parent, TiXmlDocument* skin = NULL) : cWindowListBox(parent,skin)
	{
	}
};


#endif
