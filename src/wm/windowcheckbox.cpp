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


		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+h);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h);
		
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w, yy+2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+2);

		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h);

		if (value)
		{
			glTexCoord2f((264.0f)/512.0f,	(218.0f)/512.0f);				glVertex2d(xx+3, yy+3);
			glTexCoord2f((270.0f)/512.0f,	(218.0f)/512.0f);				glVertex2d(xx+w-3, yy+3);
			glTexCoord2f((270.0f)/512.0f,	(225.0f)/512.0f);				glVertex2d(xx+w-3, yy+h-3);
			glTexCoord2f((264.0f)/512.0f,	(225.0f)/512.0f);				glVertex2d(xx+3, yy+h-3);
		}

	glEnd();
}


void cWindowCheckBox::click()
{
	value = !value;
}


bool cWindowCheckBox::onchar(char keyid)
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

