#include "windowobject.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "../graphics.h"

extern cGraphics Graphics;
/*void cWindowObject::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
}*/




cWindowObject* cWindowObject::inobject()
{
	int xx=(int)mousex-parent->px();
	int yy=(Graphics.h()-(int)mousey)-parent->py();
	if (xx > realx() && xx < realx()+w &&
		yy > realy() && yy < realy()+h)
		return this;
	return NULL;
}




void cWindowObject::click()
{

}


int cWindowObject::realx()
{
	if((alignment & ALIGN_HCENTER) != 0)
		return (parent->pw()/2)-(w/2)+x;
	else if ((alignment & ALIGN_HLEFT) != 0)
		return x;
	else if ((alignment & ALIGN_HRIGHT) != 0)
		return parent->pw()-x-w;
	return 0;
}
int cWindowObject::realy()
{
	if((alignment & ALIGN_VCENTER) != 0)
		return (parent->ph()/2)-(h/2)+y;
	else if ((alignment & ALIGN_VBOTTOM) != 0)
		return y;
	else if ((alignment & ALIGN_VTOP) != 0)
		return parent->ph()-y-h;
	return 0;
}
int cWindowObject::realy2()
{
	if((alignment & ALIGN_VCENTER) != 0)
		return (parent->ph2()/2)-(h/2)+y;
	else if ((alignment & ALIGN_VBOTTOM) != 0)
		return y;
	else if ((alignment & ALIGN_VTOP) != 0)
		return parent->ph2()-y-h;
	return 0;
}
