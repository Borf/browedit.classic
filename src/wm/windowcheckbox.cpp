#include "windowcheckbox.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowCheckBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realx();
	yy = realy();

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
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


void cWindowCheckBox::click()
{
	value = !value;
}


bool cWindowCheckBox::onchar(char keyid, bool shift)
{
	if (keyid == ' ')
	{
		value = !value;
		return true;
	}
	return false;
}

void cWindowCheckBox::SetInt(int id, int val)
{
	value = val != 0;
}

int cWindowCheckBox::GetInt(int id)
{
	return value ? 1 : 0;
}


string cWindowCheckBox::GetText(int id)
{
	return value ? "1" : "0";
}
