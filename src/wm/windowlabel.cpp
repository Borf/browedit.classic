#include "windowlabel.h"
#include "window.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library

void cWindowLabel::draw()
{
	if (lasttext != text)
		lines.clear();

	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realx();
	yy = realy();

	int iii = -17;
	glColor3f(0,0,0);

	if (lines.size() == 0)
	{
/*		string t = text;
		string t1;
		string next;
		string space = "";
		while(t.length() > 0)
		{
			next = t;
			if (t.find(" ") != string::npos)
			{
				if (t.find("\n") != string::npos)
				{
					if (t.find(" ") > t.find((char)10))
					{
						next = t.substr(0, t.find((char)10));
						t = t.substr(t.find((char)10)+1);
						space = "\n";
					}
					else
					{
						next = t.substr(0, t.find(" "));
						t = t.substr(t.find(" ")+1);
					}
				}
				else
				{
					next = t.substr(0, t.find(" "));
					t = t.substr(t.find(" ")+1);
				}
			}
			else
			{	// geen spatie
				if (t.find("\n") != string::npos)
				{
					next = t.substr(0, t.find((char)10));
					t = t.substr(t.find((char)10)+1);
					space = "\n";
				}
				else
				{
					next = t;
					t = "";
				}
			}

			if (parent->font->textlen(removecolors(t1 + " " + next)) > w || space == "\n")
			{
				lines.push_back(t1);
				t1 = next;
			}
			else
				t1 = t1 + space + next;
			if (space != "\n")
				space = " ";
			else
				space = "";
			}

		}
		lines.push_back(t1);
*/

			string textleft = text;
			string curline = "";
			string word = "";
			string symbol;
			while(textleft.size() > 0)
			{
				if (textleft.find(" ") != string::npos)
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
				else if (textleft.find("\n") != string::npos)
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

				if(parent->font->textlen(removecolors(curline + word)) > w)
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
		parent->font->print(0,0,0,parent->px()+xx, h+parent->py()+yy+iii, "%s", lines[i].c_str());
		iii-=12;
	}


	//parent->font->print(0,0,0, parent->px() + xx+5, parent->py() + yy+3, text.c_str());
	glColor4fv(colors);
	lasttext = text;
}

void cWindowLabel::SetText(int i, string s)
{
	text = s;
}

string cWindowLabel::GetText(int i)
{
	return text;
}


int cWindowLabel::GetInt(int i)
{
	if (i == 0)
		return lines.size();
	
	return 0;
}
