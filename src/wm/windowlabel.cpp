#include <bengine/forwards.h>
#include "windowlabel.h"
#include "window.h"

#include <common.h>

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

#include <font.h>

void cWindowLabel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	if (lasttext != text)
		lines.clear();

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realX();
	yy = realY();

	int iii = -17;
	glColor4f(0,0,0,colors[3]);

	if (lines.size() == 0)
	{
			std::string textleft = text;
			std::string curline = "";
			std::string word = "";
			std::string symbol;
			while(textleft.size() > 0)
			{
				if (textleft.find(" ") != std::string::npos)
				{
					if(textleft.find("\n") < textleft.find(" "))
					{
						word = textleft.substr(0, textleft.find("\n"));
						textleft = textleft.substr(textleft.find("\n")+1);
						symbol = "\n";
					}
					else
					{
						word = textleft.substr(0, textleft.find(" "));
						textleft = textleft.substr(textleft.find(" ")+1);
						symbol = " ";
					}
				}
				else if (textleft.find("\n") != std::string::npos)
				{
					word = textleft.substr(0, textleft.find("\n"));
					textleft = textleft.substr(textleft.find("\n")+1);
					symbol = "\n";
				}
				else
				{
					word = textleft;
					textleft = "";
				}

				if(parent->font->textLen(removecolors(curline + word)) > w)
				{
					if(curline == "")
					{
						lines.push_back(word);
					}
					else
					{
						lines.push_back(curline);
						curline = "";
						textleft = word + symbol + textleft;
					}
				}
				else if(symbol == "\n")
				{
					if(curline == "")
						lines.push_back(word);
					else
						lines.push_back(curline + " " + word);
					curline = "";
					textleft = textleft;
				}
				else
				{
					curline = curline + word + " ";
				}

			}
			lines.push_back(curline);
	}

	for(int i = 0; i < (int)lines.size(); i++)
	{
		parent->font->print(parent->fontColor[0], parent->fontColor[1], parent->fontColor[2],parent->getX()+xx, h+parent->getY()+yy+iii, "%s", lines[i].c_str());
		iii-=12;
	}


	//parent->font->print(0,0,0, parent->px() + xx+5, parent->getY() + yy+3, text.c_str());
	glColor4fv(colors);
	lasttext = text;
}

void cWindowLabel::setText(int i, std::string s)
{
	text = s;
}

std::string cWindowLabel::getText(int i)
{
	return text;
}


int cWindowLabel::getInt(int i)
{
	if (i == 0)
		return lines.size();
	
	return 0;
}

cWindowLabel::cWindowLabel( cWindow* parent ) : cWindowObject(parent)
{
	w = 30;
	h = 100;
	x = 10;
	y = 10;
	alignment = ALIGN_CENTER;
	text = "Default text that is very long and will wrap";
	selectable = false;
	type = OBJECT_LABEL;
}
