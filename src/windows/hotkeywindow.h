#ifndef __HOTKEYWINDOW_H__
#define __HOTKEYWINDOW_H__

#include <wm/window.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowpicturebox.h>
#include "../graphics.h"
extern cGraphics Graphics;

class cHotkeyWindow : public cWindow
{
public:
	class cHotkeyButton : public cWindowPictureBox
	{
	public:
		GLuint tid;
		char* im;
			
		cVector2 camerapointer;
		float camerarot;
		float cameraheight;
		float cameraangle;
		bool topcamera;

		cHotkeyButton(cWindow* parent) : cWindowPictureBox(parent)
		{
			alignment = ALIGN_TOPLEFT;	
			im = NULL;
		}
		void rightClick()
		{
			if (loaded)
			{
				glDeleteTextures(1, &tid);
				loaded = false;
				delete[] im;
				return;
			}
			char* image = new char[Graphics.w()*Graphics.h()*3];
			glReadPixels( 0, 0, Graphics.w(), Graphics.h(), GL_RGB, GL_UNSIGNED_BYTE, image );
			
			im = new char[256*256*3];

			float factorx = Graphics.w() / 256.0f;
			float factory = Graphics.h() / 256.0f;

			for(int x = 0; x < 256; x++)
			{
				for(int y = 0; y < 256; y++)
				{
					im[3*(x+y*256)] = image[3*(int)(floor(x*factorx)+floor(y*factory)*Graphics.w())];
					im[3*(x+y*256)+1] = image[3*(int)(floor(x*factorx)+floor(y*factory)*Graphics.w())+1];
					im[3*(x+y*256)+2] = image[3*(int)(floor(x*factorx)+floor(y*factory)*Graphics.w())+2];
				}
			}
			delete[] image;
			userfunc(NULL);
			loaded = true;
			
			camerapointer = Graphics.camerapointer;
			camerarot = Graphics.camerarot;
			cameraheight = Graphics.cameraheight;
			cameraangle = Graphics.cameraangle;
			topcamera = Graphics.topCamera;
		}

		void click()
		{
			if(loaded)
			{
				Graphics.camerapointer = camerapointer;
				Graphics.camerarot = camerarot;
				Graphics.cameraheight = cameraheight;
				Graphics.cameraangle = cameraangle;
				Graphics.topCamera = topcamera;
			}
		}

		cWindowObject* inObject()
		{
			int xx=(int)mouseX-parent->getX();
			int yy=(Graphics.h()-(int)mouseY)-parent->getY();
			if (xx > getX() && xx < getX()+w &&
				yy > getY() && yy < getY()+h)
				return this;
			return NULL;
		}

		~cHotkeyButton()
		{
			if(loaded)
			{
				glDeleteTextures(1, &tid);
				loaded = false;
				delete[] im;
			}
		}

		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
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
					if(xxx+parent->getX()+256 > Graphics.w())
						xxx = Graphics.w() - 256 - parent->getX();
					if(yyy+parent->getY()+256*(Graphics.h()/(float)Graphics.w()) > Graphics.h())
						yyy = Graphics.h() - 256 - parent->getY();

					glBegin(GL_QUADS);
						glTexCoord2f(0,0);		glVertex3d(xxx, yyy,100);
						glTexCoord2f(1,0);		glVertex3d(xxx+256, yyy,100);
						glTexCoord2f(1,1);		glVertex3d(xxx+256, yyy+256*(Graphics.h()/(float)Graphics.w()),100);
						glTexCoord2f(0,1);		glVertex3d(xxx, yyy+256*(Graphics.h()/(float)Graphics.w()),100);
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

		void* userfunc(void* param)
		{
			glGenTextures(1, &tid);
			glBindTexture(GL_TEXTURE_2D, tid);
			glTexImage2D(GL_TEXTURE_2D,0,3,256,256,0,GL_RGB,GL_UNSIGNED_BYTE,im);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			return NULL;
		}
	};



	cHotkeyWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		windowType = WT_HOTKEY;
		resizable = false;
		visible = true;
		modal = false;

		h = 40;
		w = 264+16;
		x = Graphics.w()-w;
		y = 0;
		title = "";
		initprops("hotbar");

		cWindowObject* o;

		TiXmlElement* wSkin = skin.FirstChildElement("skin")->FirstChildElement("miniwindow");

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

};

#endif
