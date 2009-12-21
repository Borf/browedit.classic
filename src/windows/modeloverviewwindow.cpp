#include "modeloverviewwindow.h"
#include <bengine/math/math.h>


#include <menu.h>
#include <menucommands.h>
#include <graphics.h>
#include <wm/windowbutton.h>
#include <world.h>
#include <RSMModel.h>




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
			if(children[i]->getText() == firstpart)
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

cModelOverViewWindow::cModelOverViewTreeNode* cModelOverViewWindow::cModelOverViewTreeNode::getNode( cRsmModel* m )
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
	if(cGraphics::popupMenu)
		delete cGraphics::popupMenu;
	cWindowTree::onRightClick();
	cGraphics::popupMenu = new cMenu();
	cGraphics::popupMenu->parent = NULL;
	cGraphics::popupMenu->drawStyle = 1;
	cGraphics::popupMenu->x = (int)cGraphics::cMouse::x;
	cGraphics::popupMenu->y = (int)cGraphics::cMouse::y;
	cGraphics::popupMenu->w = 150;
	cGraphics::popupMenu->opened = true;
	cMenuItem* mm;
	ADDMENUITEM(mm,cGraphics::popupMenu,"Go to",		&MenuCommand_removefavlight); //new
	ADDMENUITEM(mm,cGraphics::popupMenu,"Insert light",		&MenuCommand_addfavlight); //new
	ADDMENUITEM(mm,cGraphics::popupMenu,"Insert category",		&MenuCommand_addfavlightcat); //new
}

void cModelOverViewWindow::cModelOverViewTree::onChange()
{
	unsigned int i;
	int a = selected;
	cWindowTree::cTreeNode* node = NULL;
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

void cModelOverViewWindow::cModelOverViewTree::getObject( cRsmModel* m )
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
				liststart = bEngine::math::min((int)values.size()-i+1 , selected-(i/2));
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
