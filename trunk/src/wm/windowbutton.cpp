#include "windowbutton.h"
#include "window.h"

#include <font_.h>
#include <common.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <wm/wm.h>
#include <wm/windowpicturebox.h>

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

cWindowButton::cWindowButton( cWindow* parent, Json::Value &skin) : cWindowObject(parent, skin["button"])
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
	icon = NULL;
}

void cWindowButton::setText( int id, std::string txt )
{
	text = txt;
}

cWindowCloseButton::cWindowCloseButton( cWindow* parent, Json::Value &skin ) : cWindowObject(parent, skin["closebutton"])
{
	if(!skin)
		skin = cWM::skin;
	resizeTo(skin["closebutton"]["width"].asInt(), skin["closebutton"]["height"].asInt());
	moveTo(skin["closebutton"]["offsetx"].asInt(), skin["closebutton"]["offsety"].asInt());
	alignment = ALIGN_TOPRIGHT;
}

void cWindowCloseButton::onClick()
{
	parent->close();
}

cWindowRollupButton::cWindowRollupButton( cWindow* parent, Json::Value &skin ) : cWindowObject(parent, skin["collapsebutton"])
{
	if(!skin)
		skin = cWM::skin;
	resizeTo(skin["collapsebutton"]["width"].asInt(), skin["collapsebutton"]["height"].asInt());
	moveTo(skin["collapsebutton"]["offsetx"].asInt(), skin["collapsebutton"]["offsety"].asInt());
	alignment = ALIGN_TOPRIGHT;
}

void cWindowRollupButton::onClick()
{
	parent->toggleroll();
}





cWindowToggleButton::cWindowToggleButton(cWindow* parent, Json::Value &skin) : cWindowButton(parent, skin)
{
	pushed = false;
	text = "";
	linkedValue = NULL;
	alignment = ALIGN_TOPLEFT;
}

cWindowToggleButton::cWindowToggleButton( cWindow* parent, bool* linked ) : cWindowButton(parent)
{
	linkedValue = linked;
	text = "";
	resizeTo(28,28);
	alignment = ALIGN_TOPLEFT;
}
void cWindowToggleButton::onClick()
{
	pushed = !pushed;
	if(linkedValue)
		*linkedValue = !*linkedValue;
}

void cWindowToggleButton::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	if(linkedValue)
		pushed = *linkedValue;
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	if(pushed)
		glColor4f(0.75, 0.75, 0.75, 0.75);

	cWindowButton::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	if(icon != NULL)
	{
		icon->moveTo(getX() + (getWidth() - icon->getWidth())/2, getY() + (getHeight() - icon->getHeight())/2);
		icon->draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
	}

	glColor4fv(colors);
}

void cWindowToggleButton::setIcon( std::string file )
{
	icon = new cWindowPictureBox(parent);
	icon->setText(0, file);
	icon->resizeTo(16,16);
	icon->moveTo(6,6);
	icon->alignment = ALIGN_TOPLEFT;
}