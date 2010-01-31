#ifndef __LIGHTOVERVIEWWINDOW_H__
#define __LIGHTOVERVIEWWINDOW_H__

#include <wm/window.h>
#include <wm/windowtree.h>
#include <world.h>

class cLightOverViewWindow : public cWindow
{
public:
	class cLightOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cLightOverViewTreeNode();
		cLightOverViewTreeNode(std::string t);
		cLightOverViewTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items);
		bool		isCat;
		cLight		light;

		cLightOverViewTreeNode* addNodeTree(std::string n);
		cLightOverViewTreeNode* getNode(cLight m);	
	};

	class cLightOverViewTree : public cWindowTree
	{
	public:
		cLightOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin = &cWM::skin);
		void onRightClick();
		void onChange();
		void getObject(cLight m);
	};


	cLightOverViewWindow();

	static void addobjects(cLightOverViewTreeNode* parent, bool root);
	void* userfunc(void* p);
	void resizeTo(int ww, int hh);
};

#endif
