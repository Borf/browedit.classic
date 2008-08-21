#ifndef __SPRITEWINDOW_H__
#define __SPRITEWINDOW_H__

#include <tinyxml/tinyxml.h>
#include <filesystem.h>

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
		cWindowSprite(cWindow* parent, TiXmlDocument &skin);
		void draw(int,int,int,int);
	};

	class cTabPanel : public cWindowTabPanel
	{
	public:
		cTabPanel(cWindow* parent, TiXmlDocument &skin);
		void tabchange(int oldtab);
		void addxml(cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode, std::string dir = "");
	};


	class cTree : public cWindowTree
	{
	public:
		cTree(cWindow* parent, std::vector<cTreeNode*> n, TiXmlDocument &skin);
		void onChange();
	};


	class cActionChangeButton : public cWindowButton
	{
	public:
		cActionChangeButton(cWindow* p, TiXmlDocument &skin);
		void click();
	};

	class cDirectionButton : public cWindowButton
	{
		int direction;
	public:
		cDirectionButton(cWindow* p, int dir, TiXmlDocument &skin);
		void click();
	};

	class cOkButton : public cWindowButton
	{
	public:
		cOkButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};
	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};


	std::map<cWindowTree::cTreeNode*, cSpriteInfo, std::less<cWindowTree::cTreeNode*> >	lookupMap;
	bool male;

	cSpriteWindow(cTexture* t, cFont* f, TiXmlDocument &skin);


};

#endif
