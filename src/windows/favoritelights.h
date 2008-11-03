#ifndef __FAVORITELIGHTSWINDOW_H__
#define __FAVORITELIGHTSWINDOW_H__

#include <wm/window.h>

#include <menu.h>
#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowtree.h>
#include <wm/windowinputbox.h>
#include "objectwindow.h"
#include <wm/windowcheckbox.h>
extern TiXmlDocument favoritelights;
extern cMenu* popupmenu;
#include <menucommands.h>



class cFavoriteLightsWindow : public cWindow
{
public:
	class cFavoriteTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cFavoriteTreeNode();
		cFavoriteTreeNode(std::string t);
		cFavoriteTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items);

		std::vector<int> keys;
		bool			isCat;

	};

	
	static void addlights(cFavoriteTreeNode* parent, TiXmlNode* n);


	class cFavoritesTree : public cWindowTree
	{
	public:
		cFavoritesTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin = &cWM::skin);

		void onRightClick();
		void onChange();
	};


	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		virtual ~cWindowCancelButton();
		void onClick();
	};


	std::vector<int> currentkey;
	cFavoriteLightsWindow();	
};

#endif
