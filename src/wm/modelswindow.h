#ifndef __MODELSWINDOW_H__
#define __MODELSWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowtree.h"
#include "windowscrollpanel.h"

#include "../RSMModel.h"


class cModelsWindow : public cWindow
{
public:
	class cWindowModelCatSelect : public cWindowTree
	{
	public:
		int originalselection;
		cWindowModelCatSelect(cWindow* parent, vector<cWindowTree::cTreeNode*> n, TiXmlDocument &skin);
		void click();
		void holddragover();
		void dragover();
		void refreshmodels();
		void rightclick();
		bool onkeydown(int,bool);
	};
	class cWindowModel : public cWindowObject
	{
		cRSMModel* model;
		string data;
	public:
		cWindowModel(cWindow* parent);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void click();
		void rightclick();
		void SetText(int i, string s);
		string GetText(int i);
		void drag();
		void SetInt(int i, intptr_t id);
		~cWindowModel();
	};


	map<cWindowTree::cTreeNode*, vector<pair<string, string> >, less<cWindowTree::cTreeNode*> > items;
	cModelsWindow(cTexture*, cFont*, TiXmlDocument &);
	void resizeto(int ww, int hh);
	void stopdrag();
	void* userfunc(void* param);
};

#endif
