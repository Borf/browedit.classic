#ifndef __WINDOWLINE_H__
#define __WINDOWLINE_H__

#include "windowobject.h"
using namespace std;

class cWindowLine : public cWindowObject
{
protected:
	int r,g,b;
public:
	cWindowLine()
	{
		w = 100;
		h = 100;
		x = 5;
		y = 20;
		alignment = ALIGN_BOTTOMLEFT;
		type = OBJECT_LINE;
		r = (g = (b = 0));
	}
	void draw()
	{
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);
		glColor3i(r,g,b);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
			glVertex2f(min(x, parent->pw()),max(parent->ph() - y, 0));
			glVertex2f(min(w, parent->pw()),max(parent->ph() - h, 0));
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor4fv(colors);
	}
	void SetInt(int index, int val)
	{
		if (index == 0)
			x = val;
		else if (index == 1)
			y = val;
		else if (index == 2)
			w = val;
		else if (index == 3)
			h = val;
		else if (index == 4)
			r = val;
		else if (index == 5)
			g = val;
		else if (index == 6)
			b = val;
	}

	bool inobject()
	{
		int xx=mousex-parent->px();
		int yy=mousey-parent->py();
		if (xx > realx() && xx < w &&
			yy > realy() && yy < h)
			return true;
		return false;
	}
};




#endif