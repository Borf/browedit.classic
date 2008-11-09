#include "windowpicturebox.h"
#include "window.h"
#include <texturecache.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowPictureBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	if (loaded)
	{
		if (tex->texId() != 0)
		{
			GLfloat colors[4];
			glGetFloatv(GL_CURRENT_COLOR, colors);
			int xx, yy;
			xx = realX();
			yy = realY();
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
				glColor3f(0.6f, 0.6f, 0.6f);
			else if (highlightmode == 5)
				glColor3f(0.2f, 0.2f, 0.2f);


			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->texId());
			glBegin(GL_QUADS);
/*				glTexCoord2f(0,cutoffbottom / (float)h);		glVertex2d(xx+cutoffleft,		yy+cutoffbottom);
				glTexCoord2f(1,cutoffbottom / (float)h);		glVertex2d(xx+w-cutoffright,	yy+cutoffbottom);
				glTexCoord2f(1,1 - cutofftop / (float)h);			glVertex2d(xx+w-cutoffright,	yy+h-cutofftop);
				glTexCoord2f(0,1 - cutofftop / (float)h);			glVertex2d(xx+cutoffleft,		yy+h-cutofftop);*/
				glTexCoord2f(0,0);		glVertex2d(xx,		yy);
				glTexCoord2f(1,0);		glVertex2d(xx+w,	yy);
				glTexCoord2f(1,1);		glVertex2d(xx+w,	yy+h);
				glTexCoord2f(0,1);		glVertex2d(xx,		yy+h);
			glEnd();
			glColor4fv(colors);
		}
	}
}


void cWindowPictureBox::onClick()
{
//	MessageBox(NULL, "You clicked the PictureBox!", "", MB_OK);
}


void cWindowPictureBox::setText(int i, std::string fil)
{
	if(i == 0)
	{
		loaded = true;
		tex = cTextureCache::load(fil);
	}
}

void cWindowPictureBox::setInt(int i, int v)
{
	highlightmode = v;
}

int cWindowPictureBox::getInt(int i)
{
	return highlightmode;
}

cWindowPictureBox::cWindowPictureBox( cWindow* parent ) : cWindowObject(parent)
{
	highlightmode = 0;
	w = 100;
	h = 100;
	x = 5;
	y = 20;
	tex = NULL;
	alignment = ALIGN_TOPLEFT;
	loaded = false;
	type = OBJECT_PICTUREBOX;
}

cWindowPictureBox::~cWindowPictureBox()
{
	if (tex != NULL || loaded)
		cTextureCache::unload(tex);
}
