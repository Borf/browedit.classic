#include "windowlistbox.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

extern float mousex, mousey;


void cWindowListBox::draw()
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	int xx, yy;
	xx = realx();
	yy = realy();

	int ww = w - 14;


	int i = 0;
	yy = realy()+h-5-12;
	while(yy+10 > realy() && i < (int)values.size())
	{
		i++;
		yy-=12;
	}
	int barheight = max((int)(((float)(h - 16) * (float)((float)i / (float)values.size()))+0.5f), 20);

	yy = realy();


	int barpos = (values.size() - i);
	if (barpos != 0)
		barpos = (int)((float)(h-16.0f) * ((float)liststart / (float)values.size()));

	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);
// box
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+ww-2, yy+2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+ww-2, yy+h-2);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+2, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww, yy+h-2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h-2);

		glTexCoord2f((387.0f-2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy+2);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx, yy+h-2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy+h);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+ww-2, yy+h-2);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h);
		
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww, yy);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww, yy+2);

		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f+2.0f)/512.0f);		glVertex2d(xx, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+2.0f)/512.0f);		glVertex2d(xx+2, yy);
		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+2, yy+2);
		glTexCoord2f((258.0f+2.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx, yy+2);

		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+ww-2, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww, yy+h-2);
		glTexCoord2f((387.0f-2.0f)/512.0f,	(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww, yy+h);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-2.0f)/512.0f);		glVertex2d(xx+ww-2, yy+h);

/// scrollthingy-background
		glTexCoord2f((408.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+5, yy+8);
		glTexCoord2f((413.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+10, yy+8);
		glTexCoord2f((413.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+10, yy+h-8);
		glTexCoord2f((408.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+5, yy+h-8);
/// arrow up
		glTexCoord2f((404.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+1, yy+h-8);
		glTexCoord2f((417.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+14,yy+h-8);
		glTexCoord2f((417.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+14,yy+h);
		glTexCoord2f((404.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+1, yy+h);
/// arrow down
		glTexCoord2f((404.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+1, yy);
		glTexCoord2f((417.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+14,yy);
		glTexCoord2f((417.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+14,yy+8);
		glTexCoord2f((404.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+1, yy+8);
/// block
//top
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-barpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-barpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+13,yy+h-8-barpos);
		glTexCoord2f((420.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+2, yy+h-8-barpos);
//bottom
		glTexCoord2f((420.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+2, yy+h-8-barpos-barheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+13,yy+h-8-barpos-barheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-barpos-barheight+9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-barpos-barheight+9);
		
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-barpos-barheight+9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-barpos-9);
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-barpos-9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-barpos-barheight+9);

///selection
		if (selected >= liststart && selected < liststart+i)
		{
			glTexCoord2f((421.0f)/512.0f,		(424.0f)/512.0f);			glVertex2d(xx+2, yy+h-12*(selected-liststart)-12-2);
			glTexCoord2f((430.0f)/512.0f,		(424.0f)/512.0f);			glVertex2d(xx+ww,yy+h-12*(selected-liststart)-12-2);
			glTexCoord2f((430.0f)/512.0f,		(415.0f)/512.0f);			glVertex2d(xx+ww,yy+h-12*(selected-liststart)-2);
			glTexCoord2f((421.0f)/512.0f,		(415.0f)/512.0f);			glVertex2d(xx+2, yy+h-12*(selected-liststart)-2);
		}		
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	i = liststart;
	yy = realy()+h-5-12;
	while(yy+10 > realy() && i < (int)values.size())
	{
		parent->font->print(0,0,0,parent->px()+xx+5,parent->py()+yy,"%s", values[i].c_str());
		i++;
		yy-=12;
	}
	glEnable(GL_TEXTURE_2D);
	glColor4fv(colors);

}




bool cWindowListBox::onkeydown(int key)
{
	if (key == SDLK_DOWN && selected < (int)values.size()-1)
	{
		int i = 0;
		int yy = realy()+h-5-12;
		while(yy+10 > realy() && i < (int)values.size())
		{
			i++;
			yy-=12;
		}
	
		selected++;
		if (selected - liststart >= i)
			liststart++;
		return true;
	}
	else if (key == SDLK_UP)
	{
		if (selected != 0)
		{
			selected--;
			if (selected < liststart)
				liststart--;
		}
		return true;
	}
	return false;
}


void cWindowListBox::SetText(int index, string text)
{
	if (index == -1)
		values.push_back(text);
	else
	{
		if (index >= (int)values.size())
			values.resize(index+1);
		values[index] = text;
	}
}
void cWindowListBox::SetInt(int index, int value)
{
	if (index == -1)
		properties.push_back(value);
	else if (index == -2)
	{
		selected = (value == -1 ? values.size() - 1 : value);
		if(selected < liststart)
			liststart = selected;
		if( selected > (liststart + ((h - 5 - 12 - 10)/12)))
			liststart = max(0, selected - ((h - 5 - 12 - 10)/12)-1);
	}
	else
	{
		if (index >= (int)properties.size())
			properties.resize(index+1);
		properties[index] = value;
	}
}
int cWindowListBox::GetInt(int index)
{
	if (index == -1)
		return selected;
	if (index == -2)
		return properties.size();
	if (index < 0 || index > (int)properties.size())
		return 0;
	return properties[index];
}

void cWindowListBox::click()
{
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = mousey;
	yy -= realy();
	yy -= parent->py();

	if (xx < w - 14)
	{ // in the box
		int s = selected;
		selected = liststart + ((h-yy-3) / 12);
		if (selected > (int)values.size() || selected < 0)
			selected = s;
	}
	else
	{
		if (yy < 8)
		{ // arrow down
			onkeydown(SDLK_DOWN);
		}
		else if (yy+8 > h)
		{
			onkeydown(SDLK_UP);
		}
		else
		{
			int i = 0;
			int yyy = realy()+h-5-12;
			while(yyy+10 > realy() && i < (int)values.size())
			{
				i++;
				yyy-=12;
			}

			int barheight = max((int)((float)(h - 16) * (float)((float)i / (float)values.size())), 20);
			yyy = realy();
			int barpos = (values.size() - i);
			if (barpos != 0)
				barpos = (int)((float)(h-16.0f) * ((float)liststart / (float)values.size()));

			if (h-yy-8 < barpos)
			{ // pgup
				liststart = max(0, liststart-i);
			}
			else if (h-yy-8 > barpos+barheight)
			{//pgdown
				liststart = min((int)values.size()-i , liststart+i);
			}
		}	
	}

}

void cWindowListBox::drag()
{
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = mousey;
	yy -= realy();
	yy -= parent->py();

	if (startmousex - realx() - parent->px() > w-14 && startmousex - realx() - parent->px() < w)
	{
		int i = 0;
		int yyy = realy()+h-5-12;
		while(yyy+10 > realy() && i < (int)values.size())
		{
			i++;
			yyy-=12;
		}

		yyy = realy();
		int barpos = (values.size() - i);
		if (barpos != 0)
			barpos = (int)((float)(h-16.0f) * ((float)liststart / (float)values.size()));

		int offset = yy-dragoffsety;

		barpos-=offset;
		if (barpos < 0)
			barpos = 0;
		//barpos--;
		char buf[10];
		sprintf(buf, "%i - %i", barpos, (int)(((float)barpos / (float)(h-16.0f)) * (float)values.size()));

		int oldliststart = liststart;
		liststart = max(min((int)((((float)barpos / (float)(h-16.0f)) * (float)values.size())+0.5f), (int)values.size()-i), 0);
		if (oldliststart != liststart)
			dragoffsety = yy;
	}
}

void cWindowListBox::doubleclick()
{
	Log(1,0,"You doubleclicked me!");
}

