#ifndef __MODELSWINDOW_H__
#define __MODELSWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowtree.h>
#include <wm/windowscrollpanel.h>

#include "../RSMModel.h"


class cModelsWindow : public cWindow
{
public:
	class cWindowModelCatSelect : public cWindowTree
	{
	public:
		int originalselection;
		cWindowModelCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, Json::Value &skin = cWM::skin);
		void onClick();
		void onHoldDragOver();
		void onDragOver();
		void refreshmodels();
		void onRightClick();
		bool onKeyDown(int,bool);
	};
	class cWindowModel : public cWindowObject
	{
		cRsmModel* model;
		std::string data;
	public:
		cWindowModel(cWindow* parent);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void onClick();
		void onRightClick();
		void setText(int i, std::string s);
		std::string getText(int i);
		void drag();
		void setInt(int i, int id);
		~cWindowModel();
	};


	std::map<cWindowTree::cTreeNode*, std::vector<std::pair<std::string, std::string> >, std::less<cWindowTree::cTreeNode*> > items;
	cModelsWindow();
	void resizeTo(int ww, int hh);
	void onStopDrag();
	void* userfunc(void* param);
};

#endif
