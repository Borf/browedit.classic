#include "windowbutton.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowButton::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realx();
	yy = realy();
	glEnable(GL_TEXTURE_2D);

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	
	if (this->inobject())
		glColor4f(0.75f, 0.75f, 1.0f, colors[3]);
	if(!enabled)
		glColor4f(0.25, 0.25, 0.25, colors[3]);

	cWindowObject::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	parent->font->print(fontcolor[0], fontcolor[1], fontcolor[2], parent->px() + xx + (w/2)- (parent->font->textlen(text)/2), parent->py() + yy + (h/2)-6, text.c_str());
	glColor4fv(colors);
}


void cWindowButton::click()
{
	Log(1,0, "You clicked the button!");
}

bool cWindowButton::onkeydown(int c, bool shift)
{
	if (c == SDLK_RETURN)
	{
		click();
		return true;
	}
	return false;
}
