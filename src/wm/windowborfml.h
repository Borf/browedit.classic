#ifndef __WINDOWBORFML_H__
#define __WINDOWBORFML_H__

#include <tinyxml/tinyxml.h>
#include "windowobject.h"
#include <string>
#include <vector>
#include <map>
#include <list>
#include <texture.h>
using namespace std;

class cWindowBorfMLBox : public cWindowObject
{
protected:
	class cArea
	{
	public:
		cVector2 tl;
		cVector2 br;

		cArea(cVector2 v1, cVector2 v2)
		{
			tl = v1;
			br = v2;
		}
	};


public:
	void (*clickoption) (int, void*);
	void* clickparam;

	void calcinner();
	cVector2 drawnode(cVector2, TiXmlNode*, int&, list<cArea>&, cVector3 = cVector3(0.2f, 0.2f, 0.2f), cVector2 = cVector2(1,1), bool = true);
	TiXmlDocument body;
	cWindowObject* draggingobject;
	int innerheight;
	int innerwidth;

	int scrollposx;
	int scrollposy;

	cWindowBorfMLBox(cWindow* parent) : cWindowObject(parent)
	{
		w = 100;
		h = 25;
		x = 10;
		y = 10;
		alignment = ALIGN_CENTER;
		cursortype = 0;
		selectable = true;
		type = OBJECT_BMLBOX;
		innerheight = 1000;
		innerwidth = 1000;

		scrollposx = 0;
		scrollposy = 0;
		draggingobject = NULL;
	}
	~cWindowBorfMLBox();
	void draw(int,int,int,int);
	void drag();
	void click();
	cWindowObject* inobject();

	string ppopup();

	vector<cTexture*> textures;

	void SetText(int, string);
	vector<pair<cArea, int> > options;

	void scrollup();
	void scrolldown();
};




#endif
