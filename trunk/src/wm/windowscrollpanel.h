#ifndef __WINDOWSCROLLPANEL_H__
#define __WINDOWSCROLLPANEL_H__

#include <wm/wm.h>
#include "windowobject.h"
#include <string>
#include <vector>
//using namespace std;

class cWindowScrollPanel : public cWindowObject
{
protected:
public:
	std::vector<cWindowObject*>	objects;
	cWindowObject* draggingObject;
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
	cWindowScrollPanel(cWindow* parent, Json::Value &skin = cWM::skin);
	~cWindowScrollPanel();
	void draw(int,int,int,int);
	void drag();
	void onClick();
	bool onKeyUp(int, bool);
	bool onKeyDown(int, bool);
	bool onChar(char, bool);
	void onDoubleClick();
	void onRightClick();
	cWindowObject* inObject();

	void onScrollUp();
	void onScrollDown();

	std::string getPopup();

};




#endif
