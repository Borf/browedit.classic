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


cRSMEditWindow::cWindowOpenButton::cWindowOpenButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
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
	
	strcpy(((cRSMEditWindow*)parent)->filename, replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
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

cRSMEditWindow::cWindowSaveButton::cWindowSaveButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
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
	int i;
	unsigned int ii;
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
	
	pFile2.close();
}

cRSMEditWindow::cWindowSaveAsButton::cWindowSaveAsButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	moveTo(100, 0);
	resizeTo(50, 20);
	text = "Save As";
}

void cRSMEditWindow::cWindowSaveAsButton::onClick()
{
	std::string oldfilename = ((cRSMEditWindow*)parent)->filename;
	unsigned int i;
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
	
	for(i = 0; (int)i < nTextures; i++)
		pFile->read(buffer, 40);
	
	std::string rest = "";
	while(!pFile->eof())
	{
		char buf[1024];
		int read = pFile->read(buf, 1024);
		rest += std::string(buf, read);
	}
	
	pFile->close();
	
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
				strcpy(((cRSMEditWindow*)parent)->filename, replace(((cRSMEditWindow*)parent)->filename, "/", "\\").c_str());
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
					
					
					std::ofstream pFile2;
					pFile2.open(((cRSMEditWindow*)parent)->filename, std::ios_base::out | std::ios_base::binary);
					pFile2.write(header, header[5] == 4 ? 31 : 30);
					
					
					cWindowModel* model = ((cWindowModel*)parent->objects["model"]);
					long nTextures = model->model->textures.size();
					pFile2.write((char*)&nTextures, 4);
					for(i = 0; i < model->model->textures.size(); i++)
					{
						char bufje[40];
						ZeroMemory(bufje, 40);
						std::string t = model->model->textures[i]->getfilename();
						t = t.substr(cSettings::roDir.length()+13);
						strcpy(bufje, t.c_str());
						pFile2.write(bufje, 40);
					}
					
					pFile2.write(rest.c_str(), rest.length()-1);
					
					pFile2.close();
					
					
					
					
					
				}
#endif
}

cRSMEditWindow::cWindowModel::cWindowModel( cWindow* parent ) : cWindowObject(parent)
{
	type = OBJECT_MODEL;
	model = NULL;
	roty=0;
	oldy = -1;
	backgroundcolor = cVector3(1,1,1);
	rotate = 0;
}

void cRSMEditWindow::cWindowModel::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	unsigned int i;
	if(cutoffbottom < 0)
		cutoffbottom = 0;
	glEnable(GL_DEPTH_TEST);
	if(data != "" && model == NULL)
	{
		glPushMatrix();
		glLoadIdentity();
		model = new cRSMModel();
		model->load(data);
		model->pos = cVector3(0,0.7f*w,1000);
		
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
		
		
		cWindowScrollPanel* scroll = (cWindowScrollPanel*)parent->objects["scroll"];
		
		for(i = 0; i < model->textures.size(); i++)
		{
			cWindowObject* o = new cWindowModelTexture(parent,i);
			o->setText(0, model->textures[i]->getfilename());
			o->moveTo(0, 130*i);
			scroll->objects.push_back(o);
			scroll->innerheight = 130*i+130;

			o = new cWindowLabel(parent);
			o->setText(0, "#000000" + model->textures[i]->getfilename().substr(cSettings::roDir.length() + 13));
			o->moveTo(5,130*i+111);
			o->alignment = ALIGN_TOPLEFT;
			scroll->objects.push_back(o);

			o = new cWindowLabel(parent);
			o->setText(0, "#FF0000" + model->textures[i]->getfilename().substr(cSettings::roDir.length() + 13));
			o->moveTo(4,130*i+110);
			o->alignment = ALIGN_TOPLEFT;
			scroll->objects.push_back(o);
		
		}
		scroll->scrollposy = 0;
		scroll->scrollposx = 0;
		scroll->innerwidth = 128;
		
		
		
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
					model->draw(false);
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
	strcpy(filename, std::string(cSettings::roDir + "data\\model\\프론테라\\분수대.rsm").c_str());
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
	
	objects["rgbpicker"] = new cRGBPicker(this);
	
	
	open();
}

void cRSMEditWindow::resizeTo( int ww, int hh )
{
	cWindow::resizeTo(ww,hh);
	objects["model"]->resizeTo(innerWidth()-140, innerHeight()-20);
	objects["scroll"]->moveTo(innerWidth()-140, 20);
	objects["scroll"]->resizeTo(140, innerHeight()-20);
	objects["rgbpicker"]->resizeTo(innerWidth()-150,20);
}

void cRSMEditWindow::open()
{
	if (objects.find("model") != objects.end())
	{
		delete objects["model"];
		objects.erase(objects.find("model"));
	}
	
	cWindowModel* o = new cWindowModel(this);
	o->setText(1, filename);
	o->moveTo(0,20);
	o->resizeTo(innerWidth()-140, innerHeight()-20);
	o->alignment = ALIGN_TOPLEFT;
	
	objects["model"] = o;
	
	cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];
	
	for(unsigned int i = 0; i < scroll->objects.size(); i++)
		delete scroll->objects[i];
	scroll->objects.clear();
}

void cRSMEditWindow::onStopDrag()
{
	((cWindowModel*)objects["model"])->oldy = -1;
}

void cRSMEditWindow::changetexture( std::string newtexture )
{
	cWindowModel* model = ((cWindowModel*)objects["model"]);
	
	Log(3,0,"Old tid: %i", model->model->textures[selected]->texId());
	cTextureCache::unload(model->model->textures[selected]);
	model->model->textures[selected] = cTextureCache::load(cSettings::roDir + newtexture);
	Log(3,0,"new vtid: %i", model->model->textures[selected]->texId());
	
	cWindowScrollPanel* scroll = (cWindowScrollPanel*)objects["scroll"];
	delete scroll->objects[selected];
	cWindowObject* o = new cWindowModelTexture(this,selected);
	o->setText(0, cSettings::roDir + newtexture);
	o->moveTo(0, 130*selected);
	scroll->objects[selected] = o;
}
