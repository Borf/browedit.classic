#include "windowtree.h"
#include "window.h"
#include "../graphics.h"
extern cGraphics Graphics;
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


void cWindowTree::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	glEnable(GL_BLEND);
	int i;

	vector<string> values;
	for(i = 0; i < nodes.size(); i++)
		nodes[i]->getdata(values);


	int xx, yy;
	xx = realx();
	yy = realy();

	int ww = w - 14;


	i = 0;
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
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+4, yy+4);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+ww-4, yy+4);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+ww-4, yy+h-4);
		glTexCoord2f((371.0f)/512.0f,	(337.0f)/512.0f);				glVertex2d(xx+4, yy+h-4);
///selection
		if (selected >= liststart && selected < liststart+i && showselection)
		{
			glTexCoord2f((421.0f)/512.0f,		(424.0f)/512.0f);			glVertex2d(xx+4, yy+h-12*(selected-liststart)-12-4);
			glTexCoord2f((430.0f)/512.0f,		(424.0f)/512.0f);			glVertex2d(xx+ww-4,yy+h-12*(selected-liststart)-12-4);
			glTexCoord2f((430.0f)/512.0f,		(415.0f)/512.0f);			glVertex2d(xx+ww-4,yy+h-12*(selected-liststart)-4);
			glTexCoord2f((421.0f)/512.0f,		(415.0f)/512.0f);			glVertex2d(xx+4, yy+h-12*(selected-liststart)-4);
		}		

// borders
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+ww, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+ww, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+h-4);

		glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);

		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy+h);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+h-4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+ww-4, yy+h);
		
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww-4, yy);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+4);
//corners
		glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+4, yy+h);
		glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);

		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+ww, yy+h-4);
		glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+ww, yy+h);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+ww-4, yy+h);

		glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy);
		glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
		glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);

		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+ww-4, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+ww, yy);
		glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+ww, yy+4);
		glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+ww-4, yy+4);

		if (showscroll)
		{
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




bool cWindowTree::onkeydown(int key)
{
	int i;
	vector<string> values;
	for(i = 0; i < nodes.size(); i++)
		nodes[i]->getdata(values);
	if (key == SDLK_DOWN && selected < (int)values.size()-1)
	{
		i = 0;
		int yy = realy()+h-5-12;
		while(yy+10 > realy() && i < (int)values.size())
		{
			i++;
			yy-=12;
		}
	
		selected++;
		if (liststart < values.size() - (h/12))
			liststart++;
		return true;
	}
	else if (key == SDLK_UP)
	{
		if (selected != 0)
		{
			selected--;
			if (liststart > 0)
				liststart--;
		}
		return true;
	}
	else if(key == SDLK_RIGHT || key == SDLK_LEFT)
	{
		doubleclick();
		return true;
	}
	return false;
}


void cWindowTree::SetText(int index, string text)
{
}
void cWindowTree::SetInt(int index, int value)
{
	if (index == -3)
	{
		showselection = value != 0;
	}
	else if (index == -4)
	{
		showscroll = value != 0;
	}
}
int cWindowTree::GetInt(int index)
{
	if (index == 1)
		return (int)&nodes;
	return selected;
}

void cWindowTree::click()
{
	int i;
	vector<string> values;
	for(i = 0; i < nodes.size(); i++)
		nodes[i]->getdata(values);

	
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

void cWindowTree::drag()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = Graphics.h()-(int)mousey;
	yy -= realy();
	yy -= parent->py();

	if (startmousex - realx() - parent->px() > w-14 && startmousex - realx() - parent->px() < w)
	{
		int i;
		vector<string> values;
		for(i = 0; i < nodes.size(); i++)
			nodes[i]->getdata(values);

		i = 0;
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

		int offset = (int)(yy-dragoffsety);

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

void cWindowTree::doubleclick()
{
	int a = selected;
	cTreeNode* node = NULL;
	for(int i = 0; i < nodes.size(); i++)
	{
		 node = nodes[i]->getnode(a);
		 if(node != NULL)
			 break;
	}
	if(node != NULL)
	{
		node->open = !node->open;
		Log(3,0,"Selected Node %s", node->text.c_str());
	}
	else
		Log(3,0,"Wrong node selected O_o;");
}




void cWindowTree::cTreeNode::getdata(vector<string> &data, int level)
{
	string d;

	int i;
	for(i = 0; i < level; i++)
	{
		d += "    ";
	}
	if (children.size() == 0)
		d += "    ";
	else if (open)
		d += "\7  \0";
	else
		d += "\32  ";

	d += text;
	data.push_back(d);
	if (open)
	{
		for(i = 0; i < children.size(); i++)
			children[i]->getdata(data, level+1);
	}
}

cWindowTree::cTreeNode* cWindowTree::cTreeNode::getnode(int &nr)
{
	if (nr == 0)
		return this;
	nr--;

	if (open)
	{
		for(int i = 0; i < children.size(); i++)
		{
			cTreeNode* n = children[i]->getnode(nr);
			if (n != NULL)
				return n;
		}
	}
	return NULL;
}

bool cWindowTree::cTreeNode::haschild(string tofind)
{
	for(int i = 0; i < children.size(); i++)
		if (children[i]->text == tofind)
			return true;

	return false;
}


void cWindowTree::cTreeNode::addchild(cWindowTree::cTreeNode* newnode)
{
	children.push_back(newnode);
	newnode->parent = this;
}
