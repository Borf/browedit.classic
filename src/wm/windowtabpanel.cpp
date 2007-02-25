#include "windowtabpanel.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include "../graphics.h"
extern cGraphics Graphics;

void cWindowTabPanel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int i;
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realx();
	yy = realy2();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	bool overtabs = false;
	if (this->inobject() && parent->istopwindow() && ((Graphics.h()-mousey) - parent->py() - yy - h > -14))
		overtabs = true;

	glTranslatef((float)xx, (float)yy, 0);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	for(i = 0; i < tabs.size(); i++)
	{
		float left = (w/(float)tabs.size()) * i;
		float right = (w/(float)tabs.size()) * (i+1);
		if (overtabs && mousex - parent->px() - xx > left && mousex - parent->px() - xx < right)
			glColor3f(0.65f, 0.65f, 1.0f);
		else if (i == selectedtab)
			glColor3f(0.85f, 0.85f, 1.0f);
		else
			glColor4fv(colors);


		glBegin(GL_QUADS);
			glTexCoord2f(            0,	197.0f/512.0f);		glVertex2d(left,h-14);
			glTexCoord2f(99.0f/512.0f,	197.0f/512.0f);		glVertex2d(right,h-14);
			glTexCoord2f(99.0f/512.0f,	211.0f/512.0f);		glVertex2d(right,h);
			glTexCoord2f(            0,	211.0f/512.0f);		glVertex2d(left,h);		
		glEnd();	
	}	
	
	glTranslatef(-(float)realx(), -(float)realy2(), 0);

	for(i = 0; i < tabs.size(); i++)
		parent->font->print(0,0,0, parent->px() + xx+5 + (w / (float)tabs.size()) * i, parent->py() + yy+h-16, tabs[i].c_str());

	glColor4fv(colors);
}

void cWindowTabPanel::click()
{
	if(tabs.size() == 0)
		return;
	int xx, yy;
	xx = realx();
	yy = realy2();
	if (this->inobject() && parent->istopwindow() && ((Graphics.h()-mousey) - parent->py() - yy - h > -14))
	{
		int oldtab = selectedtab;
		selectedtab = (mousex - parent->px() - xx) / (w/tabs.size());
		if (selectedtab != oldtab)
			tabchange(oldtab);
	}
}

