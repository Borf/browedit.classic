#ifndef __WINDOWPROGRESSBAR_H__
#define __WINDOWPROGRESSBAR_H__

#include <wm/wm.h>
#include "windowobject.h"
#include <string>
#include <vector>
#include <texture.h>
//using namespace std;

class cWindowProgressBar: public cWindowObject
{
protected:
	int min;
	int max;
	int value;
	bool gradcolors;

	enum eSkinType
	{
		NORMAL,
		CONDITIONAL,
		GRADIANT,
		GRADIANT2
	};

	eSkinType skinType;

	class cProgressSkin
	{
	public:
		int height;
		int top;
		int leftWidth;
		int left;
		int rightWidth;
		int right;
		int condition;

		cProgressSkin(TiXmlElement* el);
	};
	
	std::vector<cProgressSkin> skins;

public:
	cWindowProgressBar(cWindow* parent, TiXmlDocument* skin = &cWM::skin);

	void drawprogress(int,int);
	virtual ~cWindowProgressBar() {}
	virtual void draw(int,int,int,int);
	virtual void onClick();
	virtual std::string getPopup();

	void setInt(int, int);
	int getInt(int);
};




#endif
