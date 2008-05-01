#include "windowlistbox.h"
#include "window.h"
#include <graphics.h>
extern cGraphics Graphics;
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


void cWindowListBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	int xx, yy;
	xx = realx();
	yy = realy();

	int ww = w - skinBarWidth;


	int i = 0;
	yy = realy()+h-5-12;
	while(yy+10 > realy() && i < (int)values.size())
	{
		i++;
		yy-=12;
	}
	int barheight = max((int)(((float)(h - (skinButtonDownHeight+skinButtonUpHeight)) * (float)((float)i / (float)values.size()))+0.5f), skinTopHeight+skinBottomHeight);

	yy = realy();


	int barpos = (values.size() - i);
	if (barpos != 0)
		barpos = (int)((float)(h-(skinButtonDownHeight+skinButtonUpHeight)) * ((float)liststart / (float)values.size()));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glBegin(GL_QUADS);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);
		glTexCoord2f(skinLeft/512.0f,					skinTop/512.0f);						glVertex2d(xx+0,				yy+h);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+h);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);

		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+ww,				yy+h);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+h);

		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);

		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+ww,				yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
///////////////			
		glTexCoord2f(skinLeft/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+0,				yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww-skinRightWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);

		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww-skinRightWidth,	yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww,				yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+ww,				yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
	
///selection
		if (selected >= liststart && selected < liststart+i && showselection)
		{
			glColor4f(selectColor[0],selectColor[1],selectColor[2],colors[3]);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+4, yy+h-12*(selected-liststart)-12-4);
			glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-4,yy+h-12*(selected-liststart)-12-4);
			glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-4,yy+h-12*(selected-liststart)-4);
			glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+4, yy+h-12*(selected-liststart)-4);
			glColor4fv(colors);
		}		


		if (showscroll)
		{
/// scrollthingy-background
		glTexCoord2f((skinBarBackLeft)/512.0f,				(skinBarBackTop)/512.0f);						glVertex2d(xx+ww,				yy+skinButtonDownHeight);
		glTexCoord2f((skinBarBackLeft+skinBarWidth)/512.0f,	(skinBarBackTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+skinButtonDownHeight);
		glTexCoord2f((skinBarBackLeft+skinBarWidth)/512.0f,	(skinBarBackTop-skinBarBackHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight);
		glTexCoord2f((skinBarBackLeft)/512.0f,				(skinBarBackTop-skinBarBackHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinButtonUpHeight);
/// arrow up
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww,				yy+h);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight);
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight);
/// arrow down
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww,				yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+skinButtonDownHeight);
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww,				yy+skinButtonDownHeight);
/// block
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);					glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);					glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight);
//top
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-skinBarTopHeight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-skinBarTopHeight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop)/512.0f);					glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop)/512.0f);					glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos);
//bottom
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom-skinBarBottomHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom-skinBarBottomHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight+skinBarBottomHeight);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight+skinBarBottomHeight);
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	i = liststart;
	yy = realy()+h-5-12;
	while(yy+10 > realy() && i < (int)values.size())
	{
		if(i == selected && showselection)
			parent->font->print(selectFontColor[0],selectFontColor[1],selectFontColor[2],parent->px()+xx+5,parent->py()+yy,"%s", values[i].c_str());
		else
			parent->font->print(fontcolor[0],fontcolor[1],fontcolor[2],parent->px()+xx+5,parent->py()+yy,"%s", values[i].c_str());
		i++;
		yy-=12;
	}
	glEnable(GL_TEXTURE_2D);
	glColor4fv(colors);

}




bool cWindowListBox::onkeydown(int key, bool shift)
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
		if (liststart < (int)values.size() - (h/12))
			liststart++;
		onChange(selected-1);
		return true;
	}
	else if (key == SDLK_UP)
	{
		if (selected != 0)
		{
			selected--;
			if (liststart > 0)
				liststart--;
			onChange(selected+1);
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
		int oldselected = selected;
		selected = (value == -1 ? values.size() - 1 : value);
		if(selected < liststart)
			liststart = selected;
		if( selected > (liststart + ((h - 5 - 12 - 10)/12)))
			liststart = max(0, selected - ((h - 5 - 12 - 10)/12)-1);
		if(selected != oldselected)
			onChange(oldselected);
	}
	else if (index == -3)
	{
		showselection = value != 0;
	}
	else if (index == -4)
	{
		showscroll = value != 0;
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
	if (index == -3)
		return values.size();
	if (index < 0 || index > (int)properties.size())
		return 0;
	return properties[index];
}

void cWindowListBox::click()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = Graphics.h()-(int)mousey;
	yy -= realy();
	yy -= parent->py();

	if (xx < w - 14)
	{ // in the box
		int s = selected;
		selected = liststart + ((h-yy-3) / 12);
		if (selected >= (int)values.size() || selected < 0)
			selected = s;
		if(selected != s)
			onChange(s);
	}
	else
	{
		if (yy < 8)
		{ // arrow down
			onkeydown(SDLK_DOWN, false);
		}
		else if (yy+8 > h)
		{
			onkeydown(SDLK_UP, false);
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

			int barheight = max((int)((float)(h - (skinButtonDownHeight+skinButtonUpHeight)) * (float)((float)i / (float)values.size())), skinTopHeight+skinBottomHeight);
			yyy = realy();
			int barpos = (values.size() - i);
			if (barpos != 0)
				barpos = (int)((float)(h-(skinButtonDownHeight+skinButtonUpHeight)) * ((float)liststart / (float)values.size()));

			if (h-yy-skinButtonUpHeight < barpos)
			{ // pgup
				liststart = max(0, liststart-i);
			}
			else if (h-yy-skinButtonDownHeight > barpos+barheight)
			{//pgdown
				liststart = min((int)values.size()-i , liststart+i);
			}
		}	
	}

}

void cWindowListBox::drag()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = Graphics.h()-(int)mousey;
	yy -= realy();
	yy -= parent->py();

	if (mousestartx - realx() - parent->px() > w-14 && mousestartx - realx() - parent->px() < w)
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
			barpos = (int)((float)(h-(skinButtonDownHeight+skinButtonUpHeight)) * ((float)liststart / (float)values.size()));

		int offset = (int)(yy-dragoffsety);

		barpos-=offset;
		if (barpos < 0)
			barpos = 0;
		//barpos--;
		char buf[10];
		sprintf(buf, "%i - %i", barpos, (int)(((float)barpos / (float)(h-(skinButtonDownHeight+skinButtonUpHeight))) * (float)values.size()));

		int oldliststart = liststart;
		liststart = max(min((int)((((float)barpos / (float)(h-(skinButtonDownHeight+skinButtonUpHeight))) * (float)values.size())+0.5f), (int)values.size()-i), 0);
		if (oldliststart != liststart)
			dragoffsety = yy;
	}
}

void cWindowListBox::doubleclick()
{
//	Log(1,0,"You doubleclicked me!");
}

string cWindowListBox::GetText(int id)
{
	if(id > -1)
		return values[id];
	if(id == -1)
		return values[selected];
	return "NULL";
}



void cWindowListBox::scrollup()
{
	liststart-=5;
	if (liststart <= 0)
		liststart = 0;
}
void cWindowListBox::scrolldown()
{
	unsigned int i = 0;

	int yy = realy()+h-5-12;
	while(yy+10 > realy() && i < values.size())
	{
		i++;
		yy-=12;
	}

	liststart+=5;
	if(liststart >= (int)values.size() - (h/12))
		liststart = values.size() - (h/12);
	if(h/12 > (int)values.size())
		liststart = 0;

}
