#include "windowobject.h"
#include "window.h"
#include <common.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <graphics.h>
#include <bengine/texture.h>
#include <bengine/util.h>

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
	int xx=(int)cGraphics::cMouse::x-parent->getX();
	int yy=(cGraphics::h()-(int)cGraphics::cMouse::y)-parent->getY();
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

cWindowObject::cWindowObject( cWindow* p, Json::Value &skin)
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

	setSkin(skin);
}


void cWindowObject::setSkin(Json::Value &skin)
{
	if(skin != NULL)
	{
		std::string color = "FFFFFF";
		if(skin.isMember("fontcolor"))
			color = skin["fontcolor"].asString();
		fontcolor[0] = bEngine::util::hex2dec(color.substr(0,2)) / 256.0f;
		fontcolor[1] = bEngine::util::hex2dec(color.substr(2,2)) / 256.0f;
		fontcolor[2] = bEngine::util::hex2dec(color.substr(4,2)) / 256.0f;
		
		skinTopHeight = skin["top"]["height"].asInt();
		skinTop =		512 - skin["top"]["pos"].asInt();
		skinBottomHeight = skin["bottom"]["height"].asInt();
		skinBottom =		512 - skin["bottom"]["pos"].asInt();
		
		skinLeftWidth = skin["left"]["width"].asInt();
		skinLeft =		skin["left"]["pos"].asInt();
		skinRightWidth = skin["right"]["width"].asInt();
		skinRight =		skin["right"]["pos"].asInt();
	}
}
