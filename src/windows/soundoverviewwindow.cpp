#include "soundoverviewwindow.h"

#include <wm/windowbutton.h>
#include <graphics.h>


cSoundOverViewWindow::cSoundOverViewTreeNode::cSoundOverViewTreeNode() : cWindowTree::cTreeNode()
{
	
}

cSoundOverViewWindow::cSoundOverViewTreeNode::cSoundOverViewTreeNode( std::string t ) : cWindowTree::cTreeNode(t)
{
	
}

cSoundOverViewWindow::cSoundOverViewTreeNode::cSoundOverViewTreeNode( std::string t, std::vector<cWindowTree::cTreeNode*> items ) : cWindowTree::cTreeNode(t, items)
{
	
}

cSoundOverViewWindow::cSoundOverViewTreeNode* cSoundOverViewWindow::cSoundOverViewTreeNode::addNodeTree( std::string n )
{
	if(n.find("\\") == std::string::npos)
	{
		cSoundOverViewTreeNode* node = new cSoundOverViewTreeNode(n);
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
				return ((cSoundOverViewTreeNode*)children[i])->addNodeTree(secondpart);
			}
		}
		
		cSoundOverViewTreeNode* node = new cSoundOverViewTreeNode(firstpart);
		addchild(node);
		node->isCat = true;
		return node->addNodeTree(secondpart);
		
		
	}
}

cSoundOverViewWindow::cSoundOverViewTreeNode* cSoundOverViewWindow::cSoundOverViewTreeNode::getnode( cSound m )
{
	if(sound == m)
		return this;
	for(unsigned int i = 0; i < children.size(); i++)
	{
		cSoundOverViewTreeNode* n = ((cSoundOverViewTreeNode*)children[i])->getnode(m);
		if(n != NULL)
			return n;
	}
	return NULL;
}

cSoundOverViewWindow::cSoundOverViewTree::cSoundOverViewTree( cWindow* parent, std::vector<cWindowTree::cTreeNode*> nodes, TiXmlDocument* skin ) : cWindowTree(parent, nodes, skin)
{
	moveTo(0,0);
	resizeTo(parent->innerWidth(), parent->innerHeight());
	alignment = ALIGN_TOPLEFT;
}

void cSoundOverViewWindow::cSoundOverViewTree::onChange()
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
	
	if(!((cSoundOverViewTreeNode*)node)->isCat)
	{
		cGraphics::worldContainer->camera.pointer.x = -5*((cSoundOverViewTreeNode*)node)->sound.pos.x;
		cGraphics::worldContainer->camera.pointer.y = cGraphics::world->height*-10+5*((cSoundOverViewTreeNode*)node)->sound.pos.z;

		for(i = 0; i < cGraphics::world->sounds.size(); i++)
		{
			if(cGraphics::world->sounds[i] == ((cSoundOverViewTreeNode*)node)->sound)
				cGraphics::selectedObject = i;
		}
	}
}

void cSoundOverViewWindow::cSoundOverViewTree::getObject( cSound m )
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		
		cTreeNode* n = ((cSoundOverViewTreeNode*)nodes[i])->getnode(m);
		cTreeNode* nn = n;
		if(n != NULL)
		{
			n = n->parent;
			while(n != NULL)
			{
				n->open = true;
				n = n->parent;
			}
			
			selected = nn->getselectionnr();
			std::vector<std::string> values;
			for(i = 0; i < nodes.size() && i < selected; i++)
				nodes[i]->getdata(values);
			
			i = 0;
			int yyy = realY()+h-5-12;
			while(yyy+10 > realY() && i < values.size())
			{
				i++;
				yyy-=12;
			}
			
			if(selected < liststart || selected > liststart+(int)i)
				liststart = min((int)values.size()-i+1 , selected-(i/2));
			if(liststart < 0)
				liststart = 0;
			break;
		}
	}
}

void cSoundOverViewWindow::addObjects( cSoundOverViewTreeNode* parent, bool root )
{
	if(root)
	{
		cSoundOverViewTreeNode* windowtreenode = new cSoundOverViewTreeNode("sounds");
		windowtreenode->isCat = true;
		parent->addchild(windowtreenode);
		
		for(unsigned int i = 0; i < cGraphics::world->sounds.size(); i++)
		{
			cSoundOverViewTreeNode* node = windowtreenode->addNodeTree(cGraphics::world->sounds[i].name);
			node->isCat = false;
			node->sound = cGraphics::world->sounds[i];
		}
	}
}

cSoundOverViewWindow::cSoundOverViewWindow( ) : cWindow()
{
	windowType = WT_SOUNDOVERVIEW;
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
	objects["list"] = new cSoundOverViewTree(this, nodes);
	
	
	resizeTo(w,h);
	userfunc(NULL);
}

void* cSoundOverViewWindow::userfunc( void* p )
{
	std::vector<cWindowTree::cTreeNode*> nodes;
	cSoundOverViewTreeNode* windownode = new cSoundOverViewTreeNode("root");
	
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
	return NULL;
}

void cSoundOverViewWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww, hh);
	objects["list"]->resizeTo(innerWidth(), innerHeight());
}