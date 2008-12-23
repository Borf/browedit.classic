#include <common.h>
#include "hotkeywindow.h"
#include <graphics.h>


cHotkeyWindow::cHotkeyButton::cHotkeyButton( cWindow* parent ) : cWindowPictureBox(parent)
{
	alignment = ALIGN_TOPLEFT;	
	im = NULL;
}

void cHotkeyWindow::cHotkeyButton::onRightClick()
{
	if (loaded)
	{
		glDeleteTextures(1, &tid);
		loaded = false;
		delete[] im;
		return;
	}
	char* image = new char[cGraphics::w()*cGraphics::h()*3];
	glReadPixels( 0, 0, cGraphics::w(), cGraphics::h(), GL_RGB, GL_UNSIGNED_BYTE, image );
	
	im = new char[256*256*3];
	
	float factorx = cGraphics::w() / 256.0f;
	float factory = cGraphics::h() / 256.0f;
	
	for(int x = 0; x < 256; x++)
	{
		for(int y = 0; y < 256; y++)
		{
			im[3*(x+y*256)] = image[3*(int)(floor(x*factorx)+floor(y*factory)*cGraphics::w())];
			im[3*(x+y*256)+1] = image[3*(int)(floor(x*factorx)+floor(y*factory)*cGraphics::w())+1];
			im[3*(x+y*256)+2] = image[3*(int)(floor(x*factorx)+floor(y*factory)*cGraphics::w())+2];
		}
	}
	delete[] image;
	image = NULL;
	userfunc(NULL);
	loaded = true;
	
	camerapointer = cGraphics::worldContainer->camera.pointer;
	camerarot = cGraphics::worldContainer->camera.rot;
	cameraheight = cGraphics::worldContainer->camera.height;
	cameraangle = cGraphics::worldContainer->camera.angle;
	topcamera = cGraphics::worldContainer->view.topCamera;
	sidecamera = cGraphics::worldContainer->view.sideCamera;
}

void cHotkeyWindow::cHotkeyButton::onClick()
{
	if(loaded)
	{
		cGraphics::worldContainer->camera.pointer = camerapointer;
		cGraphics::worldContainer->camera.rot = camerarot;
		cGraphics::worldContainer->camera.height = cameraheight;
		cGraphics::worldContainer->camera.angle = cameraangle;
		cGraphics::worldContainer->view.topCamera = topcamera;
		cGraphics::worldContainer->view.sideCamera = sidecamera;
	}
}

cWindowObject* cHotkeyWindow::cHotkeyButton::inObject()
{
	int xx=(int)cGraphics::cMouse::x-parent->getX();
	int yy=(cGraphics::h()-(int)cGraphics::cMouse::y)-parent->getY();
	if (xx > getX() && xx < getX()+w &&
		yy > getY() && yy < getY()+h)
		return this;
	return NULL;
}

cHotkeyWindow::cHotkeyButton::~cHotkeyButton()
{
	if(loaded)
	{
		glDeleteTextures(1, &tid);
		loaded = false;
	}
	if(im)
		delete[] im;
}

void cHotkeyWindow::cHotkeyButton::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = getX();
	yy = getY();
	if (loaded)
	{
		if (highlightmode == 1)
		{
			if (inObject() && parent->istopwindow())
				glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(0.2f, 0.2f, 0.2f);
		}
		else if (highlightmode == 2)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (highlightmode == 3)
		{
			if (inObject() && parent->istopwindow())
				glColor3f(0.7f, 0.7f, 0.7f);
			else
				glColor3f(1.0f, 1.0f, 1.0f);
		}
		else if (highlightmode == 4)
			glColor3f(0.3f, 0.3f, 0.3f);
		
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tid);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);		glVertex2d(xx, yy);
		glTexCoord2f(1,0);		glVertex2d(xx+h, yy);
		glTexCoord2f(1,1);		glVertex2d(xx+h, yy+w);
		glTexCoord2f(0,1);		glVertex2d(xx, yy+w);
		glEnd();
		
		if(inObject())
		{
			int xxx = xx;
			int yyy = yy+16;
			if(xxx+parent->getX()+256 > cGraphics::w())
				xxx = cGraphics::w() - 256 - parent->getX();
			if(yyy+parent->getY()+256*(cGraphics::h()/(float)cGraphics::w()) > cGraphics::h())
				yyy = cGraphics::h() - 256 - parent->getY();
			
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);		glVertex3d(xxx, yyy,100);
			glTexCoord2f(1,0);		glVertex3d(xxx+256, yyy,100);
			glTexCoord2f(1,1);		glVertex3d(xxx+256, yyy+256*(cGraphics::h()/(float)cGraphics::w()),100);
			glTexCoord2f(0,1);		glVertex3d(xxx, yyy+256*(cGraphics::h()/(float)cGraphics::w()),100);
			glEnd();
		}
		
	}
	glColor3f(0,0,0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glVertex2d(xx, yy);
	glVertex2d(xx+h, yy);
	glVertex2d(xx+h, yy+w);
	glVertex2d(xx, yy+w);
	glEnd();			
	glColor4fv(colors);
}

void* cHotkeyWindow::cHotkeyButton::userfunc( void* param )
{
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D,0,3,256,256,0,GL_RGB,GL_UNSIGNED_BYTE,im);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return NULL;
}

cHotkeyWindow::cHotkeyWindow(cWorldContainer* container) : cWindow()
{
	worldContainer = container;
	windowType = WT_HOTKEY;
	resizable = false;
	visible = true;
	modal = false;
	
	h = 40;
	w = 264+16;
	x = cGraphics::w()-w;
	y = 0;
	title = "";
	initProps("hotbar");
	
	cWindowObject* o;
	
	TiXmlElement* wSkin = cWM::skin.FirstChildElement("skin")->FirstChildElement("miniwindow");
	
	skinTopHeight = atoi(wSkin->FirstChildElement("top")->Attribute("height"));
	skinTop =		512 - atoi(wSkin->FirstChildElement("top")->FirstChild()->Value());
	skinBottomHeight = atoi(wSkin->FirstChildElement("bottom")->Attribute("height"));
	skinBottom =		512 - atoi(wSkin->FirstChildElement("bottom")->FirstChild()->Value());
	
	skinLeftWidth = atoi(wSkin->FirstChildElement("left")->Attribute("width"));
	skinLeft =		atoi(wSkin->FirstChildElement("left")->FirstChild()->Value());
	skinRightWidth = atoi(wSkin->FirstChildElement("right")->Attribute("width"));
	skinRight =		atoi(wSkin->FirstChildElement("right")->FirstChild()->Value());		
	
	for(int i = 0; i < 8; i++)
	{
		char buf[10];
		sprintf(buf, "obj%i", i);
		o = new cHotkeyButton(this);
		o->moveTo(20+32*i, 4);
		o->resizeTo(32,32);
		objects[buf] = o;
	}
}
