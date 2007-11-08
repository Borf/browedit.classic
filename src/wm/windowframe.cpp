#include "windowframe.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowFrame::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realx();
	yy = realy();

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);
	if(drawinner)
	{
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+h-3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+h-3);
	}
// borders
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);

		glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);

		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy+h);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);
		
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
//corners
		glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy+h);
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);

		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w, yy+h);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);

		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);

		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+w-4, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);	
	glEnd();

}


void cWindowFrame::SetInt(int id, int val)
{
	drawinner = val != 0;
}
