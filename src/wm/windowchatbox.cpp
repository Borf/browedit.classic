#include "windowchatbox.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

extern int mousex, mousey;
extern int mousestartx;

void cWindowChatBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realx();
	yy = realy();

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int ww = w - 14;


	int i = liststart;
	yy = realy()+(FONTHEIGHT*h)-5-14;
	while(i - liststart < h && i < (int)values.size())
	{
		i++;
		yy-=FONTHEIGHT;
	}
	int barheight = max((int)(((float)((FONTHEIGHT*h) - 16) * (float)((float)(i-liststart) / (float)values.size()))+0.5f), 5);

	yy = realy();


	int barpos = (values.size() - (i-liststart));
	if (barpos != 0)
		barpos = (int)(((float)(FONTHEIGHT*h)-16.0f) * ((float)liststart / (float)values.size()));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glColor4f(1.0f, 1.0f, 1.0f,1.0f);
	glBegin(GL_QUADS);
/// scrollthingy-background
		glTexCoord2f((408.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+5, yy+8);
		glTexCoord2f((413.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+10, yy+8);
		glTexCoord2f((413.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+10, yy+(FONTHEIGHT*h)-8);
		glTexCoord2f((408.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+5, yy+(FONTHEIGHT*h)-8);
/// arrow up
		glTexCoord2f((404.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+1, yy+(FONTHEIGHT*h)-8);
		glTexCoord2f((417.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+14,yy+(FONTHEIGHT*h)-8);
		glTexCoord2f((417.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+14,yy+(FONTHEIGHT*h));
		glTexCoord2f((404.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+1, yy+(FONTHEIGHT*h));
/// arrow down
		glTexCoord2f((404.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+1, yy);
		glTexCoord2f((417.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+14,yy);
		glTexCoord2f((417.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+14,yy+8);
		glTexCoord2f((404.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+1, yy+8);
/// block
//top
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos);
		glTexCoord2f((420.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos);
//bottom
		glTexCoord2f((420.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos-barheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos-barheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos-barheight+9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos-barheight+9);
		
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos-barheight+9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+(FONTHEIGHT*h)-8-barpos-9);
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos-9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+(FONTHEIGHT*h)-8-barpos-barheight+9);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	i = liststart;
	yy = realy()+(FONTHEIGHT*h)-5-10;
	while(i - liststart < h && i < (int)values.size())
	{
		parent->font->print(1,1,1,parent->px()+xx+5,parent->py()+yy,"%s", values[i].c_str());
		i++;
		yy-=FONTHEIGHT;
	}
	glColor4fv(colors);


}




bool cWindowChatBox::onkeydown(int key)
{
	if (key == SDLK_DOWN && liststart + h < (int)values.size())
	{
		liststart++;
		return true;
	}
	else if (key == SDLK_UP && liststart > 0)
	{
		liststart--;
		return true;
	}
	if (key == SDLK_PAGEDOWN)
	{
		liststart+=h;
		if (liststart + h >= (int)values.size())
			liststart = (int)values.size() - h;
		return true;
	}

	else if (key == SDLK_PAGEUP)
	{
		liststart-=h;
		if (liststart < 0)
			liststart = 0;
		return true;
	}
	return false;

}


void cWindowChatBox::SetText(int index, string text)
{
	if (index == 0)
	{
		values.push_back((char)1 + string("255255255") + getLongTimeString() + " " + (char)1 + string("255255000") + text);
		if (values.size() > 50)
			values.erase(values.begin());
		if (((int)values.size() - liststart) > h)
			liststart = values.size() - h;
	}
}
void cWindowChatBox::SetInt(int index, int value)
{
	if (index == 0)
	{
		values.clear();
		liststart = 0;
	}
}


void cWindowChatBox::click()
{
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = mousey;
	yy -= realy();
	yy -= parent->py();

	if (xx < w - 14)
	{ // in the box
//		selected = liststart + ((h-yy-3) / 12);
	}
	else
	{
		if (yy < 8)
		{ // arrow down
			onkeydown(SDLK_DOWN);
		}
		else if (yy+8 > (FONTHEIGHT*h))
		{
			onkeydown(SDLK_UP);
		}
		else
		{
			int i = liststart;
			int yyy = realy()+(FONTHEIGHT*h)-5-12;
			while(i - liststart < h && i < (int)values.size())
			{
				parent->font->print(1,1,0,parent->px()+xx+5,parent->py()+yyy,"%s", values[i].c_str());
				i++;
				yyy-=FONTHEIGHT;
			}

			int barheight = max((int)((float)((FONTHEIGHT*h) - 16) * (float)((float)(i-liststart) / (float)values.size())), 5);
			yyy = realy();
			int barpos = (values.size() - (i-liststart));
			if (barpos != 0)
				barpos = (int)((float)((FONTHEIGHT*h)-16.0f) * ((float)liststart / (float)values.size()));

			if ((FONTHEIGHT*h)-yy-8 < barpos)
			{ // pgup
				liststart = max(0, liststart-(i-liststart));
			}
			else if ((FONTHEIGHT*h)-yy-8 > barpos+barheight)
			{//pgdown
				liststart = min((int)values.size()-(i-liststart) , liststart+(i-liststart));
			}
		}	
	}

}

void cWindowChatBox::drag()
{
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = mousey;
	yy -= realy();
	yy -= parent->py();

	if (mousestartx - realx() - parent->px() > w-14 && mousestartx - realx() - parent->px() < w)
	{
		int i = liststart;
		int yyy = realy()+(FONTHEIGHT*h)-5-12;
		while(i - liststart < h && i < (int)values.size())
		{
			parent->font->print(1,1,0,parent->px()+xx+5,parent->py()+yyy,"%s", values[i].c_str());
			i++;
			yyy-=FONTHEIGHT;
		}

		yyy = realy();
		int barpos = (values.size() - (i-liststart));
		if (barpos != 0)
			barpos = (int)((float)((FONTHEIGHT*h)-16.0f) * ((float)liststart / (float)values.size()));

		int offset = yy-dragoffsety;

		barpos-=offset;
		if (barpos < 0)
			barpos = 0;
		//barpos--;
		char buf[10];
		sprintf(buf, "%i - %i", barpos, (int)(((float)barpos / (float)((FONTHEIGHT*h)-16.0f)) * (float)values.size()));

		int oldliststart = liststart;
		liststart = max(min((int)((((float)barpos / (float)((FONTHEIGHT*h)-16.0f)) * (float)values.size())+0.5f), (int)values.size()-(i-liststart)), 0);
		if (oldliststart != liststart)
			dragoffsety = yy;
	}
}

void cWindowChatBox::doubleclick()
{
//	MessageBox(NULL, "You doubleclicked me!", "bla", MB_OK);
}


cWindowObject* cWindowChatBox::inobject()
{
	int xx=mousex-parent->px();
	int yy=mousey-parent->py();
	if (xx > realx() && xx < realx()+w &&
		yy > realy() && yy < realy()+(FONTHEIGHT*h))
		return this;
	return NULL;
}

