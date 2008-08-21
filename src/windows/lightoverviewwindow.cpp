#include "lightoverviewwindow.h"

#include <menu.h>
#include <menucommands.h>
#include <graphics.h>
extern cGraphics Graphics;

extern cMenu* popupmenu;


cLightOverViewWindow::cLightOverViewTreeNode::cLightOverViewTreeNode() : cWindowTree::cTreeNode()
{
	
}

cLightOverViewWindow::cLightOverViewTreeNode::cLightOverViewTreeNode( std::string t ) : cWindowTree::cTreeNode(t)
{
	
}

cLightOverViewWindow::cLightOverViewTreeNode::cLightOverViewTreeNode( std::string t, std::vector<cWindowTree::cTreeNode*> items ) : cWindowTree::cTreeNode(t, items)
{
	
}

cLightOverViewWindow::cLightOverViewTreeNode* cLightOverViewWindow::cLightOverViewTreeNode::addNodeTree( std::string n )
{
	if(n.find("\\") == std::string::npos)
	{
		cLightOverViewTreeNode* node = new cLightOverViewTreeNode(n);
		addchild(node);
		return node;
	}
	else
	{
		std::string firstpart = n.substr(0, n.find("\\"));
		std::string secondpart = n.substr(n.find("\\")+1);
		
		for(unsigned int i = 0; i < children.size(); i++)
		{
			if(children[i]->text == firstpart)
			{
				return ((cLightOverViewTreeNode*)children[i])->addNodeTree(secondpart);
			}
		}
		
		cLightOverViewTreeNode* node = new cLightOverViewTreeNode(firstpart);
		addchild(node);
		node->isCat = true;
		return node->addNodeTree(secondpart);
		
		
	}
}

cLightOverViewWindow::cLightOverViewTreeNode* cLightOverViewWindow::cLightOverViewTreeNode::getNode( cLight m )
{
	if(light == m)
		return this;
	for(unsigned int i = 0; i < children.size(); i++)
	{
		cLightOverViewTreeNode* n = ((cLightOverViewTreeNode*)children[i])->getNode(m);
		if(n != NULL)
			return n;
	}
	return NULL;
}

cLightOverViewWindow::cLightOverViewTree::cLightOverViewTree( cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument &skin ) : cWindowTree(parent, nodes, skin)
{
	moveTo(0,0);
	resizeTo(parent->innerWidth(), parent->innerHeight());
	alignment = ALIGN_TOPLEFT;
}

void cLightOverViewWindow::cLightOverViewTree::rightClick()
{
	cWindowTree::rightClick();
	popupmenu = new cMenu();
	popupmenu->parent = NULL;
	popupmenu->drawstyle = 1;
	popupmenu->x = (int)mouseX;
	popupmenu->y = (int)mouseY;
	popupmenu->w = 150;
	popupmenu->opened = true;
	cMenuItem* mm;
	ADDMENUITEM(mm,popupmenu,"Go to",		&MenuCommand_removefavlight); //new
	ADDMENUITEM(mm,popupmenu,"Insert light",		&MenuCommand_addfavlight); //new
	ADDMENUITEM(mm,popupmenu,"Insert category",		&MenuCommand_addfavlightcat); //new
}

void cLightOverViewWindow::cLightOverViewTree::onChange()
{
	unsigned int i;
	int a = selected;
	cWindowTree::cTreeNode* node;
	for(i = 0; i < nodes.size(); i++)
	{
		node = nodes[i]->getnode(a);
		if(node != NULL)
			break;
	}
	if(node == NULL)
		return;
	
	if(!((cLightOverViewTreeNode*)node)->isCat)
	{
		Graphics.camerapointer.x = -5*((cLightOverViewTreeNode*)node)->light.pos.x;
		Graphics.camerapointer.y = Graphics.world.height*-10+5*((cLightOverViewTreeNode*)node)->light.pos.z;
		
		for(i = 0; i < Graphics.world.lights.size(); i++)
		{
			if(Graphics.world.lights[i] == ((cLightOverViewTreeNode*)node)->light)
				Graphics.selectedObject = i;
		}
	}
}

void cLightOverViewWindow::cLightOverViewTree::getObject( cLight m )
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		
		cTreeNode* n = ((cLightOverViewTreeNode*)nodes[i])->getNode(m);
		cTreeNode* nn = n;
		if(n != NULL)
		{
			n = n->parent;
			while(n != NULL)
			{
				n->open = true;
				n = n->parent;
			}
			return;
			
			std::vector<std::string> values;
			for(i = 0; i < nodes.size(); i++)
				nodes[i]->getdata(values);
			
			i = 0;
			int yyy = realY()+h-5-12;
			while(yyy+10 > realY() && i < values.size())
			{
				i++;
				yyy-=12;
			}
			
			selected = nn->getselectionnr();
			if(selected < liststart || selected > liststart+(int)i)
				liststart = min((int)values.size()-i+1 , selected-(i/2));
			if(liststart < 0)
				liststart = 0;
			break;
		}
	}
}

void cLightOverViewWindow::addobjects( cLightOverViewTreeNode* parent, bool root )
{
	if(root)
	{
		cLightOverViewTreeNode* windowtreenode = new cLightOverViewTreeNode("lights");
		windowtreenode->isCat = true;
		parent->addchild(windowtreenode);
		
		for(unsigned int i = 0; i < Graphics.world.lights.size(); i++)
		{
			cLightOverViewTreeNode* node = windowtreenode->addNodeTree(Graphics.world.lights[i].name);
			node->isCat = false;
			node->light = Graphics.world.lights[i];
		}
	}
}

cLightOverViewWindow::cLightOverViewWindow( cTexture* t, cFont* f, TiXmlDocument &skin ) : cWindow(t,f,skin)
{
	windowType = WT_LIGHTOVERVIEW;
	visible = true;
	closeType = HIDE;
	noTransparency = true;
	
	
	resizable = false;
	movable = false;
	h = Graphics.h();
	w = 256;
	title = GetMsg("wm/overview/TITLE");
	y = 0;
	x = Graphics.w() - w;
	
	//		cWindowObject* o;
	objects["close"] = new cWindowCloseButton(this,skin);
	
	
	std::vector<cWindowTree::cTreeNode*> nodes;
	objects["list"] = new cLightOverViewTree(this, nodes,skin);
	
	
	resizeTo(w,h);
	userfunc(NULL);
}

void* cLightOverViewWindow::userfunc( void* p )
{
	std::vector<cWindowTree::cTreeNode*> nodes;
	cLightOverViewTreeNode* windownode = new cLightOverViewTreeNode("root");
	
	addobjects(windownode, true);
	windownode->sort();
	
	for(unsigned int i = 0; i < windownode->children.size(); i++)
	{
		windownode->children[i]->parent = NULL;
		nodes.push_back(windownode->children[i]);
		windownode->children[i] = NULL;
	}
	delete windownode;
	
	((cWindowTree*)objects["list"])->nodes = nodes;
	return NULL;
}

void cLightOverViewWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww, hh);
	objects["list"]->resizeTo(innerWidth(), innerHeight());
}