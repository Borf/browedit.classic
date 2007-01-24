#include "windowmainbutton.h"
#include "window.h"
#include <math.h>

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowMainButton::draw()
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realx();
	yy = realy2();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	if (this->inobject() && parent->istopwindow())
		glColor3f(0.75f, 0.75f, 1.0f);

	glTranslatef((float)xx, (float)yy, 0);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);
		glTexCoord2f(            0,	197.0f/512.0f);		glVertex2d(0,0);
		glTexCoord2f(99.0f/512.0f,	197.0f/512.0f);		glVertex2d(w,0);
		glTexCoord2f(99.0f/512.0f,	211.0f/512.0f);		glVertex2d(w,h);
		glTexCoord2f(            0,	211.0f/512.0f);		glVertex2d(0,h);		
	glEnd();
	glTranslatef(-(float)realx(), -(float)realy2(), 0);

	parent->font->print(0.0f,0.0f,0.0f, (float)(parent->px() + xx+5), (float)(parent->py() + yy), text.c_str());
	glColor4fv(colors);

}


void cWindowMainButton::click()
{
	Log(1,0, "You clicked the button!");
}


void cWindowMainButton::SetText(int id, string s)
{ 
	text = s;
}

string cWindowMainButton::GetText(int id)
{
	return text;
}
