#ifndef __WINDOWTABPANEL_H__
#define __WINDOWTABPANEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowTabPanel : public cWindowObject
{
protected:

	int skinTabTop[2];
	int skinTabHeight[2];
	int skinTabLeft[2];
	int skinTabLeftWidth[2];
	int skinTabRight[2];
	int skinTabRightWidth[2];
	int skinTabOverlapLeft[2];
	int skinTabOverlapRight[2];
	int skinTabFontOffX, skinTabFontOffY;

public:
	vector<string>	tabs;
	int selectedtab;


	cWindowTabPanel(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("tabstrip"))
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


		skinTabFontOffX = atoi(skin.FirstChildElement("skin")->FirstChildElement("tabstrip")->FirstChildElement("fontoffx")->FirstChild()->Value());
		skinTabFontOffY = atoi(skin.FirstChildElement("skin")->FirstChildElement("tabstrip")->FirstChildElement("fontoffy")->FirstChild()->Value());

		TiXmlElement* el = skin.FirstChildElement("skin")->FirstChildElement("tabstrip")->FirstChildElement("tabselected");
		skinTabTop[0] =		512-atoi(el->FirstChildElement("top")->FirstChild()->Value());
		skinTabHeight[0] =		atoi(el->FirstChildElement("height")->FirstChild()->Value());
		skinTabLeft[0] =		atoi(el->FirstChildElement("left")->FirstChild()->Value());
		skinTabLeftWidth[0] =	atoi(el->FirstChildElement("left")->Attribute("width"));
		skinTabRight[0] =		atoi(el->FirstChildElement("right")->FirstChild()->Value());
		skinTabRightWidth[0] =	atoi(el->FirstChildElement("right")->Attribute("width"));
		skinTabOverlapLeft[0] =	atoi(el->FirstChildElement("overlapleft")->FirstChild()->Value());
		skinTabOverlapRight[0] =atoi(el->FirstChildElement("overlapright")->FirstChild()->Value());

		el = skin.FirstChildElement("skin")->FirstChildElement("tabstrip")->FirstChildElement("tabunselected");
		skinTabTop[1] =		512-atoi(el->FirstChildElement("top")->FirstChild()->Value());
		skinTabHeight[1] =		atoi(el->FirstChildElement("height")->FirstChild()->Value());
		skinTabLeft[1] =		atoi(el->FirstChildElement("left")->FirstChild()->Value());
		skinTabLeftWidth[1] =	atoi(el->FirstChildElement("left")->Attribute("width"));
		skinTabRight[1] =		atoi(el->FirstChildElement("right")->FirstChild()->Value());
		skinTabRightWidth[1] =	atoi(el->FirstChildElement("right")->Attribute("width"));
		skinTabOverlapLeft[1] =	atoi(el->FirstChildElement("overlapleft")->FirstChild()->Value());
		skinTabOverlapRight[1] =atoi(el->FirstChildElement("overlapright")->FirstChild()->Value());
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
