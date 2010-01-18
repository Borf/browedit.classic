#include <common.h>
#include "windowcheckbox.h"
#include "window.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <wm/wm.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <bengine/texture.h>

void cWindowCheckBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realX();
	yy = realY();

	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	glBegin(GL_QUADS);



		if (value)
		{
			glTexCoord2f((skinCheckedLeft)/512.0f,		(skinCheckedTop)/512.0f);				glVertex2d(xx, yy+h);
			glTexCoord2f((skinCheckedLeft+w)/512.0f,	(skinCheckedTop)/512.0f);				glVertex2d(xx+w, yy+h);
			glTexCoord2f((skinCheckedLeft+w)/512.0f,	(skinCheckedTop-h)/512.0f);				glVertex2d(xx+w, yy);
			glTexCoord2f((skinCheckedLeft)/512.0f,		(skinCheckedTop-h)/512.0f);				glVertex2d(xx, yy);
		}
		else
		{
			glTexCoord2f((skinUncheckedLeft)/512.0f,	(skinUncheckedTop)/512.0f);				glVertex2d(xx, yy+h);
			glTexCoord2f((skinUncheckedLeft+w)/512.0f,	(skinUncheckedTop)/512.0f);				glVertex2d(xx+w, yy+h);
			glTexCoord2f((skinUncheckedLeft+w)/512.0f,	(skinUncheckedTop-h)/512.0f);			glVertex2d(xx+w, yy);
			glTexCoord2f((skinUncheckedLeft)/512.0f,	(skinUncheckedTop-h)/512.0f);			glVertex2d(xx, yy);
		}

	glEnd();
}


void cWindowCheckBox::onClick()
{
	value = !value;
}


bool cWindowCheckBox::onChar(char keyid, bool shift)
{
	if (keyid == ' ')
	{
		value = !value;
		return true;
	}
	return false;
}

void cWindowCheckBox::setInt(int id, int val)
{
	value = val != 0;
}

int cWindowCheckBox::getInt(int id)
{
	return value ? 1 : 0;
}


std::string cWindowCheckBox::getText(int id)
{
	return value ? "1" : "0";
}

cWindowCheckBox::cWindowCheckBox( cWindow* parent, Json::Value &skin ) : cWindowObject(parent)
{
	x = 40;
	y = 40;
	alignment = ALIGN_CENTER;
	value = true;
	type = OBJECT_CHECKBOX;
	
	if(!skin)
		skin = cWM::skin;
	Json::Value bSkin = skin["checkbox"];
	
	w = bSkin["width"].asInt();
	h = bSkin["height"].asInt();
	
	skinCheckedLeft = bSkin["checked"]["left"].asInt();
	skinCheckedTop = 512-bSkin["checked"]["top"].asInt();
	skinUncheckedLeft = bSkin["unchecked"]["left"].asInt();
	skinUncheckedTop = 512-bSkin["unchecked"]["top"].asInt();
}

cWindowBoolCheckBox::cWindowBoolCheckBox( cWindow* parent, Json::Value &skin ) : cWindowCheckBox(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	boolvalue = NULL;
	firstTime = true;
}

void cWindowBoolCheckBox::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	if(boolvalue != NULL && firstTime)
	{
		value = *boolvalue;
		firstTime = false;
	}
	*boolvalue = value;
	cWindowCheckBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}

void cWindowBoolCheckBox::setInt( int id, int val )
{
	cWindowCheckBox::setInt(id,val);
	if (id == 3)
	{
		boolvalue = (bool*)val;
		value  = *boolvalue;
	}
}
