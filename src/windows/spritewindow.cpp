#include "spritewindow.h"

#include <sprite.h>
#include "objectwindow.h"
#include <graphics.h>
#include <settings.h>
#include <world.h>
#include <bengine/math/math.h>
#include <bengine/util/filesystem.h>
#include <math.h>
TiXmlDocument sprites;



cSpriteWindow::cSpriteInfo::cSpriteInfo()
{
	
}

cSpriteWindow::cSpriteInfo::cSpriteInfo( std::string f )
{
	fileName = f;
}

cSpriteWindow::cWindowSprite::cWindowSprite( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent, skin->FirstChildElement("skin")->FirstChildElement("frame"))
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0,0);
	resizeTo(120,160);
	sprite = new cSprite();
}

void cSpriteWindow::cWindowSprite::draw( int,int,int,int )
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realX();
	yy = realY();
	
	cWindowObject::draw(0,0,0,0);
	
	sprite->pos = bEngine::math::cVector3(xx/5 + w/2/5,yy+h/4,0);
	sprite->scale = 1;
	sprite->draw();
}

cSpriteWindow::cTabPanel::cTabPanel( cWindow* parent, TiXmlDocument* skin ) : cWindowTabPanel(parent, skin)
{
	alignment = ALIGN_TOPLEFT;
	tabs.clear();
	tabs.push_back("Body");
	tabs.push_back("Face");
	tabs.push_back("Upper");
	tabs.push_back("Mid");
	tabs.push_back("Low");
	moveTo(120, 0);
	resizeTo(parent->innerWidth() - 120,parent->innerHeight());
}

void cSpriteWindow::cTabPanel::addxml( cWindowTree* tree, TiXmlNode* n, cWindowTree::cTreeNode* treenode, std::string dir /*= ""*/ )
{
	while(n != NULL)
	{
		if(strcmp(n->Value(), "sprite") == 0)
		{
			cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->ToElement()->Attribute("name"));
			
			((cSpriteWindow*)parent)->lookupMap[node] = cSpriteInfo(dir + n->FirstChild()->Value());
			if(treenode == NULL)
				tree->nodes.push_back(node);
			else
				treenode->addchild(node);
		}
		else
		{
			cWindowTree::cTreeNode* node = new cWindowTree::cTreeNode(n->Value());
			if(treenode == NULL)
				tree->nodes.push_back(node);
			else
				treenode->addchild(node);
			char* d = (char*)n->ToElement()->Attribute("directory");
			addxml(NULL, n->FirstChildElement(), node, dir + (d != NULL ? d : ""));
			
		}
		n = n->NextSiblingElement();
	}
}
void cSpriteWindow::cTabPanel::tabchange( int oldtab )
{
	cWindowTree* tree = (cWindowTree*)parent->objects["tree"];
	tree->nodes.clear();
	tree->liststart = 0;
	
	TiXmlNode* n = sprites.FirstChildElement("sprites");
	if(selectedtab == 0)
		n = n->FirstChildElement("body");
	else if(selectedtab == 1)
		n = n->FirstChildElement("face");
	else
		n = n->FirstChildElement("headgear");
	((cSpriteWindow*)parent)->lookupMap.clear();
	if(selectedtab != 0)
		tree->nodes.push_back(new cWindowTree::cTreeNode("Nothing"));
	addxml(tree, n->FirstChildElement(), NULL, n->ToElement()->Attribute("directory") != NULL ? n->ToElement()->Attribute("directory") : "");
	
	tree->selected = 0;
	
	if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite == NULL)
		return;
	
	
	std::string name = "";
	if(selectedtab == 0 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body != NULL)
	{
		name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body->fileName;
		name = name.substr(cSettings::roDir.length() + 12);
	}
	if(selectedtab == 1 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head != NULL)
	{
		name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head->fileName;
		name = name.substr(cSettings::roDir.length() + 12);
	}
	if(selectedtab == 2 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 0)
	{
		if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[0] != NULL)
		{
			name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[0]->fileName;
			name = name.substr(name.rfind("\\")+1);
			name = name.substr(name.find("_"));
		}
	}
	if(selectedtab == 3 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 1)
	{
		if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[1] != NULL)
		{
			name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[1]->fileName;
			name = name.substr(name.rfind("\\")+1);
			name = name.substr(name.find("_"));
		}
	}
	if(selectedtab == 4 && ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size() > 2)
	{
		if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[2] != NULL)
		{
			name = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[2]->fileName;
			name = name.substr(name.rfind("\\")+1);
			name = name.substr(name.find("_"));
		}
	}
	
	
	cWindowTree::cTreeNode* node = NULL;
	for(std::map<cWindowTree::cTreeNode*, cSpriteInfo, std::less<cWindowTree::cTreeNode*> >::iterator it = ((cSpriteWindow*)parent)->lookupMap.begin(); it != ((cSpriteWindow*)parent)->lookupMap.end(); it++)
	{
		std::string f = it->second.fileName;
		if(it->second.fileName == name)
		{
			node = it->first;
		}
	}
	
	
	
	cWindowTree::cTreeNode* nn = node;
	while(node != NULL)
	{
		node->open = true;
		node = node->parent;
	}
	
	node = nn;
	if(node != NULL)
	{
		unsigned int i;
		tree->selected = 0;
		std::vector<cWindowTree::cTreeNode*> values;
		for(i = 0; i < tree->nodes.size(); i++)
			tree->nodes[i]->getnodes(values);
		
		for(i = 0; i < values.size(); i++)
		{
			if(values[i] == node)
				tree->selected = i;
		}
		
		tree->liststart = tree->selected - ((tree->getHeight()/2) / 20);
		if(tree->liststart < 0)
			tree->liststart = 0;
		
		int yy = tree->realY()+h-5-12;
		while(yy+10 > tree->realY() && i < values.size())
		{
			i++;
			yy-=12;
		}
		
		if(tree->getHeight()/12 > (int)values.size())
			tree->liststart = 0;
		
		if(tree->liststart >= (int)values.size() - (tree->getHeight()/12))
			tree->liststart = values.size() - (tree->getHeight()/12);
		if(tree->liststart < 0)
			tree->liststart = 0;
		
		
		
	}
}


cSpriteWindow::cTree::cTree( cWindow* parent, std::vector<cTreeNode*> n, TiXmlDocument* skin ) : cWindowTree(parent, n,skin)
{
	
}
 
void cSpriteWindow::cTree::onChange()
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
	
	if(node->children.size() == 0)
	{
		
		int selectedtab = parent->objects["tabpanel"]->getInt(0);
		if(((cSpriteWindow*)parent)->lookupMap.find(node) == ((cSpriteWindow*)parent)->lookupMap.end())
		{
			if(selectedtab == 1)
			{
				delete ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head;
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head = NULL;
			}
			if(selectedtab > 1)
			{
				delete ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[selectedtab-2];
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[selectedtab-2] = NULL;
			}
			return;
		}
		else
		{
			cSpriteWindow::cSpriteInfo* info = &((cSpriteWindow*)parent)->lookupMap[node];
			//	cGraphics::WM.MessageBox("Selected: " + info->fileName);
			if(selectedtab == 0)
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->loadBody(cSettings::roDir + "data\\sprite\\" + info->fileName);
			if(selectedtab == 1)
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->loadHead(cSettings::roDir + "data\\sprite\\" + info->fileName);
			if(selectedtab == 2)
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setExtra(0, cSettings::roDir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->fileName);
			if(selectedtab == 3)
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setExtra(1, cSettings::roDir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->fileName);
			if(selectedtab == 4)
				((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->setExtra(2, cSettings::roDir + "data\\sprite\\악세사리\\" + (((cSpriteWindow*)parent)->male ? "남\\남" : "여\\여")+ info->fileName);
		}
	}
}



cSpriteWindow::cActionChangeButton::cActionChangeButton( cWindow* p, TiXmlDocument* skin ) : cWindowButton(p,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0, 200);
	resizeTo(120, 20);
	text = "Action 1";
}

void cSpriteWindow::cActionChangeButton::onClick()
{
	((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action = (((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action + 1) % 12;
	char buf[32];
	sprintf(buf, "Action: %i", ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action);
	text = buf;
}


cSpriteWindow::cDirectionButton::cDirectionButton( cWindow* p, int dir, TiXmlDocument* skin ) : cWindowButton(p,skin)
{
	direction = dir;
	const char* directions[] = { "S", "SE","E","NE","N","NW","W","SW" };
	text = directions[dir];
	resizeTo(50,20);
	alignment = ALIGN_TOPLEFT;
	moveTo(35+(int)(35*sin(dir/8.0f*2*bEngine::math::PI)), 300+     (dir<5 ? (-20*dir) : -20*(8-dir))     );
}

void cSpriteWindow::cDirectionButton::onClick()
{
	((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->direction = direction;
}

cSpriteWindow::cOkButton::cOkButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0, parent->innerHeight()-20);
	resizeTo(120, 20);
	text = "Ok";
}

void cSpriteWindow::cOkButton::onClick()
{
	if((int)cGraphics::world->sprites.size() > cGraphics::worldContainer->settings.selectedObject && cGraphics::worldContainer->settings.selectedObject != -1)
	{
		
		if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head != NULL)
			cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->loadHead(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->head->fileName);
		else
		{
			delete cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->head;
			cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->head = NULL;
		}
		cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->loadBody(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->body->fileName);
		
		for(unsigned int i = 0; i < ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras.size(); i++)
		{
			if(((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[i] != NULL)
			{
				cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->setExtra(i, ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->extras[i]->fileName);
			}
			else
			{
				if(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras.size() > i)
					if(cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras[i] != NULL)
					{
						delete cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras[i];
						cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->extras[i] = NULL;
					}
			}
		}
		cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->action = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->action;
		cGraphics::world->sprites[cGraphics::worldContainer->settings.selectedObject]->direction = ((cSpriteWindow::cWindowSprite*)parent->objects["spritewindow"])->sprite->direction;
	}
	
	parent->close();
}

cSpriteWindow::cCancelButton::cCancelButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent, skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0, parent->innerHeight()-40);
	resizeTo(120, 20);
	text = "Cancel";
}

void cSpriteWindow::cCancelButton::onClick()
{
	parent->close();
}

cSpriteWindow::cSpriteWindow() : cWindow()
{
	male = true;
	cWindowObject* o;
	windowType = WT_SPRITE;
	resizable = false;
	visible = true;
	male = true;
	
	h = 400;
	w = 550;
	title = GetMsg("wm/sprite/TITLE");
	center();
	
	initProps("sprite");
	
	defaultObject = "OkButton";
	
	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);
	
	objects["spritewindow"] = new cWindowSprite(this);
	
	objects["tabpanel"] = new cTabPanel(this);
	std::vector<cWindowTree::cTreeNode*> nodes;
	o = new cTree(this,nodes);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(125, 30);
	o->resizeTo(innerWidth() - 130, innerHeight()- 35);
	objects["tree"] = o;
	objects["actionbutton"] = new cActionChangeButton(this);
	objects["ok"] = new cOkButton(this);
	objects["cancel"] = new cCancelButton(this);
	
	for(int i = 0; i < 8; i++)
	{
		char buf[10];
		sprintf(buf, "dir%i", i);
		objects[buf] = new cDirectionButton(this, i);
	}
	
	if(!sprites.FirstChild())
		sprites;//TODObengine = cFileSystem::getXml("data/sprites.xml");
	
	((cTabPanel*)objects["tabpanel"])->tabchange(-1);
}
