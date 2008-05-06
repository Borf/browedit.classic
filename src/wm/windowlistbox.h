#ifndef __WINDOWLISTBOX_H__
#define __WINDOWLISTBOX_H__

#include "windowobject.h"
#include <string>
#include <tinyxml/tinyxml.h>
#include <vector>
using namespace std;

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
	vector<string>	values;
	vector<int>		properties;
	cWindowListBox(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent,skin.FirstChildElement("skin")->FirstChildElement("list"))
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
		TiXmlElement* bSkin = skin.FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("scroll");

		string scolor = skin.FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("selectcolor")->FirstChild()->Value();
		selectColor[0] = hex2dec(scolor.substr(0,2)) / 256.0f;
		selectColor[1] = hex2dec(scolor.substr(2,2)) / 256.0f;
		selectColor[2] = hex2dec(scolor.substr(4,2)) / 256.0f;

		scolor = skin.FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("selectfontcolor")->FirstChild()->Value();
		selectFontColor[0] = hex2dec(scolor.substr(0,2)) / 256.0f;
		selectFontColor[1] = hex2dec(scolor.substr(2,2)) / 256.0f;
		selectFontColor[2] = hex2dec(scolor.substr(4,2)) / 256.0f;


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
	virtual ~cWindowListBox() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void onChange(int) {};
	bool onkeydown(int, bool);
//	void onchar(char);

	void SetText(int, string);

	void drag();
	void doubleclick();
	void SetInt(int, int);
	int GetInt(int);
	string GetText(int);
	void scrollup();
	void scrolldown();
};




#endif
