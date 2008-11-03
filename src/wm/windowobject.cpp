#include "windowobject.h"
#include "window.h"
#include <common.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <graphics.h>

extern cGraphicsBase Graphics;
void cWindowObject::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realX();
	yy = realy2();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	glBegin(GL_QUADS);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);
		glTexCoord2f(skinLeft/512.0f,					skinTop/512.0f);						glVertex2d(xx+0,				yy+h);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+h);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);

		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w,				yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+w,				yy+h);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+h);

		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);

		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+w,				yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w,				yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+w-skinRightWidth,	yy+h-skinTopHeight);
///////////////			
		glTexCoord2f(skinLeft/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+0,				yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+w-skinRightWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);

		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+w-skinRightWidth,	yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+w,				yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+w,				yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+w-skinRightWidth,	yy+skinBottomHeight);

	glEnd();
}




cWindowObject* cWindowObject::inObject()
{
	int xx=(int)mouseX-parent->getX();
	int yy=(Graphics.h()-(int)mouseY)-parent->getY();
	if (xx > realX() && xx < realX()+w &&
		yy > realY() && yy < realY()+h)
		return this;
	return NULL;
}




void cWindowObject::onClick()
{

}


int cWindowObject::realX()
{
	if((alignment & ALIGN_HCENTER) != 0)
		return (parent->getWidth()/2)-(w/2)+x;
	else if ((alignment & ALIGN_HLEFT) != 0)
		return parent->skinOffLeft+x;
	else if ((alignment & ALIGN_HRIGHT) != 0)
		return parent->getWidth()-x-w-parent->skinOffRight;
	return 0;
}
int cWindowObject::realY()
{
	if((alignment & ALIGN_VCENTER) != 0)
		return (parent->getHeight()/2)-(h/2)+y;
	else if ((alignment & ALIGN_VBOTTOM) != 0)
		return y+parent->skinOffBottom;
	else if ((alignment & ALIGN_VTOP) != 0)
		return parent->getHeight()-y-h-parent->skinOffTop;
	return 0;
}
int cWindowObject::realy2()
{
	if((alignment & ALIGN_VCENTER) != 0)
		return (parent->ph2()/2)-(h/2)+y;
	else if ((alignment & ALIGN_VBOTTOM) != 0)
		return y+parent->skinOffBottom;
	else if ((alignment & ALIGN_VTOP) != 0)
		return parent->ph2()-y-h-parent->skinOffTop;
	return 0;
}

cWindowObject::cWindowObject()
{
	parent = NULL;
	x = 10;
	y = 10;
	w = 100;
	h = 100;
	cursorType = 0;
	alignment = ALIGN_CENTER;
	selectable = true;
	type = OBJECT_UNDEFINED;
	enabled = true;
}

cWindowObject::cWindowObject( cWindow* p, TiXmlElement* skin)
{
	parent = p;
	x = 10;
	y = 10;
	w = 100;
	h = 100;
	cursorType = 0;
	alignment = ALIGN_CENTER;
	selectable = true;
	type = OBJECT_UNDEFINED;
	enabled = true;
	
	if(skin != NULL)
	{
		std::string color = "FFFFFF";
		if(skin->FirstChildElement("fontcolor"))
			color = skin->FirstChildElement("fontcolor")->FirstChild()->Value();
		fontcolor[0] = hex2dec(color.substr(0,2)) / 256.0f;
		fontcolor[1] = hex2dec(color.substr(2,2)) / 256.0f;
		fontcolor[2] = hex2dec(color.substr(4,2)) / 256.0f;
		
		skinTopHeight = atoi(skin->FirstChildElement("top")->Attribute("height"));
		skinTop =		512 - atoi(skin->FirstChildElement("top")->FirstChild()->Value());
		skinBottomHeight = atoi(skin->FirstChildElement("bottom")->Attribute("height"));
		skinBottom =		512 - atoi(skin->FirstChildElement("bottom")->FirstChild()->Value());
		
		skinLeftWidth = atoi(skin->FirstChildElement("left")->Attribute("width"));
		skinLeft =		atoi(skin->FirstChildElement("left")->FirstChild()->Value());
		skinRightWidth = atoi(skin->FirstChildElement("right")->Attribute("width"));
		skinRight =		atoi(skin->FirstChildElement("right")->FirstChild()->Value());
	}
}