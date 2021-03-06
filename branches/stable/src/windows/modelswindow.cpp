#include <common.h>
#include "modelswindow.h"

#include <filesystem.h>
#include "../graphics.h"
#include "../menu.h"
#include "../menucommands.h"
#include <windows/confirmwindow.h>
#include <wm/windowlabel.h>
#include <fstream>
#include <settings.h>


cModelsWindow::cWindowModel::cWindowModel(cWindow* parent) : cWindowObject(parent)
{
	type = OBJECT_MODEL;
	model = NULL;
}
void cModelsWindow::cWindowModel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	
	if(cutoffbottom < 0)
		cutoffbottom = 0;
	glEnable(GL_DEPTH_TEST);
	if(data != "" && model == NULL)
	{
		glPushMatrix();
		glLoadIdentity();
		model = new cRsmModel(cSettings::roDir + data);
		model->pos = cVector3(0,0.3*w,1000);
		
		float sc = 0;
		sc = max(sc, model->bbmax[0] - model->bbmin[0]);
		sc = max(sc, model->bbmax[1] - model->bbmin[1]);
		sc = max(sc, model->bbmax[2] - model->bbmin[2]);
		sc = 1.4f*min(h,w) / sc;

		model->scale = cVector3(sc,sc,sc);

		model->rot = cVector3(0,0,0);
//TODO		model->bb2.bbrange[0] = 0;
		//model->bb2.bbmin[1] = 0;
//TODO		model->bb2.bbrange[2] = 0;
		glPopMatrix();
	}


	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();

	int xx, yy;
	xx = parent->getX() + realX();
	yy = parent->getY() + realY();
	glViewport(xx, yy+(cutoffbottom), w, h-(cutofftop) - (cutoffbottom));
	glLoadIdentity();
	glOrtho(-w,w,-h+2*cutoffbottom,h-2*cutofftop,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(0,0,0,colors[3]);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-w,-h);
		glVertex2f(w,-h);
		glVertex2f(w,h);
		glVertex2f(-w,h);
	glEnd();
	glLineWidth(1);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,colors[3]);

	if (model != NULL)
	{
		model->draw(); //TODO: check frustum
		model->rot.y+=40*(cGraphicsBase::getFrameTicks() / 1000.0f);
		model->rot.x = -45;
	}

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);

}


void cModelsWindow::cWindowModel::onClick()
{
	if(cGraphics::previewModel != NULL)
	{
		if(cGraphics::previewModel->filename == model->filename)
		{
			if (cSettings::editMode == MODE_OBJECTS)
				cGraphics::previewColor = 0;
			parent->close();
			return;
		}
		delete cGraphics::previewModel;
	}
	cGraphics::previewModel = new cRsmModel(cSettings::roDir + data);
	cGraphics::previewModel->rot = cVector3(0,0,0);
	cGraphics::previewModel->scale = cVector3(4,4,4);
	cGraphics::previewModel->pos = cVector3(40,-40,-40);
	cGraphics::previewColor = 200;

}

void cModelsWindow::cWindowModel::setText(int i, std::string s)
{
	if(i == 1)
	{
		data = s;
	}
}

std::string cModelsWindow::cWindowModel::getText(int i)
{
	return data;
}

void cModelsWindow::cWindowModel::drag()
{
	parent->objects["zdragger"]->moveTo((int)cGraphics::cMouse::x-parent->getX(),(int)cGraphics::cMouse::y-(cGraphics::h()-parent->getHeight()-parent->getY()));
	parent->objects["zdragger"]->setText(1, data);
}

void cModelsWindow::cWindowModel::setInt(int i, int id)
{
	data = "";
	if (model != NULL)
	{
		delete model;
		model = NULL;
	}
}


cModelsWindow::cWindowModel::~cWindowModel()
{
	if(model != NULL)
		delete model;
	model = NULL;
}



class cConfirmDeleteModel : public cConfirmWindowCaller
{
	private:
		cModelsWindow::cWindowModel* model;
	public:
		cConfirmDeleteModel(cModelsWindow::cWindowModel* m)
		{
			model = m;
		}
		void Ok()
		{
			unsigned int i;
			cModelsWindow::cWindowModelCatSelect* tree = (cModelsWindow::cWindowModelCatSelect*)model->parent->objects["tree"];
			int a = tree->selected;
			cWindowTree::cTreeNode* node = NULL;
			for(i = 0; i < tree->nodes.size(); i++)
			{
				 node = tree->nodes[i]->getnode(a);
				 if(node != NULL)
					 break;
			}
			std::string orig;
			cWindowTree::cTreeNode* n = node;
			while(n != NULL)
			{
				orig = "/" + n->text + orig;
				n = n->parent;
			}
			orig = orig.substr(1);


			for(i = 0; i < cSettings::objectFiles.size(); i++)
			{
				cFile* pFile = cFileSystem::open(cSettings::objectFiles[i]);
				std::ofstream pFile2((cSettings::objectFiles[i] + ".tmp").c_str());
				while(!pFile->eof())
				{
					std::string line = pFile->readLine();
					std::string pre = line.substr(0, line.find("|"));
					std::string filename = line.substr(line.find("|")+1);
					std::string directory = pre.substr(0, pre.rfind("/"));
					if(filename == model->getText(0) && directory == orig)
					{
						Log(3,0, "Found the evil one, removing");
					}
					else
					{
						line += "\n";
						pFile2.write(line.c_str(), line.length());
					}
				}
				pFile->close();
				pFile2.close();
#ifdef WIN32
				DeleteFile((cSettings::objectFiles[i]+".bak").c_str());
				MoveFile(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
				MoveFile((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#else
				unlink((cSettings::objectFiles[i]+".bak").c_str());
				rename(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
				rename((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#endif
			}

			model->parent->selectedObject = NULL;
			for(i = 0; i < ((cModelsWindow*)model->parent)->items[node].size(); i++)
			{
				std::string a = ((cModelsWindow*)model->parent)->items[node][i].second;
				std::string b = model->getText(0);
				if(a == b)
				{
					((cModelsWindow*)model->parent)->items[node].erase(((cModelsWindow*)model->parent)->items[node].begin() + i);
					i--;
				}
			}

			tree->refreshmodels();

				
			
			
		}
		void Cancel()
		{
		}

};


void cModelsWindow::cWindowModel::onRightClick()
{
	if(cGraphics::popupMenu)
		delete cGraphics::popupMenu;
	((cModelsWindow*)parent)->onStopDrag();
	cGraphics::popupMenu = new cMenu();
	cGraphics::popupMenu->parent = NULL;
	cGraphics::popupMenu->drawStyle = 1;
	cGraphics::popupMenu->x = (int)cGraphics::cMouse::x;
	cGraphics::popupMenu->y = (int)cGraphics::cMouse::y;
	cGraphics::popupMenu->w = 150;
	cGraphics::popupMenu->opened = true;
	cMenuItem* mm;
	ADDMENUITEM(mm,cGraphics::popupMenu,"Remove model from list",		&MenuCommand_new);
	ADDMENUITEM(mm,cGraphics::popupMenu,"Rename Model",				&MenuCommand_new);

	//cWM::ConfirmWindow(GetMsg("wm/model/DELETECONFIRM"), new cConfirmDeleteModel(this));
}

void cModelsWindow::cWindowModelCatSelect::onRightClick()
{
	unsigned int i;
	onClick();
	int a = selected;
	cTreeNode* node = NULL;
	for(i = 0; i < nodes.size(); i++)
	{
		 node = nodes[i]->getnode(a);
		 if(node != NULL)
			 break;
	}
	if(node != NULL)
	{
		if(cGraphics::popupMenu)
			delete cGraphics::popupMenu;
		cGraphics::popupMenu = new cMenu();
		cGraphics::popupMenu->parent = NULL;
		cGraphics::popupMenu->drawStyle = 1;
		cGraphics::popupMenu->x = (int)cGraphics::cMouse::x;
		cGraphics::popupMenu->y = (int)cGraphics::cMouse::y;
		cGraphics::popupMenu->w = 150;
		cGraphics::popupMenu->opened = true;
		cMenuItem* mm;
		ADDMENUITEM(mm,cGraphics::popupMenu,"Add new category",		&MenuCommand_new);
		ADDMENUITEM(mm,cGraphics::popupMenu,"Rename category",			&MenuCommand_new);

		
		
		
		std::string newnode = cWM::inputWindow(GetMsg("wm/models/NODENAME"));
		if(newnode == "")
			return;
		cTreeNode* n = new cTreeNode(newnode);
		n->parent = node;
		node->addchild(n);
		((cModelsWindow*)parent)->items[n] = std::vector<std::pair<std::string,std::string> >();

		std::string cat = "";
		n = node;
		while(n != NULL)
		{
			cat = n->text + "/" + cat;
			n = n->parent;
		}

		std::ofstream pFile("romodels.txt", std::ios_base::out | std::ios_base::app);
		if(pFile.good() && !pFile.bad())
		{
			char buf[512];
			sprintf(buf, "%s%s/|\n", cat.c_str(), newnode.c_str());
			pFile.write(buf, strlen(buf));
			pFile.close();
		}
	}
	else if (node == NULL)
	{
		std::string newnode = cWM::inputWindow(GetMsg("wm/models/NODENAME"));
		if(newnode == "")
			return;
		cTreeNode* n = new cTreeNode(newnode);
		n->parent = NULL;
		((cModelsWindow*)parent)->items[n] = std::vector<std::pair<std::string,std::string> >();
		nodes.push_back(n);
	}
}


bool cModelsWindow::cWindowModelCatSelect::onKeyDown(int key, bool shift)
{
	unsigned int i;
	bool b = cWindowTree::onKeyDown(key, shift);
	if(b)
		return true;
	
	if(key == SDLK_DELETE || key == SDLK_BACKSPACE)
	{
		int a = selected;
		cTreeNode* node = NULL;
		for(i = 0; i < nodes.size(); i++)
		{
			 node = nodes[i]->getnode(a);
			 if(node != NULL)
				 break;
		}
		if (node != NULL)
		{
			if(node->parent != NULL)
			{
				for(i = 0; i < node->parent->children.size(); i++)
				{
					if (node->parent->children[i] == node)
					{
						node->parent->children.erase(node->parent->children.begin() + i);
						break;
					}
				}
			}
			else
			{
				for(i = 0; i < nodes.size(); i++)
				{
					if (nodes[i] == node)
					{
						nodes.erase(nodes.begin() + i);
						break;
					}
				}
			}
			((cModelsWindow*)parent)->items.erase(((cModelsWindow*)parent)->items.find(node));
			std::string cat = "";
			cTreeNode* n = node;
			while(n != NULL)
			{
				cat = n->text + "/" + cat;
				n = n->parent;
			}

			for(i = 0; i < cSettings::objectFiles.size(); i++)
			{
				cFile* pFile = cFileSystem::open(cSettings::objectFiles[i]);
				std::ofstream pFile2((cSettings::objectFiles[i] + ".tmp").c_str());
				while(!pFile->eof())
				{
					std::string line = pFile->readLine();
					if(line.substr(0, cat.length()) != cat)
					{
						line += "\n";
						pFile2.write(line.c_str(), line.length());
					}
				}
				pFile->close();
				pFile2.close();
#ifdef WIN32
				DeleteFile((cSettings::objectFiles[i]+".bak").c_str());
				MoveFile(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
				MoveFile((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#else
				unlink((cSettings::objectFiles[i]+".bak").c_str());
				rename(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
				rename((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#endif
			}

			delete node;
			refreshmodels();
		}
		
		return true;
	}
	return false;
}

cModelsWindow::cWindowModelCatSelect::cWindowModelCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, TiXmlDocument* skin) : cWindowTree(parent, n,skin)
{
	originalselection = -1;
}

void cModelsWindow::cWindowModelCatSelect::onClick()
{
	cWindowTree::onClick();
	refreshmodels();
}
void cModelsWindow::cWindowModelCatSelect::onHoldDragOver()
{
	if(originalselection == -1)
		originalselection = selected;
	unsigned int i;
	std::vector<std::string> values;
	for(i = 0; i < nodes.size(); i++)
		nodes[i]->getdata(values);

	
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int yy = cGraphics::h()-(int)cGraphics::cMouse::y;
	yy -= realY();
	yy -= parent->getY();

	if (xx < w - 14)
	{ // in the box
		int s = selected;
		selected = liststart + ((h-yy-3) / 12);
		if (selected > (int)values.size() || selected < 0)
			selected = s;
	}
}

void cModelsWindow::cWindowModelCatSelect::onDragOver()
{
	bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
	unsigned int i;
	int a = selected;
	cTreeNode* node = NULL;
	for(i = 0; i < nodes.size(); i++)
	{
		 node = nodes[i]->getnode(a);
		 if(node != NULL)
			 break;
	}
	a = originalselection;
	cTreeNode* nodeorig = NULL;
	for(i = 0; i < nodes.size(); i++)
	{
		 nodeorig = nodes[i]->getnode(a);
		 if(nodeorig != NULL)
			 break;
	}

	if(node != NULL && nodeorig != NULL)
	{
		std::string orig = "";
		std::string dest = "";
		cTreeNode* n = node;
		while(n != NULL)
		{
			dest = "/" + n->text + dest;
			n = n->parent;
		}
		dest = dest.substr(1);
		n = nodeorig;
		while(n != NULL)
		{
			orig = "/" + n->text + orig;
			n = n->parent;
		}
		orig = orig.substr(1);

		for(i = 0; i < cSettings::objectFiles.size(); i++)
		{
			cFile* pFile = cFileSystem::open(cSettings::objectFiles[i]);
			std::ofstream pFile2((cSettings::objectFiles[i] + ".tmp").c_str());
			while(!pFile->eof())
			{
				std::string line = pFile->readLine();
				std::string pre = line.substr(0, line.find("|"));
				std::string filename = line.substr(line.find("|")+1);
				std::string directory = pre.substr(0, pre.rfind("/"));
				if(filename == cWM::draggingObject->getText(0) && directory == orig)
				{
					std::string bla = dest + "/" + cWM::draggingObject->getPopup() + "|" + cWM::draggingObject->getText(0) + "\n";
					pFile2.write(bla.c_str(), bla.length());
					if (ctrl)
					{
						line += "\n";
						pFile2.write(line.c_str(), line.length());
					}

				}
				else
				{
					line += "\n";
					pFile2.write(line.c_str(), line.length());
				}
			}
			pFile->close();
			pFile2.close();
#ifdef WIN32
			DeleteFile((cSettings::objectFiles[i]+".bak").c_str());
			MoveFile(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
			MoveFile((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#else
			unlink((cSettings::objectFiles[i]+".bak").c_str());
			rename(cSettings::objectFiles[i].c_str(), (cSettings::objectFiles[i]+".bak").c_str());
			rename((cSettings::objectFiles[i]+".tmp").c_str(), cSettings::objectFiles[i].c_str());
#endif
		}



		if(((cModelsWindow*)parent)->items.find(nodeorig) == ((cModelsWindow*)parent)->items.end())
			Log(1,0,"Oops");

		for(i = 0; i < ((cModelsWindow*)parent)->items[nodeorig].size(); i++)
		{
			std::string a = ((cModelsWindow*)parent)->items[nodeorig][i].second;
			std::string b = cWM::draggingObject->getText(0);
			if(a == b)
			{
				((cModelsWindow*)parent)->items[node].push_back(((cModelsWindow*)parent)->items[nodeorig][i]);
				if (!ctrl)
				{
					((cModelsWindow*)parent)->items[nodeorig].erase(((cModelsWindow*)parent)->items[nodeorig].begin() + i);
					i--;
				}
			}
		}

		selected = originalselection;
		originalselection = -1;
		if(!ctrl)
			refreshmodels();


	}
	else
	{
		selected = originalselection;
		originalselection = -1;
	}
	parent->onStopDrag();
	cWM::draggingObject = NULL;
	cWM::draggingWindow = NULL;

}


void cModelsWindow::cWindowModelCatSelect::refreshmodels()
{
	unsigned int i;
	cWindowScrollPanel* box = (cWindowScrollPanel*)parent->objects["models"];
	for(i = 0; i < box->objects.size(); i++)
		delete box->objects[i];
	box->objects.clear();
	int a = selected;
	cWindowTree::cTreeNode* node = NULL;
	for(i = 0; i < nodes.size(); i++)
	{
		 node = nodes[i]->getnode(a);
		 if(node != NULL)
			 break;
	}

	std::vector<std::pair<std::string, std::string> > v;
	v = *((std::vector<std::pair<std::string, std::string> >*)parent->userfunc(node));

	for(i = 0; i < v.size(); i++)
	{
		std::pair<std::string, std::string> p = v[i];
		cWindowObject* o = new cWindowModel(parent);
		o->alignment = ALIGN_TOPLEFT;
		o->moveTo(i*130, 32);
		o->resizeTo(128,128);
		o->setText(0,cSettings::roDir + p.second);
		o->setText(1,p.second);
		o->setPopup(p.first);
		box->objects.push_back(o);


		o = new cWindowLabel(parent);
		o->alignment = ALIGN_TOPLEFT;
		o->setText(0,"#000000" + p.first);
		box->objects.push_back(o);

		o = new cWindowLabel(parent);
		o->alignment = ALIGN_TOPLEFT;
		o->setText(0,"#FF0000" + p.first);
		box->objects.push_back(o);
	}
	parent->resizeTo(parent->getWidth(), parent->getHeight());
	cWM::draggingWindow = NULL;
	cWM::draggingObject = NULL;
}

void addnode(std::vector<cWindowTree::cTreeNode*> &nodes, std::map<std::string, cWindowTree::cTreeNode*, std::less<std::string> > &lookup, std::string cat)
{
	if(lookup.find(cat) == lookup.end())
	{
		if(cat.rfind("/") != std::string::npos)
		{
			std::string p = cat.substr(0, cat.rfind("/"));
			if(lookup.find(p) == lookup.end())
				addnode(nodes, lookup, p);
			cWindowTree::cTreeNode* n = new cWindowTree::cTreeNode(cat.substr(cat.rfind("/")+1));
			lookup[cat] = n;
			lookup[p]->addchild(n);
		}
		else
		{
			cWindowTree::cTreeNode* n = new cWindowTree::cTreeNode(cat);
			lookup[cat] = n;
			nodes.push_back(n);
		}
	}

}

cModelsWindow::cModelsWindow() : cWindow()
{
	windowType = WT_MODELS;
	closeType = HIDE;
	resizable = true;
	visible = true;
	modal = false;

	h = cGraphics::h()-50;
	w = cGraphics::w()-50;
	title = GetMsg("wm/model/TITLE");
	center();

	initProps("modelswindow");

	cWindowObject* o;

	std::vector<cWindowTree::cTreeNode*> nodes;
	std::map<std::string, cWindowTree::cTreeNode*, std::less<std::string> > lookup;

	for(unsigned int i = 0; i < cSettings::objectFiles.size(); i++)
	{
		cFile* pFile = cFileSystem::open(cSettings::objectFiles[i]);
		while(!pFile->eof())
		{
			std::string line = pFile->readLine();
			if (line == "")
				continue;
			std::string pre = line.substr(0, line.find("|"));
			std::string filename = line.substr(line.find("|")+1);

			std::string cat = pre.substr(0, pre.rfind("/"));
			std::string name = pre.substr(pre.rfind("/")+1);

			for(unsigned int ii = 0; ii < cSettings::translations.size(); ii++)
			{
				name = replace(name, cSettings::translations[ii].first, cSettings::translations[ii].second);
				cat = replace(cat, cSettings::translations[ii].first, cSettings::translations[ii].second);
			}

			if(lookup.find(cat) == lookup.end())
			{
				addnode(nodes, lookup, cat);
			}
			if(filename != "")
			{
				if (items.find(lookup[cat]) == items.end())
				{
					items[lookup[cat]] = std::vector<std::pair<std::string, std::string> >();
				}
				items[lookup[cat]].push_back(std::pair<std::string,std::string>(name,filename));
			}
		}
	}

	o = new cWindowModelCatSelect(this, nodes);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(0,0);
	o->resizeTo(400,400);
	objects["tree"] = o;

/*		o = new cWindowListBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(20,20);
	o->resizeTo(100,100);
	objects["textures"] = o;*/

	o = new cWindowScrollPanel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(20, 30);
	o->resizeTo(100,100);
	objects["models"] = o;

	o = new cWindowModel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveTo(9999,9999);
	o->resizeTo(128,128);
	//o->SetText(0,"data/dragger.tga");
	objects["zdragger"] = o;

//	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);

	resizeTo(w,h);
}	

void cModelsWindow::resizeTo(int ww, int hh)
{
	cWindow::resizeTo(ww,hh);
	objects["tree"]->resizeTo(200, innerHeight());
	cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["models"];
	panel->moveTo(200, 0);
	panel->resizeTo(innerWidth()-200, innerHeight());
	panel->innerwidth = innerWidth()-200;

	int x = 0;
	int y = 0;
	for(unsigned int i = 0; i < panel->objects.size(); i+=3)
	{
		panel->objects[i]->moveTo(x,y);
		panel->objects[i+1]->moveTo(x+5,y+96);
		panel->objects[i+2]->moveTo(x+4,y+95);
		x+=130;
		if(x+130 > panel->innerwidth-18)
		{
			x = 0;
			y += 130;
		}
	}
	panel->scrollposx = 0;
	panel->scrollposy = 0;
	panel->innerheight = y+140;
}
void cModelsWindow::onStopDrag()
{
	objects["zdragger"]->moveTo(-4000,-4000);
	objects["zdragger"]->setInt(0,0);
	((cWindowScrollPanel*)objects["models"])->draggingObject = NULL;
	cWM::draggingObject = NULL;

}	

void* cModelsWindow::userfunc(void* param)
{
	return &items[(cWindowTree::cTreeNode*)param];
}

