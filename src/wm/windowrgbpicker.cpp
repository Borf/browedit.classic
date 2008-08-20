#include "windowrgbpicker.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowRGBPicker::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realX();
	yy = realY();
	glEnable(GL_TEXTURE_2D);

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	
	glColor4f(1,1,1, colors[3]);
	if (this->inObject())
		glColor4f(r, g, b, colors[3]);

	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	glBegin(GL_QUADS);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+w-3, yy+h-3);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+3, yy+h-3);

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
	glDisable(GL_TEXTURE_2D);


	int color;
	glBegin(GL_LINES);
	for(int i = 0; i < w-8; i++)
	{
		color = (int)((i/(float)(w-8)) * 16777216);
//		color = color & 16776960;
		glColor3f((color&255)/256.0f,((color>>8)&255)/256.0f,((color>>16)&255)/256.0f);
		glVertex2f(xx+4 + i, yy+4);
		glVertex2f(xx+4 + i, yy+h-4);
	}
	glEnd();




 	glColor4fv(colors);
}


void cWindowRGBPicker::click()
{
	float clickx = mouseX - (x + 4);
	int color = (int)((clickx/(float)(w-8)) * 16777216);

	r = (color&255)/256.0f;
	g = ((color>>8)&255)/256.0f;
	b = ((color>>16)&255)/256.0f;


}
void cWindowRGBPicker::drag()
{
	click();
}

cWindowRGBPicker::cWindowRGBPicker( cWindow* parent ) : cWindowObject(parent)
{
	w = 100;
	h = 25;
	x = 10;
	y = 10;
	alignment = ALIGN_CENTER;
	type = OBJECT_RGBPICKER;
	r = 0.5f;
	g = 0.5f;
	b = 1;
}

