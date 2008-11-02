#ifndef __MODELOVERVIEWWINDOW_H__
#define __MODELOVERVIEWWINDOW_H__

#include <wm/window.h>
#include <wm/windowtree.h>

class cRSMModel;


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
		cRSMModel* model;

		cModelOverViewTreeNode* addNodeTree(std::string n);
		cModelOverViewTreeNode* getNode(cRSMModel* m);
	
	};

	class cModelOverViewTree : public cWindowTree
	{
	public:
		cModelOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin = NULL);
		void rightClick();
		void onChange();
		void getObject(cRSMModel* m);
	};



	static void addObjects(cModelOverViewTreeNode* parent, bool root);
	cModelOverViewWindow();
	void* userfunc(void* p);
	void resizeTo(int ww, int hh);
};

#endif
