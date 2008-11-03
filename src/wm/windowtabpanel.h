#ifndef __WINDOWTABPANEL_H__
#define __WINDOWTABPANEL_H__

#include <wm/wm.h>
#include "windowobject.h"
#include <string>
#include <vector>

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
	std::vector<std::string>	tabs;
	int selectedtab;


	cWindowTabPanel(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
	void draw(int,int,int,int);
	void onClick();

	int getInt(int id);

	virtual void tabchange(int ) {};

};




#endif
