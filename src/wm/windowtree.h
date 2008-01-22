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
			for(int i = 0; i < children.size(); i++)
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

	cWindowTree(cWindow* parent, vector<cTreeNode*> n) : cWindowObject(parent)
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
		type = OBJECT_TREE;

		nodes = n;

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
	void SetInt(int, intptr_t);
	intptr_t GetInt(int);
	void scrollup();
	void scrolldown();
};




#endif
