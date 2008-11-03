#ifndef __WINDOWTREE_H__
#define __WINDOWTREE_H__

#include "windowobject.h"
#include <string>
#include <vector>
#include <wm/wm.h>
//using namespace std;

class cWindowTree : public cWindowObject
{
public:
	class cTreeNode
	{
	public:
		cTreeNode();
		cTreeNode(std::string t);
		cTreeNode(std::string t, std::vector<cTreeNode*> items);

		bool open;
		cTreeNode* parent;
		std::string text;
		std::vector<cTreeNode*> children;
		
		void getdata(std::vector<std::string>&, int = 0);
		bool haschild(std::string);
		cTreeNode* getnode(int&);
		cTreeNode* getnode(std::string);
		int getselectionnr(cTreeNode* = NULL);
		void getnodes(std::vector<cTreeNode*> &);

		void addchild(cTreeNode*);
		int openchildcount();
		void sort();
		int id;
	};


	int							liststart;
	int							selected;
	bool						showselection;
	bool						showscroll;
	std::vector<cTreeNode*>		nodes;
	std::vector<std::string>	values;
	bool						validCache;

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


	cWindowTree(cWindow* parent, std::vector<cTreeNode*> n, TiXmlDocument* skin = &cWM::skin);
	virtual ~cWindowTree() {}
	virtual void draw(int,int,int,int);
	virtual void onClick();
	virtual void onRightClick();
	virtual void onChange() {};
	bool onKeyDown(int, bool);
//	void onchar(char);

	void setText(int, std::string);

	void drag();
	void onDoubleClick();
	void setInt(int, int);
	int getInt(int);
	void onScrollUp();
	void onScrollDown();
};




#endif
