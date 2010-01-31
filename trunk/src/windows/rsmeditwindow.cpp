#include "rsmeditwindow.h"
#include <wm/windowlabel.h>

#include <common.h>
#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#endif
#include <graphics.h>
#include <fstream>
#include <SDL/SDL_syswm.h>
#include <common.h>
#include <settings.h>
#include <RSMModel.h>
#include <windows/rsmmeshprops.h>

#include <bengine/util.h>
#include <bengine/math/math.h>
#include <bengine/texture.h>
#include <bengine/texturecache.h>

cRSMEditWindow::cWindowOpenButton::cWindowOpenButton( cWindow* parent, Json::Value &skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0, 0);
	resizeTo(50, 20);
	text = "Open";
}

void cRSMEditWindow::cWindowOpenButton::onClick()
{
#ifdef WIN32
	char curdir[255];
	getcwd(curdir, 255);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version)	;
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	
	strcpy(((cRSMEditWindow*)parent)->filename, bEngine::util::replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
	ofn.lpstrFile = ((cRSMEditWindow*)parent)->filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO Models\0*.rsm\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	if (GetOpenFileName(&ofn))
	{
		
		chdir(curdir);
		((cRSMEditWindow*)parent)->open();
		
		
	}
	
	
#endif
}

cRSMEditWindow::cWindowSaveButton::cWindowSaveButton( cWindow* parent, Json::Value &skin) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(50, 0);
	resizeTo(50, 20);
	text = "Save";
}

void cRSMEditWindow::cWindowSaveButton::onClick()
{
	if(!cWM::confirmWindow("Are you sure you want to overwrite this file?"))
		return;


	((cWindowModel*)parent->objects["model"])->model->save(((cRSMEditWindow*)parent)->filename);
/*
	cFile* pFile = cFileSystem::open(((cRSMEditWindow*)parent)->filename);
	
	char buffer[100];
	char header[100];
	pFile->read(header, 6); // header
	if (header[5] == 4)
		pFile->read(header+6, 25); // unknown
	else
		pFile->read(header+6, 24); // unknown
	
	pFile->read(buffer, 4); // ntextures;
	long nTextures;
	memcpy((char*)&nTextures, buffer, 4);
	
	for(i = 0; i < nTextures; i++)
		pFile->read(buffer, 40);
	
	std::string rest = "";
	while(!pFile->eof())
	{
		char buf[1024];
		int read = pFile->read(buf, 1024);
		rest += std::string(buf, read);
	}
	
	pFile->close();
	std::ofstream pFile2;
	pFile2.open(((cRSMEditWindow*)parent)->filename, std::ios_base::out | std::ios_base::binary);
	pFile2.write(header, header[5] == 4 ? 31 : 30);
	
	
	cWindowModel* model = ((cWindowModel*)parent->objects["model"]);
	nTextures = model->model->textures.size();
	pFile2.write((char*)&nTextures, 4);
	for(ii = 0; ii < model->model->textures.size(); ii++)
	{
		char bufje[40];
		ZeroMemory(bufje, 40);
		std::string t = model->model->textures[ii]->getfilename();
		t = t.substr(cSettings::roDir.length()+13);
		strcpy(bufje, t.c_str());
		pFile2.write(bufje, 40);
	}
	
	pFile2.write(rest.c_str(), rest.length()-1);
	
	pFile2.close();*/
}

cRSMEditWindow::cWindowSaveAsButton::cWindowSaveAsButton( cWindow* parent, Json::Value &skin) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(100, 0);
	resizeTo(50, 20);
	text = "Save As";
}

void cRSMEditWindow::cWindowSaveAsButton::onClick()
{
	std::string oldfilename = ((cRSMEditWindow*)parent)->filename;
#ifdef WIN32
	char curdir[255];
	getcwd(curdir, 255);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	strcpy(((cRSMEditWindow*)parent)->filename, bEngine::util::replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
	ofn.lpstrFile = ((cRSMEditWindow*)parent)->filename;
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = "All\0*.*\0RO models\0*.rsm\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&ofn))
	{
		strcpy(((cRSMEditWindow*)parent)->filename, oldfilename.c_str());
		chdir(curdir);
	}
	else
	{
		if (strcmp(((cRSMEditWindow*)parent)->filename+strlen(((cRSMEditWindow*)parent)->filename)-4, ".rsm") != 0)
		{ 
			strcpy(((cRSMEditWindow*)parent)->filename, (std::string(((cRSMEditWindow*)parent)->filename) + ".rsm").c_str());
		}
		chdir(curdir);
		((cWindowModel*)parent->objects["model"])->model->save(((cRSMEditWindow*)parent)->filename);
	}
				
#endif
}

cRSMEditWindow::cWindowModel::cWindowModel( cWindow* parent ) : cWindowObject(parent)
{
	type = OBJECT_MODEL;
	model = NULL;
	roty=0;
	oldy = -1;
	backgroundcolor = bEngine::math::cVector3(1,1,1);
	rotate = 0;
}

void cRSMEditWindow::cWindowModel::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	if(cutoffbottom < 0)
		cutoffbottom = 0;
	glEnable(GL_DEPTH_TEST);
	
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
	
	glColor4f(0,0,0,1);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
				glVertex2d(-w,-h);
				glVertex2d(w,-h);
				glVertex2d(w,h);
				glVertex2d(-w,h);
				glEnd();
				glLineWidth(1);
				glColor4f(backgroundcolor.x, backgroundcolor.y, backgroundcolor.z,1);
				glBegin(GL_QUADS);
				glVertex2d(-w+2,-h+2);
				glVertex2d(w-2,-h+2);
				glVertex2d(w-2,h-2);
				glVertex2d(-w+2,h-2);
				glEnd();
				glColor4f(1,1,1,1);
				glEnable(GL_TEXTURE_2D);
				
				if (model != NULL)
				{
					model->draw();
					if(rotate < cGraphicsBase::getFrameTicks())
						model->rot.y+=40*(cGraphicsBase::getFrameTicks() / 1000.0f);
					model->rot.x = roty;
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

void cRSMEditWindow::cWindowModel::drag()
{
	if(oldy != -1)
	{
		if(SDL_GetModState() & KMOD_SHIFT)
		{
			model->pos.y -= (oldy - cGraphics::cMouse::y) * 2.0f;
			model->pos.x -= (oldx - cGraphics::cMouse::x) / 2.0f;
		}
		else
		{
			roty += (oldy-cGraphics::cMouse::y) / 2.0f;
			model->rot.y -= (oldx - cGraphics::cMouse::x) / 2.0f;
			rotate = tickcount() + 5000;
		}
	}
	oldy = cGraphics::cMouse::y;
	oldx = cGraphics::cMouse::x;
}

void cRSMEditWindow::cWindowModel::setText( int i, std::string s )
{
	if(i == 1)
	{
		data = s;
	}
}

std::string cRSMEditWindow::cWindowModel::getText( int i )
{
	return data;
}

void cRSMEditWindow::cWindowModel::setInt( int i, int id )
{
	if(i == 0)
		backgroundcolor.x = id/256.0f;
	else if(i == 1)
		backgroundcolor.y = id/256.0f;
	else if(i == 2)
		backgroundcolor.z = id/256.0f;
}

cRSMEditWindow::cWindowModel::~cWindowModel()
{
	if(model != NULL)
		delete model;
	model = NULL;
}

void cRSMEditWindow::cWindowModel::onScrollUp()
{
	model->scale = model->scale * 1.1f;
}

void cRSMEditWindow::cWindowModel::onScrollDown()
{
	model->scale = model->scale / 1.1f;
}

cRSMEditWindow::cWindowModelTexture::cWindowModelTexture( cWindow* parent, int ii ) : cWindowPictureBox(parent)
{
	resizeTo(128,128);
	alignment = ALIGN_TOPLEFT;
	i = ii;
}

void cRSMEditWindow::cWindowModelTexture::onClick()
{
	((cRSMEditWindow*)parent)->selected = i;
	SDL_Event ev;
	ev.type = SDL_KEYDOWN;
	ev.key.keysym.sym = SDLK_t;
	SDL_PushEvent(&ev);
	ev.type = SDL_KEYUP;
	ev.key.keysym.sym = SDLK_t;
	SDL_PushEvent(&ev);
}

cRSMEditWindow::cRGBPicker::cRGBPicker( cWindow* p ) : cWindowRGBPicker(p)
{
	moveTo(150,0);
	resizeTo(parent->innerWidth()-150,20);
	alignment = ALIGN_TOPLEFT;
}

void cRSMEditWindow::cRGBPicker::onClick()
{
	cWindowRGBPicker::onClick();
	parent->objects["model"]->setInt(0,(int)(r*256));
	parent->objects["model"]->setInt(1,(int)(g*256));
	parent->objects["model"]->setInt(2,(int)(b*256));
}

cRSMEditWindow::cRSMEditWindow( ) : cWindow()
{
	strcpy(filename, std::string(cSettings::roDir + "data\\model\\¶óÇï\\ºÐ¼ö.rsm").c_str());
	windowType = WT_RSMEDIT;
	resizable = true;
	visible = true;
	
	h = cGraphics::h()-50;
	w = cGraphics::w()-50;
	title = "RSM Editor";
	center();
	
	defaultObject = "OkButton";
	
	objects["close"] = new cWindowCloseButton(this);
	
	objects["OpenButton"] = new cWindowOpenButton(this);
	objects["SaveButton"] = new cWindowSaveButton(this);
	objects["SaveAsButton"] = new cWindowSaveAsButton(this);
	
	cWindowObject* o = new cWindowScrollPanel(this);
	o->moveTo(innerWidth()-140, 20);
	o->resizeTo(140, innerHeight()-20);
	o->alignment = ALIGN_TOPLEFT;
	((cWindowScrollPanel*)o)->innerheight = 1000;
	((cWindowScrollPanel*)o)->innerwidth = 128;
	
	objects["scroll"] = o;


	std::vector<cWindowTree::cTreeNode*> nodes;
	o = new cWindowMeshTree(this, nodes);
	o->moveTo(0,20);
	o->resizeTo(200, innerHeight()-20);
	objects["tree"] = o;

	
	objects["rgbpicker"] = new cRGBPicker(this);
	
	
	open();
}

void cRSMEditWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww,hh);
	if(objects.find("model") != objects.end())
		objects["model"]->resizeTo(innerWidth()-140-200, innerHeight()-20);
	objects["scroll"]->moveTo(innerWidth()-140, 20);
	objects["scroll"]->resizeTo(140, innerHeight()-20);
	objects["tree"]->resizeTo(200, innerHeight()-20);
	objects["rgbpicker"]->resizeTo(innerWidth()-150,20);
}


class cObjectTreeNode : public cWindowTree::cTreeNode
{
public:
	cRsmModelBase::cMesh* mesh;
	std::string getText()
	{
		return mesh->name;
	}
};

cWindowTree::cTreeNode* buildObjectTree(cRsmModelBase::cMesh* mesh)
{
	cObjectTreeNode* bla = new cObjectTreeNode();
	bla->mesh = mesh;
	bla->open = true;

	for(unsigned int i = 0; i < mesh->children.size(); i++)
		bla->children.push_back(buildObjectTree(mesh->children[i]));
	return bla;
}

void cRSMEditWindow::open()
{
	unsigned int i;
	if (objects.find("model") != objects.end())
	{
		delete objects["model"];
		objects.erase(objects.find("model"));
	}
	
	cWindowModel* o = new cWindowModel(this);
	o->setText(1, filename);
	o->moveTo(200,20);
	o->resizeTo(innerWidth()-140-200, innerHeight()-20);
	o->alignment = ALIGN_TOPLEFT;

	o->model = new cRsmModel(filename);
	o->model->pos = bEngine::math::cVector3(0,0.7f*o->getWidth(),1000);
	
	float sc = 0;
	sc = bEngine::math::max(sc, o->model->bbmax.v[0] - o->model->bbmin.v[0]);
	sc = bEngine::math::max(sc, o->model->bbmax.v[1] - o->model->bbmin.v[1]);
	sc = bEngine::math::max(sc, o->model->bbmax.v[2] - o->model->bbmin.v[2]);
	sc = 1.5f*bEngine::math::min(o->getHeight(),o->getWidth()) / sc;
	
	o->model->scale = bEngine::math::cVector3(sc,sc,sc);
	
	o->model->rot = bEngine::math::cVector3(0,0,0);
	o->roty = -15;
	
	objects["model"] = o;
	
	cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];
	for(i = 0; i < scroll->objects.size(); i++)
		delete scroll->objects[i];
	scroll->objects.clear();

	for(i = 0; i < o->model->textures.size(); i++)
	{
		cWindowObject* oo = new cWindowModelTexture(this,i);
		oo->setText(0, o->model->textures[i]->getFilename());
		oo->moveTo(0, 130*i);
		scroll->objects.push_back(oo);
		scroll->innerheight = 130*i+130;
		
		oo = new cWindowLabel(this);
		oo->setText(0, "#000000" + o->model->textures[i]->getFilename().substr(cSettings::roDir.length() + 13));
		oo->moveTo(5,130*i+111);
		oo->alignment = ALIGN_TOPLEFT;
		scroll->objects.push_back(oo);
		
		oo = new cWindowLabel(this);
		oo->setText(0, "#FF0000" + o->model->textures[i]->getFilename().substr(cSettings::roDir.length() + 13));
		oo->moveTo(4,130*i+110);
		oo->alignment = ALIGN_TOPLEFT;
		scroll->objects.push_back(oo);
		
	}
	scroll->scrollposy = 0;
	scroll->scrollposx = 0;
	scroll->innerwidth = 128;	



	cWindowTree* tree = (cWindowTree*)objects["tree"];
	if(tree->nodes.size() > 0)
		delete tree->nodes[0];
	else
		tree->nodes.push_back(NULL);
	tree->nodes[0] = buildObjectTree(o->model->root);


}

void cRSMEditWindow::onStopDrag()
{
	((cWindowModel*)objects["model"])->oldy = -1;
}

void cRSMEditWindow::changetexture( std::string newtexture )
{
	cWindowModel* model = ((cWindowModel*)objects["model"]);
	
	Log(3,0,"Old tid: %i", model->model->textures[selected]->texId());
	bEngine::cTexture* oldTexture = model->model->textures[selected];

	model->model->setTexture(oldTexture, bEngine::cTextureCache::load(cSettings::roDir + newtexture));
	bEngine::cTextureCache::unload(oldTexture);
	Log(3,0,"new vtid: %i", model->model->textures[selected]->texId());
	
	cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];
	delete scroll->objects[selected];
	cWindowObject* o = new cWindowModelTexture(this,selected);
	o->setText(0, cSettings::roDir + newtexture);
	o->moveTo(0, 130*selected);
	scroll->objects[selected] = o;
}

cRSMEditWindow::cWindowMeshTree::cWindowMeshTree( cWindow* parent, std::vector<cTreeNode*> n, Json::Value &skin) : cWindowTree(parent, n, skin)
{
	
}

void cRSMEditWindow::cWindowMeshTree::onDoubleClick()
{
	int a = selected;
	cWindowTree::cTreeNode* node = NULL;
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		node = nodes[i]->getnode(a);
		if(node != NULL)
			break;
	}
	
	cObjectTreeNode* n = (cObjectTreeNode*)node;


	cWM::addWindow(new cRsmMeshPropsWindow(n->mesh));
}