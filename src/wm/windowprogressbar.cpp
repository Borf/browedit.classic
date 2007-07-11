#include "windowprogressbar.h"
#include "window.h"

void cWindowProgressBar::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	int xx, yy;
	xx = realx();
	yy = realy();

	float ratio = ((float)value / (float)max-min);
	float ww = (float)w*ratio;

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+w-2, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+w, yy+2);
		
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h);

		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+w-2, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+w, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w, yy+h);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-2, yy+h);

		if (value > min)
		{
			if (gradcolors)
				glColor4f(1.0f-ratio,ratio,0, colors[3]);
			else
				glColor4f(0.5f,0.5f,1.0f,colors[3]);


			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+2);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h-2);
			glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);

			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
			glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
			glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
			glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

			glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+2);
			glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww, yy+2);
			glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww, yy+h-2);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h-2);

			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
			glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+ww, yy);
			glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww, yy+2);
			
			glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);
			glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h-2);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww-2, yy+h);

			glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+2, yy);
			glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
			glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);

			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h-2);
			glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww, yy+h-2);
			glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+ww, yy+h);
			glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww-2, yy+h);
		}

	glEnd();

	glColor4fv(colors);
	
}


void cWindowProgressBar::click()
{
	Log(3,0,"Clicked the progress bar");
}


int cWindowProgressBar::GetInt(int id)
{
	return 0;
}

void cWindowProgressBar::SetInt(int id, int val)
{
	if (id == 0)
		value = val;
	else if (id == 1)
		min = val;
	else if (id == 2)
		max = val;
	else if (id == 3)
		gradcolors = (val != 0);
}

string cWindowProgressBar::ppopup()
{
	char buf[100];
	sprintf(buf ,"%i/%i (%.02f%%)", value, max-min, ((float)value / (float)(max-min))*100.0f);
	return buf;
}