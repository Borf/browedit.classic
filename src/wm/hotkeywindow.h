#ifndef __HOTKEYWINDOW_H__
#define __HOTKEYWINDOW_H__

#include "window.h"

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowbutton.h"
#include "windowpicturebox.h"
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
		void rightclick()
		{
			if (loaded)
			{
				glDeleteTextures(1, &tid);
				loaded = false;
				delete[] im;
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
			topcamera = Graphics.topcamera;
		}

		void click()
		{
			if(loaded)
			{
				Graphics.camerapointer = camerapointer;
				Graphics.camerarot = camerarot;
				Graphics.cameraheight = cameraheight;
				Graphics.cameraangle = cameraangle;
				Graphics.topcamera = topcamera;
			}
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
			xx = realx();
			yy = realy();
			if (loaded)
			{
				if (highlightmode == 1)
				{
					if (inobject() && parent->istopwindow())
						glColor3f(1.0f, 1.0f, 1.0f);
					else
						glColor3f(0.2f, 0.2f, 0.2f);
				}
				else if (highlightmode == 2)
					glColor3f(1.0f, 1.0f, 1.0f);
				else if (highlightmode == 3)
				{
					if (inobject() && parent->istopwindow())
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
				
				if(inobject())
				{
					int xxx = xx;
					int yyy = yy+16;
					if(xxx+parent->px()+256 > Graphics.w())
						xxx = Graphics.w() - 256 - parent->px();
					if(yyy+parent->py()+256*(Graphics.h()/(float)Graphics.w()) > Graphics.h())
						yyy = Graphics.h() - 256 - parent->py();

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



	cHotkeyWindow(cTexture* t, cFont* f) : cWindow(t,f)
	{
		wtype = WT_HOTKEY;
		resizable = false;
		visible = true;
		modal = false;

		h = 40;
		w = 264+16;
		x = Graphics.w()-w;
		y = 0;
		title = "Hotkey";

		cWindowObject* o;

		for(int i = 0; i < 8; i++)
		{
			char buf[10];
			sprintf(buf, "obj%i", i);
			o = new cHotkeyButton(this);
			o->moveto(20+32*i, 4);
			o->resizeto(32,32);
			objects[buf] = o;
		}


	}	

	void draw()
	{
		glTranslatef(x, y, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->texid());
		glBegin(GL_QUADS);
			glTexCoord2f(0,				496.0f/512.0f);		glVertex2d(0,  h-16);
			glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3, h-16);
			glTexCoord2f(3/512.0f,		1);					glVertex2d(3, h);
			glTexCoord2f(0,				1);					glVertex2d(0,  h);

			glTexCoord2f(3/512.0f,		496.0f/512.0f);		glVertex2d(3,  h-16);
			glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3, h-16);
			glTexCoord2f(509/512.0f,	1);					glVertex2d(w-3, h);
			glTexCoord2f(3/512.0f,		1);					glVertex2d(3,  h);

			glTexCoord2f(509/512.0f,	496.0f/512.0f);		glVertex2d(w-3,  h-16);
			glTexCoord2f(1,				496.0f/512.0f);		glVertex2d(w, h-16);
			glTexCoord2f(1,				1);					glVertex2d(w, h);
			glTexCoord2f(509/512.0f,	1);					glVertex2d(w-3,  h);

			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3, 4);
			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3, h-16);
			glTexCoord2f(0,				493.0f/512.0f);		glVertex2d(0,  h-16);

			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3,  4);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3, 4);
			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3, h-16);
			glTexCoord2f(3/512.0f,		493.0f/512.0f);		glVertex2d(3,  h-16);

			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3,  4);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(1,				493.0f/512.0f);		glVertex2d(w, h-16);
			glTexCoord2f(509/512.0f,	493.0f/512.0f);		glVertex2d(w-3,  h-16);

			glTexCoord2f(0,				470.0f/512.0f);		glVertex2d(0,  0);
			glTexCoord2f(3/512.0f,		470.0f/512.0f);		glVertex2d(3, 0);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3, 4);
			glTexCoord2f(0,				474.0f/512.0f);		glVertex2d(0,  4);

			glTexCoord2f(3/512.0f,		470.0f/512.0f);		glVertex2d(3,  0);
			glTexCoord2f(509/512.0f,	470.0f/512.0f);		glVertex2d(w-3, 0);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3, 4);
			glTexCoord2f(3/512.0f,		474.0f/512.0f);		glVertex2d(3,  4);

			glTexCoord2f(509/512.0f,	470.0f/512.0f);		glVertex2d(w-3,  0);
			glTexCoord2f(1,				470.0f/512.0f);		glVertex2d(w, 0);
			glTexCoord2f(1,				474.0f/512.0f);		glVertex2d(w, 4);
			glTexCoord2f(509/512.0f,	474.0f/512.0f);		glVertex2d(w-3,  4);
		glEnd();

		
		map<string, cWindowObject*, less<string> >::iterator i;
		for(i = objects.begin(); i != objects.end(); i++)
		{
			cWindowObject* o = i->second;
			if (!rolledup || o->realy2() > h-19)
			{
				//if(i->second->realy() > 0)
					o->draw();
			}
		}
		glTranslatef(-x, -y, 0);
	}
};

#endif
