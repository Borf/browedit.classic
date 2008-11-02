#ifndef __SOUNDOVERVIEWWINDOW_H__
#define __SOUNDOVERVIEWWINDOW_H__

#include <wm/window.h>
#include <wm/windowtree.h>
#include <world.h>

class cSoundOverViewWindow : public cWindow
{
public:
	class cSoundOverViewTreeNode : public cWindowTree::cTreeNode
	{
	public:
		cSoundOverViewTreeNode();
		cSoundOverViewTreeNode(std::string t);
		cSoundOverViewTreeNode(std::string t, std::vector<cWindowTree::cTreeNode*> items);
		bool		isCat;
		cSound		sound;

		cSoundOverViewTreeNode* addNodeTree(std::string n);
		cSoundOverViewTreeNode* getnode(cSound m);
	};

	class cSoundOverViewTree : public cWindowTree
	{
	public:
		cSoundOverViewTree(cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument &skin);
		void onChange();
		void getObject(cSound m);
	};



	static void addObjects(cSoundOverViewTreeNode* parent, bool root);
	cSoundOverViewWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
	void* userfunc(void* p);
	void resizeTo(int ww, int hh);
};

#endif
