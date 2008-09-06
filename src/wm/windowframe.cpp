#include "windowframe.h"
#include "window.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <texture.h>

void cWindowFrame::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realX();
	yy = realY();

	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	if(drawinner)
	{
		glBegin(GL_QUADS);
			glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+3);
			glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+3);
			glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+h-3);
			glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+h-3);
		glEnd();
	}

	cWindowObject::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);

}


void cWindowFrame::setInt(int id, int val)
{
	drawinner = val != 0;
}

cWindowFrame::cWindowFrame( cWindow* parent, TiXmlDocument &skin ) : cWindowObject(parent, skin.FirstChildElement("skin")->FirstChildElement("frame"))
{
	w = 100;
	h = 25;
	x = 10;
	y = 10;
	alignment = ALIGN_CENTER;
	type = OBJECT_FRAME;
	drawinner = false;
}