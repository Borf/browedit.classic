#include "windowlistbox.h"
#include "window.h"
#include <graphics.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <font.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


void cWindowListBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	int xx, yy;
	xx = realX();
	yy = realY();

	int ww = w - skinBarWidth;


	int i = 0;
	yy = realY()+h-5-12;
	while(yy+10 > realY() && i < (int)values.size())
	{
		i++;
		yy-=12;
	}
	int barheight = max((int)(((float)(h - (skinButtonDownHeight+skinButtonUpHeight)) * (float)((float)i / (float)values.size()))+0.5f), skinTopHeight+skinBottomHeight);

	yy = realY();


	int barpos = (values.size() - i);
	if (barpos != 0)
		barpos = (int)((float)(h-(skinButtonDownHeight+skinButtonUpHeight)) * ((float)liststart / (float)values.size()));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
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
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h);
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww,				yy+h);
/// arrow down
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww,				yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+skinButtonDownHeight);
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww,				yy+skinButtonDownHeight);
/// block
		if(skinBarCenterHeight != 0)
		{
			int c1 = skinBarTop-skinBarTopHeight-	skinBarCenterHeight/2;
			int c2 = (skinBarBottom+				skinBarCenterHeight/2);

			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c1)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c1)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight);
		
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(c2)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c2)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos);

			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c2)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c2)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c1)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c1)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight/2-skinBarCenterHeight/2);
		}
		else
		{
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos-barheight);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-barpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-barpos-barheight);
		}
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
	yy = realY()+h-5-12;
	while(yy+10 > realY() && i < (int)values.size())
	{
		if(i == selected && showselection)
			parent->font->print(selectFontColor[0],selectFontColor[1],selectFontColor[2],parent->getX()+xx+5,parent->getY()+yy,"%s", values[i].c_str());
		else
			parent->font->print(fontcolor[0],fontcolor[1],fontcolor[2],parent->getX()+xx+5,parent->getY()+yy,"%s", values[i].c_str());
		i++;
		yy-=12;
	}
	glEnable(GL_TEXTURE_2D);
	glColor4fv(colors);

}




bool cWindowListBox::onKeyDown(int key, bool shift)
{
	if (key == SDLK_DOWN && selected < (int)values.size()-1)
	{
		int i = 0;
		int yy = realY()+h-5-12;
		while(yy+10 > realY() && i < (int)values.size())
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


void cWindowListBox::setText(int index, std::string text)
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
void cWindowListBox::setInt(int index, int value)
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
int cWindowListBox::getInt(int index)
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

void cWindowListBox::onClick()
{
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int yy = cGraphics::h()-(int)cGraphics::cMouse::y;
	yy -= realY();
	yy -= parent->getY();

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
			onKeyDown(SDLK_DOWN, false);
		}
		else if (yy+8 > h)
		{
			onKeyDown(SDLK_UP, false);
		}
		else
		{
			int i = 0;
			int yyy = realY()+h-5-12;
			while(yyy+10 > realY() && i < (int)values.size())
			{
				i++;
				yyy-=12;
			}

			int barheight = max((int)((float)(h - (skinButtonDownHeight+skinButtonUpHeight)) * (float)((float)i / (float)values.size())), skinTopHeight+skinBottomHeight);
			yyy = realY();
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
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int yy = cGraphics::h()-(int)cGraphics::cMouse::y;
	yy -= realY();
	yy -= parent->getY();

	if (cGraphics::cMouse::xStart - realX() - parent->getX() > w-14 && cGraphics::cMouse::xStart - realX() - parent->getX() < w)
	{
		int i = 0;
		int yyy = realY()+h-5-12;
		while(yyy+10 > realY() && i < (int)values.size())
		{
			i++;
			yyy-=12;
		}

		yyy = realY();
		int barpos = (values.size() - i);
		if (barpos != 0)
			barpos = (int)((float)(h-(skinButtonDownHeight+skinButtonUpHeight)) * ((float)liststart / (float)values.size()));

		int offset = (int)(yy-cGraphics::dragoffsety);

		barpos-=offset;
		if (barpos < 0)
			barpos = 0;
		//barpos--;
		char buf[10];
		sprintf(buf, "%i - %i", barpos, (int)(((float)barpos / (float)(h-(skinButtonDownHeight+skinButtonUpHeight))) * (float)values.size()));

		int oldliststart = liststart;
		liststart = max(min((int)((((float)barpos / (float)(h-(skinButtonDownHeight+skinButtonUpHeight))) * (float)values.size())+0.5f), (int)values.size()-i), 0);
		if (oldliststart != liststart)
			cGraphics::dragoffsety = yy;
	}
}

void cWindowListBox::onDoubleClick()
{
//	Log(1,0,"You doubleclicked me!");
}

std::string cWindowListBox::getText(int id)
{
	if(id > -1)
		return values[id];
	if(id == -1)
		return values[selected];
	return "NULL";
}



void cWindowListBox::onScrollUp()
{
	liststart-=5;
	if (liststart <= 0)
		liststart = 0;
}
void cWindowListBox::onScrollDown()
{
	unsigned int i = 0;

	int yy = realY()+h-5-12;
	while(yy+10 > realY() && i < values.size())
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

cWindowListBox::cWindowListBox( cWindow* parent, TiXmlDocument *skin ) : cWindowObject(parent,skin->FirstChildElement("skin")->FirstChildElement("list"))
{
	if(!skin)
		skin = &cWM::skin;
	w = 280;
	h = 100;
	x = 5;
	y = 20;
	alignment = ALIGN_TOPLEFT;
	liststart = 0;
	selected = 0;
	showselection = true;
	showscroll = true;
	type = OBJECT_LISTBOX;
	TiXmlElement* bSkin = skin->FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("scroll");
	
	std::string scolor = skin->FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("selectcolor")->FirstChild()->Value();
	selectColor[0] = hex2dec(scolor.substr(0,2)) / 256.0f;
	selectColor[1] = hex2dec(scolor.substr(2,2)) / 256.0f;
	selectColor[2] = hex2dec(scolor.substr(4,2)) / 256.0f;
	
	scolor = skin->FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("selectfontcolor")->FirstChild()->Value();
	selectFontColor[0] = hex2dec(scolor.substr(0,2)) / 256.0f;
	selectFontColor[1] = hex2dec(scolor.substr(2,2)) / 256.0f;
	selectFontColor[2] = hex2dec(scolor.substr(4,2)) / 256.0f;
	
	
	skinBarWidth =			atoi(bSkin->FirstChildElement("width")->FirstChild()->Value());
	skinBarLeft =			atoi(bSkin->FirstChildElement("left")->FirstChild()->Value());
	skinButtonUpLeft =		atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("left")->FirstChild()->Value());
	skinButtonUpTop =		512-atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("top")->FirstChild()->Value());
	skinButtonUpHeight =	atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("height")->FirstChild()->Value());
	skinButtonDownLeft =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("left")->FirstChild()->Value());
	skinButtonDownTop =		512-atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("top")->FirstChild()->Value());
	skinButtonDownHeight =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("height")->FirstChild()->Value());
	
	skinBarTopHeight = atoi(bSkin->FirstChildElement("top")->Attribute("height"));
	skinBarTop =		512 - atoi(bSkin->FirstChildElement("top")->FirstChild()->Value());
	skinBarBottomHeight = atoi(bSkin->FirstChildElement("bottom")->Attribute("height"));
	skinBarBottom =		512 - atoi(bSkin->FirstChildElement("bottom")->FirstChild()->Value());
	
	skinBarBackTop=		512-atoi(bSkin->FirstChildElement("background")->FirstChildElement("top")->FirstChild()->Value());
	skinBarBackHeight =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("height")->FirstChild()->Value());
	skinBarBackLeft =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("left")->FirstChild()->Value());
	skinBarCenterHeight = atoi(bSkin->FirstChildElement("centerheight")->FirstChild()->Value());
}
