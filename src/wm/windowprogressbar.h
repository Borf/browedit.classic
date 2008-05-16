#ifndef __WINDOWPROGRESSBAR_H__
#define __WINDOWPROGRESSBAR_H__

#include "windowobject.h"
#include <string>
#include <vector>
#include <texture.h>
using namespace std;

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

		cProgressSkin(TiXmlElement* el)
		{
			height = atoi(el->FirstChildElement("height")->FirstChild()->Value());
			top = 512-atoi(el->FirstChildElement("top")->FirstChild()->Value());
			leftWidth = atoi(el->FirstChildElement("left")->Attribute("width"));
			left = atoi(el->FirstChildElement("left")->FirstChild()->Value());
			rightWidth = atoi(el->FirstChildElement("right")->Attribute("width"));
			right = atoi(el->FirstChildElement("right")->FirstChild()->Value());

			if(el->Attribute("cond"))
				condition = atoi(el->Attribute("cond"));
			else
				condition = 0;
		}
	};
	
	vector<cProgressSkin> skins;

public:
	cWindowProgressBar(cWindow* parent, TiXmlDocument &skin) : cWindowObject(parent)
	{
		w = 100;
		h = 10;
		x = 5;
		y = 20;
		alignment = ALIGN_TOPLEFT;
		type = OBJECT_PROGRESSBAR;
		min = 0;
		max = 100;
		value = 33;


		TiXmlElement* p = skin.FirstChildElement("skin")->FirstChildElement("progress");
		skinType = (strcmp(p->Attribute("type"), "normal") == 0 ? NORMAL : (strcmp(p->Attribute("type"), "conditional") == 0 ? CONDITIONAL : NORMAL));


		skins.push_back(cProgressSkin(p->FirstChildElement("back")));
		TiXmlElement* el = p->FirstChildElement("bar");
		while(el != NULL)
		{
			skins.push_back(cProgressSkin(el));
			el = el->NextSiblingElement("bar");
		}



	}

	void drawprogress(int,int);
	virtual ~cWindowProgressBar() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual string ppopup();

	void SetInt(int, int);
	int GetInt(int);
};




#endif
