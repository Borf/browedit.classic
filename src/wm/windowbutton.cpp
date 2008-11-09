#include "windowbutton.h"
#include "window.h"

#include <font.h>
#include <common.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <wm/wm.h>

void cWindowButton::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realX();
	yy = realY();
	glEnable(GL_TEXTURE_2D);

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	
	if (this->inObject())
		glColor4f(0.75f, 0.75f, 1.0f, colors[3]);
	if(!enabled)
		glColor4f(0.25, 0.25, 0.25, colors[3]);

	cWindowObject::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	parent->font->print(fontcolor[0], fontcolor[1], fontcolor[2], parent->getX() + xx + (w/2)- (parent->font->textLen(text)/2), parent->getY() + yy + (h/2)-6, text.c_str());
	glColor4fv(colors);
}


void cWindowButton::onClick()
{
	Log(1,0, "You clicked the button!");
}

bool cWindowButton::onKeyDown(int c, bool shift)
{
	if (c == SDLK_RETURN)
	{
		onClick();
		return true;
	}
	return false;
}

cWindowButton::cWindowButton( cWindow* parent, TiXmlDocument* skin) : cWindowObject(parent, skin ? skin->FirstChildElement("skin")->FirstChildElement("button") : cWM::skin.FirstChildElement("skin")->FirstChildElement("button"))
{
	w = 100;
	h = 25;
	x = 15;
	y = 10;
	alignment = ALIGN_CENTER;
	text = "Default";
	cursorType = 1;
	selectable = true;
	type = OBJECT_BUTTON;
	enabled = true;
}

void cWindowButton::setText( int id, std::string txt )
{
	text = txt;
}

cWindowCloseButton::cWindowCloseButton( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent, skin ? skin->FirstChildElement("skin")->FirstChildElement("closebutton") : cWM::skin.FirstChildElement("skin")->FirstChildElement("closebutton"))
{
	if(!skin)
		skin = &cWM::skin;
	resizeTo(atoi(skin->FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("width")->FirstChild()->Value()), atoi(skin->FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("height")->FirstChild()->Value()));
	moveTo(atoi(skin->FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("offsetx")->FirstChild()->Value()), atoi(skin->FirstChildElement("skin")->FirstChildElement("closebutton")->FirstChildElement("offsety")->FirstChild()->Value()));
	alignment = ALIGN_TOPRIGHT;
}

void cWindowCloseButton::onClick()
{
	parent->close();
}

cWindowRollupButton::cWindowRollupButton( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent, skin ? skin->FirstChildElement("skin")->FirstChildElement("collapsebutton") : cWM::skin.FirstChildElement("skin")->FirstChildElement("collapsebutton"))
{
	if(!skin)
		skin = &cWM::skin;
	resizeTo(atoi(skin->FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("width")->FirstChild()->Value()), atoi(skin->FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("height")->FirstChild()->Value()));
	moveTo(atoi(skin->FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("offsetx")->FirstChild()->Value()), atoi(skin->FirstChildElement("skin")->FirstChildElement("collapsebutton")->FirstChildElement("offsety")->FirstChild()->Value()));
	alignment = ALIGN_TOPRIGHT;
}

void cWindowRollupButton::onClick()
{
	parent->toggleroll();
}
