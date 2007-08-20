#include "modelswindow.h"

#include "../filesystem.h"
#include "../graphics.h"
extern cGraphics Graphics;
extern cFileSystem fs;
extern string rodir;
extern cWindow* draggingwindow;
extern cWindowObject* draggingobject;
extern eMode editmode;

extern vector<string> objectfiles;



cModelsWindow::cWindowModel::cWindowModel(cWindow* parent) : cWindowObject(parent)
{
	type = OBJECT_MODEL;
	model = NULL;
}
void cModelsWindow::cWindowModel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	if(cutoffbottom < 0)
		cutoffbottom = 0;
	glEnable(GL_DEPTH_TEST);
	if(data != "" && model == NULL)
	{
		glPushMatrix();
		glLoadIdentity();
		model = new cRSMModel();
		model->load(rodir + data);
		model->pos = cVector3(0,0.7*w,1000);
		
		float sc = 0;
		sc = max(sc, model->bb2.bbmax[0] - model->bb2.bbmin[0]);
		sc = max(sc, model->bb2.bbmax[1] - model->bb2.bbmin[1]);
		sc = max(sc, model->bb2.bbmax[2] - model->bb2.bbmin[2]);
		sc = 1.5f*min(h,w) / sc;

		model->scale = cVector3(sc,sc,sc);

		model->rot = cVector3(0,0,0);
		model->bb2.bbrange[0] = 0;
		//model->bb2.bbmin[1] = 0;
		model->bb2.bbrange[2] = 0;
		glPopMatrix();
	}


	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();

	int xx, yy;
	xx = parent->px() + realx();
	yy = parent->py() + realy();
	glViewport(xx, yy+(cutoffbottom), w, h-(cutofftop) - (cutoffbottom));
	glLoadIdentity();
	glOrtho(-w,w,-h+2*cutoffbottom,h-2*cutofftop,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(0,0,0,1);
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
	glColor4f(1,1,1,1);

	if (model != NULL)
	{
		model->draw(false);
		model->rot.y+=40*(Graphics.frameticks / 1000.0f);
	}

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	glDisable(GL_DEPTH_TEST);

}


void cModelsWindow::cWindowModel::click()
{
	if(Graphics.previewmodel != NULL)
	{
		if(Graphics.previewmodel->filename == model->filename)
		{
			if (editmode == MODE_OBJECTS)
				Graphics.previewcolor = 0;
			parent->close();
			return;
		}
		delete Graphics.previewmodel;
	}
	Graphics.previewmodel = new cRSMModel();
	Graphics.previewmodel->load(rodir + data);
	Graphics.previewmodel->rot = cVector3(0,0,0);
	Graphics.previewmodel->scale = cVector3(4,4,4);
	Graphics.previewmodel->pos = cVector3(40,-40,-40);
	Graphics.previewcolor = 200;

}

void cModelsWindow::cWindowModel::SetText(int i, string s)
{
	if(i == 1)
	{
		data = s;
	}
}

string cModelsWindow::cWindowModel::GetText(int i)
{
	return data;
}

void cModelsWindow::cWindowModel::drag()
{
	parent->objects["zdragger"]->moveto(mousex-parent->px(),mousey-(Graphics.h()-parent->ph()-parent->py()));
	parent->objects["zdragger"]->SetText(1, data);
}

void cModelsWindow::cWindowModel::SetInt(int i, int id)
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



class cConfirmDeleteModel : public cConfirmWindow::cConfirmWindowCaller
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
			int i;
			cModelsWindow::cWindowModelCatSelect* tree = (cModelsWindow::cWindowModelCatSelect*)model->parent->objects["tree"];
			int a = tree->selected;
			cWindowTree::cTreeNode* node = NULL;
			for(i = 0; i < tree->nodes.size(); i++)
			{
				 node = tree->nodes[i]->getnode(a);
				 if(node != NULL)
					 break;
			}
			string orig;
			cWindowTree::cTreeNode* n = node;
			while(n != NULL)
			{
				orig = "/" + n->text + orig;
				n = n->parent;
			}
			orig = orig.substr(1);


			for(i = 0; i < objectfiles.size(); i++)
			{
				cFile* pFile = fs.open(objectfiles[i]);
				ofstream pFile2((objectfiles[i] + ".tmp").c_str());
				while(!pFile->eof())
				{
					string line = pFile->readline();
					string pre = line.substr(0, line.find("|"));
					string filename = line.substr(line.find("|")+1);
					string directory = pre.substr(0, pre.rfind("/"));
					if(filename == model->GetText(0) && directory == orig)
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
				DeleteFile((objectfiles[i]+".bak").c_str());
				MoveFile(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
				MoveFile((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#else
				unlink((objectfiles[i]+".bak").c_str());
				rename(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
				rename((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#endif
			}

			model->parent->selectedobject = NULL;
			for(i = 0; i < ((cModelsWindow*)model->parent)->items[node].size(); i++)
			{
				string a = ((cModelsWindow*)model->parent)->items[node][i].second;
				string b = model->GetText(0);
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


void cModelsWindow::cWindowModel::rightclick()
{
	((cModelsWindow*)parent)->stopdrag();
	Graphics.WM.ConfirmWindow(msgtable[WINDOW_MODELS_DELETECONFIRM], new cConfirmDeleteModel(this));
}

void cModelsWindow::cWindowModelCatSelect::rightclick()
{
	int i;
	click();
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
		string newnode = Graphics.WM.InputWindow(msgtable[WINDOW_MODELS_NODENAME]);
		if(newnode == "")
			return;
		cTreeNode* n = new cTreeNode(newnode);
		n->parent = node;
		node->addchild(n);
		((cModelsWindow*)parent)->items[n] = vector<pair<string,string> >();

		string cat = "";
		n = node;
		while(n != NULL)
		{
			cat = n->text + "/" + cat;
			n = n->parent;
		}

		ofstream pFile("romodels.txt", ios_base::out | ios_base::app);
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
		string newnode = Graphics.WM.InputWindow(msgtable[WINDOW_MODELS_NODENAME]);
		if(newnode == "")
			return;
		cTreeNode* n = new cTreeNode(newnode);
		n->parent = NULL;
		((cModelsWindow*)parent)->items[n] = vector<pair<string,string> >();
		nodes.push_back(n);
	}
}


bool cModelsWindow::cWindowModelCatSelect::onkeydown(int key, bool shift)
{
	int i;
	bool b = cWindowTree::onkeydown(key, shift);
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
			string cat = "";
			cTreeNode* n = node;
			while(n != NULL)
			{
				cat = n->text + "/" + cat;
				n = n->parent;
			}

			for(i = 0; i < objectfiles.size(); i++)
			{
				cFile* pFile = fs.open(objectfiles[i]);
				ofstream pFile2((objectfiles[i] + ".tmp").c_str());
				while(!pFile->eof())
				{
					string line = pFile->readline();
					if(line.substr(0, cat.length()) != cat)
					{
						line += "\n";
						pFile2.write(line.c_str(), line.length());
					}
				}
				pFile->close();
				pFile2.close();
#ifdef WIN32
				DeleteFile((objectfiles[i]+".bak").c_str());
				MoveFile(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
				MoveFile((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#else
				unlink((objectfiles[i]+".bak").c_str());
				rename(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
				rename((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#endif
			}

			delete node;
			refreshmodels();
		}
		
		return true;
	}
	return false;
}

cModelsWindow::cWindowModelCatSelect::cWindowModelCatSelect(cWindow* parent, vector<cWindowTree::cTreeNode*> n) : cWindowTree(parent, n)
{
	originalselection = -1;
}

void cModelsWindow::cWindowModelCatSelect::click()
{
	cWindowTree::click();
	refreshmodels();
}
void cModelsWindow::cWindowModelCatSelect::holddragover()
{
	if(originalselection == -1)
		originalselection = selected;
	int i;
	vector<string> values;
	for(i = 0; i < nodes.size(); i++)
		nodes[i]->getdata(values);

	
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = Graphics.h()-(int)mousey;
	yy -= realy();
	yy -= parent->py();

	if (xx < w - 14)
	{ // in the box
		int s = selected;
		selected = liststart + ((h-yy-3) / 12);
		if (selected > (int)values.size() || selected < 0)
			selected = s;
	}
}

void cModelsWindow::cWindowModelCatSelect::dragover()
{
	bool ctrl = (SDL_GetModState() & KMOD_CTRL) != 0;
	int i;
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
		string orig = "";
		string dest = "";
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

		for(i = 0; i < objectfiles.size(); i++)
		{
			cFile* pFile = fs.open(objectfiles[i]);
			ofstream pFile2((objectfiles[i] + ".tmp").c_str());
			while(!pFile->eof())
			{
				string line = pFile->readline();
				string pre = line.substr(0, line.find("|"));
				string filename = line.substr(line.find("|")+1);
				string directory = pre.substr(0, pre.rfind("/"));
				if(filename == draggingobject->GetText(0) && directory == orig)
				{
					string bla = dest + "/" + draggingobject->ppopup() + "|" + draggingobject->GetText(0) + "\n";
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
			DeleteFile((objectfiles[i]+".bak").c_str());
			MoveFile(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
			MoveFile((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#else
			unlink((objectfiles[i]+".bak").c_str());
			rename(objectfiles[i].c_str(), (objectfiles[i]+".bak").c_str());
			rename((objectfiles[i]+".tmp").c_str(), objectfiles[i].c_str());
#endif
		}



		if(((cModelsWindow*)parent)->items.find(nodeorig) == ((cModelsWindow*)parent)->items.end())
			Log(1,0,"Oops");

		for(i = 0; i < ((cModelsWindow*)parent)->items[nodeorig].size(); i++)
		{
			string a = ((cModelsWindow*)parent)->items[nodeorig][i].second;
			string b = draggingobject->GetText(0);
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
	parent->stopdrag();
	draggingobject = NULL;
	draggingwindow = NULL;

}


void cModelsWindow::cWindowModelCatSelect::refreshmodels()
{
	int i;
	cWindowScrollPanel* box = (cWindowScrollPanel*)parent->objects["models"];
	for(i = 0; i < box->objects.size(); i++)
		delete box->objects[i];
	box->objects.clear();
	int a = selected;
	cWindowTree::cTreeNode* node;
	for(i = 0; i < nodes.size(); i++)
	{
		 node = nodes[i]->getnode(a);
		 if(node != NULL)
			 break;
	}

	vector<pair<string, string> > v;
	v = *((vector<pair<string, string> >*)parent->userfunc(node));

	for(i = 0; i < v.size(); i++)
	{
		pair<string, string> p = v[i];
		cWindowObject* o = new cWindowModel(parent);
		o->alignment = ALIGN_TOPLEFT;
		o->moveto(i*130, 32);
		o->resizeto(128,128);
		o->SetText(0,rodir + p.second);
		o->SetText(1,p.second);
		o->setpopup(p.first);
		box->objects.push_back(o);
	}
	parent->resizeto(parent->pw(), parent->ph());
	draggingwindow = NULL;
	draggingobject = NULL;
}

void addnode(vector<cWindowTree::cTreeNode*> &nodes, map<string, cWindowTree::cTreeNode*, less<string> > &lookup, string cat)
{
	if(lookup.find(cat) == lookup.end())
	{
		if(cat.rfind("/") != string::npos)
		{
			string p = cat.substr(0, cat.rfind("/"));
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

cModelsWindow::cModelsWindow(cTexture* t, cFont* f) : cWindow(t,f)
{
	wtype = WT_MODELS;
	closetype = HIDE;
	resizable = true;
	visible = true;
	modal = false;

	h = Graphics.h()-50;
	w = Graphics.w()-50;
	title = msgtable[WINDOW_MODELS_TITLE];
	center();

	cWindowObject* o;

	vector<cWindowTree::cTreeNode*> nodes;
	map<string, cWindowTree::cTreeNode*, less<string> > lookup;

	for(int i = 0; i < objectfiles.size(); i++)
	{
		cFile* pFile = fs.open(objectfiles[i]);
		while(!pFile->eof())
		{
			string line = pFile->readline();
			if (line == "")
				continue;
			string pre = line.substr(0, line.find("|"));
			string filename = line.substr(line.find("|")+1);

			string cat = pre.substr(0, pre.rfind("/"));
			string name = pre.substr(pre.rfind("/")+1);

			if(lookup.find(cat) == lookup.end())
			{
				addnode(nodes, lookup, cat);
			}
			if(filename != "")
			{
				if (items.find(lookup[cat]) == items.end())
				{
					items[lookup[cat]] = vector<pair<string, string> >();
				}
				items[lookup[cat]].push_back(pair<string,string>(name,filename));
			}
		}
	}

	o = new cWindowModelCatSelect(this, nodes);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(20,20);
	o->resizeto(400,400);
	objects["tree"] = o;

/*		o = new cWindowListBox(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(20,20);
	o->resizeto(100,100);
	objects["textures"] = o;*/

	o = new cWindowScrollPanel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(20, 20);
	o->resizeto(100,100);
	objects["models"] = o;

	o = new cWindowModel(this);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(9999,9999);
	o->resizeto(128,128);
	//o->SetText(0,"data/dragger.tga");
	objects["zdragger"] = o;

//	objects["rollup"] = new cWindowRollupButton(this);
//	objects["close"] = new cWindowCloseButton(this);

	resizeto(w,h);
}	

void cModelsWindow::resizeto(int ww, int hh)
{
	cWindow::resizeto(ww,hh);
	objects["tree"]->resizeto(200, hh-30);
	cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["models"];
	panel->moveto(220, 20);
	panel->resizeto(ww-220, hh-30);
	panel->innerwidth = ww-220;

	int x = 0;
	int y = 0;
	for(int i = 0; i < panel->objects.size(); i++)
	{
		panel->objects[i]->moveto(x,y);
		x+=130;
		if(x+130 > panel->innerwidth-18)
		{
			x = 0;
			y += 130;
		}
	}
	panel->scrollposx = 0;
	panel->scrollposy = 0;
	panel->innerheight = y+130;
}
void cModelsWindow::stopdrag()
{
	objects["zdragger"]->moveto(-4000,-4000);
	objects["zdragger"]->SetInt(0,0);
	((cWindowScrollPanel*)objects["models"])->draggingobject = NULL;
	draggingobject = NULL;

}	

void* cModelsWindow::userfunc(void* param)
{
	return &items[(cWindowTree::cTreeNode*)param];
}

