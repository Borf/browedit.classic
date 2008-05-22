#ifndef __WINDOWTREE_H__
#define __WINDOWTREE_H__

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowTree : public cWindowObject
{
public:
	class cTreeNode
	{
	public:
		cTreeNode() { parent = NULL; open = false; }
		cTreeNode(string t) { text = t; parent = NULL; open = false; }
		cTreeNode(string t, vector<cTreeNode*> items)
		{
			parent = NULL;
			text = t;
			children = items;
			for(unsigned int i = 0; i < children.size(); i++)
				children[i]->parent = this;
			open = false;
		}

		bool open;
		cTreeNode* parent;
		string text;
		vector<cTreeNode*> children;
		
		void getdata(vector<string>&, int = 0);
		bool haschild(string);
		cTreeNode* getnode(int&);
		cTreeNode* getnode(string);
		int getselectionnr(cTreeNode* = NULL);
		void getnodes(vector<cTreeNode*> &);

		void addchild(cTreeNode*);
		int openchildcount();
		void sort();
		int id;
	};


	int				liststart;
	int				selected;
	bool			showselection;
	bool			showscroll;
	vector<cTreeNode*>	nodes;
	vector<string>		values;
	bool				validCache;

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


	int skinBarLeft;
	float selectColor[3];
	float selectFontColor[3];


	cWindowTree(cWindow* parent, vector<cTreeNode*> n, TiXmlDocument &skin) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("list"))
	{
		validCache = false;
		w = 280;
		h = 100;
		x = 5;
		y = 20;
		alignment = ALIGN_TOPLEFT;
		liststart = 0;
		selected = 0;
		showselection = true;
		showscroll = true;
		type = OBJECT_TREE;

		nodes = n;

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
	virtual ~cWindowTree() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void rightclick();
	virtual void onchange() {};
	bool onkeydown(int, bool);
//	void onchar(char);

	void SetText(int, string);

	void drag();
	void doubleclick();
	void SetInt(int, int);
	int GetInt(int);
	void scrollup();
	void scrolldown();
};




#endif
