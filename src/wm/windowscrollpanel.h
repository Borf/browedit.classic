#ifndef __WINDOWSCROLLPANEL_H__
#define __WINDOWSCROLLPANEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowScrollPanel : public cWindowObject
{
protected:
public:
	vector<cWindowObject*>	objects;
	cWindowObject* draggingobject;
	int innerheight;
	int innerwidth;

	int scrollposx;
	int scrollposy;

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

	int skinBarBackTop;
	int skinBarBackHeight;
	int skinBarBackLeft;
	int skinBarCenterHeight;


	int skinBarLeft;
	cWindowScrollPanel(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("list"))
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		cursortype = 0;
		selectable = true;
		type = OBJECT_BUTTON;
		innerheight = 1000;
		innerwidth = 1000;

		scrollposx = 0;
		scrollposy = 0;
		draggingobject = NULL;
	
		TiXmlElement* bSkin = skin.FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("scroll");

		skinBarWidth =			atoi(bSkin->FirstChildElement("width")->FirstChild()->Value());
		skinBarLeft =			atoi(bSkin->FirstChildElement("left")->FirstChild()->Value());
		skinButtonUpLeft =		atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("left")->FirstChild()->Value());
		skinButtonUpTop =		512-atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("top")->FirstChild()->Value());
		skinButtonUpHeight =	atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("height")->FirstChild()->Value());
		skinButtonDownLeft =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("left")->FirstChild()->Value());
		skinButtonDownTop =		512-atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("top")->FirstChild()->Value());
		skinButtonDownHeight =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("height")->FirstChild()->Value());

		skinBarTopHeight = atoi(bSkin->FirstChildElement("top")->Attribute("height"));
		skinBarTop =		512 - atoi(bSkin->FirstChildElement("top")->FirstChild()->Value());
		skinBarBottomHeight = atoi(bSkin->FirstChildElement("bottom")->Attribute("height"));
		skinBarBottom =		512 - atoi(bSkin->FirstChildElement("bottom")->FirstChild()->Value());

		skinBarBackTop=		512-atoi(bSkin->FirstChildElement("background")->FirstChildElement("top")->FirstChild()->Value());
		skinBarBackHeight =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("height")->FirstChild()->Value());
		skinBarBackLeft =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("left")->FirstChild()->Value());
		skinBarCenterHeight = atoi(bSkin->FirstChildElement("centerheight")->FirstChild()->Value());

	}
	~cWindowScrollPanel();
	void draw(int,int,int,int);
	void drag();
	void click();
	bool onkeyup(int, bool);
	bool onkeydown(int, bool);
	bool onchar(char, bool);
	void doubleclick();
	void rightclick();
	cWindowObject* inobject();

	void scrollup();
	void scrolldown();

	string ppopup();

};




#endif
