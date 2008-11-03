#include "windowroundbutton.h"
#include "window.h"
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <graphics.h>
extern cGraphicsBase Graphics;

void cWindowRoundButton::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	if (this->inObject())
		glColor3f(0.75f, 0.75f, 1.0f);
	glTranslatef((float)realX(), (float)realy2(), 0);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	glBegin(GL_QUADS);
		glTexCoord2f(258.0f/512.0f,	211.0f/512.0f);		glVertex2d(0,0);
		glTexCoord2f(386.0f/512.0f,	211.0f/512.0f);		glVertex2d(w,0);
		glTexCoord2f(386.0f/512.0f,	339.0f/512.0f);		glVertex2d(w,h);
		glTexCoord2f(258.0f/512.0f,	339.0f/512.0f);		glVertex2d(0,h);		
	glEnd();
	glTranslatef(-(float)realX(), -(float)realy2(), 0);
	glColor4fv(colors);
}


void cWindowRoundButton::onClick()
{
	Log(1,0,"You clicked the button!");
}

cWindowObject* cWindowRoundButton::inObject()
{
	int xx=(int)mouseX-parent->getX();
	int yy=(Graphics.h()-(int)mouseY)-parent->getY();
	int xxx = realX() + (w/2);
	int yyy = realY() + (h/2);
	if (((xx - xxx)*(xx - xxx))+((yy - yyy)*(yy - yyy)) < (w*w)/2)
		return this;
	return NULL;
}

cWindowRoundButton::cWindowRoundButton( cWindow* parent ) : cWindowObject(parent)
{
	cursorType = 1;
	type = OBJECT_ROUNDBUTTON;
}