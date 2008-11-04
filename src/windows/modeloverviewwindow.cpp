#include "modeloverviewwindow.h"


#include <menu.h>
#include <menucommands.h>
#include <graphics.h>
#include <wm/windowbutton.h>

extern cMenu* popupmenu;




cModelOverViewWindow::cModelOverViewTreeNode::cModelOverViewTreeNode() : cWindowTree::cTreeNode()
{
	
}

cModelOverViewWindow::cModelOverViewTreeNode::cModelOverViewTreeNode( std::string t ) : cWindowTree::cTreeNode(t)
{
	
}

cModelOverViewWindow::cModelOverViewTreeNode::cModelOverViewTreeNode( std::string t, std::vector<cWindowTree::cTreeNode*> items ) : cWindowTree::cTreeNode(t, items)
{
	
}

cModelOverViewWindow::cModelOverViewTreeNode* cModelOverViewWindow::cModelOverViewTreeNode::addNodeTree( std::string n )
{
	if(n.find("\\") == std::string::npos)
	{
		cModelOverViewTreeNode* node = new cModelOverViewTreeNode(n);
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
				return ((cModelOverViewTreeNode*)children[i])->addNodeTree(secondpart);
			}
		}
		
		cModelOverViewTreeNode* node = new cModelOverViewTreeNode(firstpart);
		addchild(node);
		node->isCat = true;
		return node->addNodeTree(secondpart);
		
		
	}
}

cModelOverViewWindow::cModelOverViewTreeNode* cModelOverViewWindow::cModelOverViewTreeNode::getNode( cRSMModel* m )
{
	if(model == m)
		return this;
	for(unsigned int i = 0; i < children.size(); i++)
	{
		cModelOverViewTreeNode* n = ((cModelOverViewTreeNode*)children[i])->getNode(m);
		if(n != NULL)
			return n;
	}
	return NULL;
}

cModelOverViewWindow::cModelOverViewTree::cModelOverViewTree( cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin ) : cWindowTree(parent, nodes, skin)
{
	moveTo(0,0);
	resizeTo(parent->innerWidth(), parent->innerHeight());
	alignment = ALIGN_TOPLEFT;
}

void cModelOverViewWindow::cModelOverViewTree::onRightClick()
{
	cWindowTree::onRightClick();
	popupmenu = new cMenu();
	popupmenu->parent = NULL;
	popupmenu->drawStyle = 1;
	popupmenu->x = (int)mouseX;
	popupmenu->y = (int)mouseY;
	popupmenu->w = 150;
	popupmenu->opened = true;
	cMenuItem* mm;
	ADDMENUITEM(mm,popupmenu,"Go to",		&MenuCommand_removefavlight); //new
	ADDMENUITEM(mm,popupmenu,"Insert light",		&MenuCommand_addfavlight); //new
	ADDMENUITEM(mm,popupmenu,"Insert category",		&MenuCommand_addfavlightcat); //new
}

void cModelOverViewWindow::cModelOverViewTree::onChange()
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
	
	if(!((cModelOverViewTreeNode*)node)->isCat)
	{
		cGraphics::worldContainer->camera.pointer.x = -5*((cModelOverViewTreeNode*)node)->model->pos.x;
		cGraphics::worldContainer->camera.pointer.y = cGraphics::world->height*-10+5*((cModelOverViewTreeNode*)node)->model->pos.z;
	
		for(i = 0; i < cGraphics::world->models.size(); i++)
		{
			if(cGraphics::world->models[i] == ((cModelOverViewTreeNode*)node)->model)
				cGraphics::worldContainer->settings.selectedObject = i;
		}
	}
}

void cModelOverViewWindow::cModelOverViewTree::getObject( cRSMModel* m )
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		
		cTreeNode* n = ((cModelOverViewTreeNode*)nodes[i])->getNode(m);
		cTreeNode* nn = n;
		if(n != NULL)
		{
			n = n->parent;
			while(n != NULL)
			{
				n->open = true;
				n = n->parent;
			}
			
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

void cModelOverViewWindow::addObjects( cModelOverViewTreeNode* parent, bool root )
{
	if(root)
	{
		cModelOverViewTreeNode* windowtreenode = new cModelOverViewTreeNode("objects");
		windowtreenode->isCat = true;
		parent->addchild(windowtreenode);
		
		for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
		{
			cModelOverViewTreeNode* node = windowtreenode->addNodeTree(cGraphics::world->models[i]->name);
			node->isCat = false;
			node->model = cGraphics::world->models[i];
		}
	}
}

cModelOverViewWindow::cModelOverViewWindow( ) : cWindow()
{
	windowType = WT_MODELOVERVIEW;
	visible = true;
	closeType = HIDE;
	noTransparency = true;
	
	
	resizable = false;
	movable = false;
	h = cGraphics::h();
	w = 256;
	title = GetMsg("wm/overview/TITLE");
	y = 0;
	x = cGraphics::w() - w;
	
	//		cWindowObject* o;
	objects["close"] = new cWindowCloseButton(this);
	
	
	std::vector<cWindowTree::cTreeNode*> nodes;
	objects["list"] = new cModelOverViewTree(this, nodes);
	
	
	resizeTo(w,h);
	userfunc(NULL);
}

void* cModelOverViewWindow::userfunc( void* p )
{
	std::vector<cWindowTree::cTreeNode*> nodes;
	cModelOverViewTreeNode* windownode = new cModelOverViewTreeNode("root");
	
	addObjects(windownode, true);
	windownode->sort();
	
	for(unsigned int i = 0; i < windownode->children.size(); i++)
	{
		windownode->children[i]->parent = NULL;
		nodes.push_back(windownode->children[i]);
		windownode->children[i] = NULL;
	}
	delete windownode;
	
	((cWindowTree*)objects["list"])->nodes = nodes;
	((cWindowTree*)objects["list"])->validCache = false;
	return NULL;
}

void cModelOverViewWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww, hh);
	objects["list"]->resizeTo(innerWidth(), innerHeight());
}