#ifndef __MODELOVERVIEWWINDOW_H__
#define __MODELOVERVIEWWINDOW_H__

#include <wm/window.h>
#include <wm/windowtree.h>

class cRsmModel;


class cModelOverViewWindow : public cWindow
{
public:
	class cModelOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cModelOverViewTreeNode();
		cModelOverViewTreeNode(std::string t);
		cModelOverViewTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items);

		bool		isCat;
		int type;
		cRsmModel* model;

		cModelOverViewTreeNode* addNodeTree(std::string n);
		cModelOverViewTreeNode* getNode(cRsmModel* m);
	
	};

	class cModelOverViewTree : public cWindowTree
	{
	public:
		cModelOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin = &cWM::skin);
		void onRightClick();
		void onChange();
		void getObject(cRsmModel* m);
	};



	static void addObjects(cModelOverViewTreeNode* parent, bool root);
	cModelOverViewWindow();
	void* userfunc(void* p);
	void resizeTo(int ww, int hh);
};

#endif
