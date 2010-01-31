#ifndef __SPRITEWINDOW_H__
#define __SPRITEWINDOW_H__

#include <common.h>

#include <wm/window.h>
#include <wm/windowtree.h>
#include <wm/windowbutton.h>
#include <wm/windowtabpanel.h>

class cSprite;


class cSpriteWindow : public cWindow
{
public:
	class cSpriteInfo
	{
	public:
		std::string fileName;
		cSpriteInfo();
		cSpriteInfo(std::string f);
	};

	class cWindowSprite : public cWindowObject
	{
	public:
		cSprite* sprite;
		cWindowSprite(cWindow* parent, Json::Value &skin = cWM::skin);
		void draw(int,int,int,int);
	};

	class cTabPanel : public cWindowTabPanel
	{
	public:
		cTabPanel(cWindow* parent, Json::Value &skin = cWM::skin);
		void tabchange(int oldtab);
//		void addxml(cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode, std::string dir = "");
	};


	class cTree : public cWindowTree
	{
	public:
		cTree(cWindow* parent, std::vector<cTreeNode*> n, Json::Value &skin = cWM::skin);
		void onChange();
	};


	class cActionChangeButton : public cWindowButton
	{
	public:
		cActionChangeButton(cWindow* p, Json::Value &skin = cWM::skin);
		void onClick();
	};

	class cDirectionButton : public cWindowButton
	{
		int direction;
	public:
		cDirectionButton(cWindow* p, int dir, Json::Value &skin = cWM::skin);
		void onClick();
	};

	class cOkButton : public cWindowButton
	{
	public:
		cOkButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};
	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, Json::Value &skin = cWM::skin);
		void onClick();
	};


	std::map<cWindowTree::cTreeNode*, cSpriteInfo, std::less<cWindowTree::cTreeNode*> >	lookupMap;
	bool male;

	cSpriteWindow();


};

#endif
