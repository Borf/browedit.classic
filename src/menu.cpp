#include "menu.h"
#include <gl/gl.h>

#include "graphics.h"
#include "font.h"

extern cGraphics Graphics;
extern cMenu*			lastmenu;
extern cMenu*			menu;


cMenu::cMenu()
{
	ticked = false;
	opened = false;
	w = 100;
	mouseover = false;
	maxlen = -1;
	mouseoutproc = NULL;
	mouseoverproc = NULL;
	parent = NULL;
	updatedChildrenPos = false;
	opacity = 0;
}

void cMenu::unmouseover()
{
	mouseover = false;
	for(int i = 0; i < (int)items.size(); i++)
		items[i]->unmouseover();
}

void cMenu::closemenu()
{
	opacity = 0;
	opened = false;
	if(mouseoutproc!= NULL)
		mouseoutproc(this);
	for(int i = 0; i < (int)items.size(); i++)
		items[i]->closemenu();
}

cMenu* cMenu::find(std::string s, bool recurse)
{
	int i;
	if (title == s)
		return this;
	else
	{
		if(recurse)
		{
			cMenu* m = NULL;
			for(i = 0; i < (int)items.size() && m == NULL; i++)
				m = items[i]->find(s);
			if (m != NULL)
				return m;
		}
		else
		{
			for(i = 0; i < (int)items.size(); i++)
				if(items[i]->title == s)
					return items[i];
		}
	}
	return NULL;
}

void cMenu::sort()
{
//	mergesort(items, cmp);
	//TODO
	for(unsigned int i = 0; i < items.size(); i++)
		items[i]->sort();
}

unsigned int cMenu::h()
{
	return (unsigned int) ((drawstyle==0) ? 20 : 20*items.size());
}
cMenuItem::cMenuItem()
{
	item = true;
}


void cMenu::draw()
{
	int i;
	if(drawstyle == 0)
	{
		bool oneopened = false;
		for(i = 0; i < (int)items.size(); i++)
			if (items[i]->opened)
				oneopened = true;
		
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			glVertex2f(0, Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h());
			glVertex2f(0, Graphics.h());
		glEnd();
		glBegin(GL_LINES);
		for(i = 0; i < 10; i++)
		{
			glColor4f(i/3.0f,i/3.0f,i/3.0f,1);
			glVertex2f(0, Graphics.h()-20+i);
			glVertex2f(Graphics.w(), Graphics.h()-20+i);
		}
		glEnd();
		glColor4f(0,0,0,1);
		glBegin(GL_QUADS);
			glVertex2f(0, Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-20);
			glVertex2f(Graphics.w(), Graphics.h()-19);
			glVertex2f(0, Graphics.h()-19);
		glEnd();
		for(i = 0; i < (int)items.size(); i++)
		{
			if (mouseX >= x + items[i]->x && mouseX < x + items[i]->x + items[i]->w && mouseY < 20)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,0.5);
				glBegin(GL_QUADS);
					glVertex2f(x+items[i]->x, 		Graphics.h()-y-15);
					glVertex2f(x+items[i]->x+items[i]->w, 	Graphics.h()-y-15);
					glVertex2f(x+items[i]->x+items[i]->w, 	Graphics.h()-y);
					glVertex2f(x+items[i]->x, 		Graphics.h()-y);
					if (oneopened && !items[i]->opened)
					{
						items[i]->opacity = 0;
						items[i]->opened = true;
					}
				glEnd();
				glColor4f(0,0,0,1);
			}
			else if (oneopened && items[i]->opened && mouseY < 20)
				items[i]->closemenu();

			Graphics.font->print(0,0,0,x+items[i]->x+3,Graphics.h()-y-18,"%s",items[i]->title.c_str());

			if(items[i]->opened)
				items[i]->draw();
			glColor4f(1,1,1,1);
		}
		glColor4f(0,0,0,1);
	}
	else if (drawstyle == 1)
	{
		if(maxlen == -1)
		{
			for(i = 0; i < (int)items.size(); i++)
			{
				if (Graphics.font->textlen(items[i]->title.c_str()) > maxlen-50)
					maxlen = Graphics.font->textlen(items[i]->title.c_str())+50;
			}
		}
		if(opacity < 0.75f)
			opacity += (Graphics.frameticks/300.0f);
		else
			opacity = 0.75f;


		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,opacity);
		glBegin(GL_QUADS);
			glVertex2f(x, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y);
			glVertex2f(x, Graphics.h()-y);
		glEnd();
		glColor4f(0,0,0,opacity+0.25f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y-20*items.size());
			glVertex2f(x+maxlen, Graphics.h()-y);
			glVertex2f(x, Graphics.h()-y);
		glEnd();
		glColor4f(1,1,1,opacity+0.25f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x+1, Graphics.h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, Graphics.h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, Graphics.h()-y-1);
			glVertex2f(x+1, Graphics.h()-y-1);
		glEnd();
		glColor4f(0,0,0,opacity+0.25f);
		for(i = 0; i < (int)items.size(); i++)
		{
			if (Graphics.font->textlen(items[i]->title.c_str()) > maxlen-50)
				maxlen = Graphics.font->textlen(items[i]->title.c_str())+50;
			float color = 0;
			if ((mouseX > x && mouseX < x+maxlen && (mouseY) > y+i*20 && (mouseY) < y+i*20+20))
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,opacity+0.25f);
				glBegin(GL_QUADS);
					glVertex2f(x+3, Graphics.h()-y-20*i-18);
					glVertex2f(x+maxlen-3, Graphics.h()-y-20*i-18);
					glVertex2f(x+maxlen-3, Graphics.h()-y-20*i-2);
					glVertex2f(x+3, Graphics.h()-y-20*i-2);
				glEnd();
				glColor4f(0,0,0,opacity+0.25f);
				color = 1;
			}
			
			if (items[i]->ticked)
				Graphics.font->print(color,color,color,x+5,Graphics.h()-y-20*i-18,"X");
			Graphics.font->print(color,color,color,x+23,Graphics.h()-y-20*i-18,"%s",items[i]->title.c_str());
			if(items[i]->items.size() > 0)
			{
				Graphics.font->print(color,color,color,x+maxlen-12,Graphics.h()-y-20*i-18,">");
				Graphics.font->print(color,color,color,x+maxlen-12+1,Graphics.h()-y-20*i-18-1,"%c",7);
			}
			if(items[i]->opened)
				items[i]->draw();
		}
		if(w != maxlen && !updatedChildrenPos && parent != NULL)
		{
			if(parent->drawstyle != 0)
			{
				w = maxlen;
			}
			else
				updatedChildrenPos = true;
			for(unsigned int ii = 0; ii < items.size(); ii++)
			{
				items[ii]->x = x+maxlen;
				items[ii]->y = y+ii*20;
				if (items[ii]->y + (int)items[ii]->items.size()*20 > Graphics.h())
					items[ii]->y = Graphics.h() - items[ii]->items.size()*20;
			}
		}
	}
}


cMenu* cMenu::inwindow(int xx, int yy)
{
	int i,m = w;
	if (parent != NULL && parent->drawstyle == 0)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			if (Graphics.font->textlen(items[i]->title.c_str()) > m-50)
				m = Graphics.font->textlen(items[i]->title.c_str())+50;

		}
	}
		
	mouseover = false;
	if (xx > x && xx < x+m && Graphics.h()-yy > y && Graphics.h()-yy < y+(int)h() && opened)
	{
		mouseover = true;
		return this;
	}
	if (opened)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			cMenu* m = items[i]->inwindow(xx, yy);
			if (m != NULL)
				return m;
		}
	}
	return NULL;
}

void cMenu::click(int xx, int yy)
{
	unsigned int i,ii;
	int m;
	if (drawstyle == 0)
	{
		for(i = 0; i < items.size(); i++)
		{
			m = items[i]->w;
			for(ii = 0; ii < items[i]->items.size(); ii++)
			{
				if (Graphics.font->textlen(items[i]->items[ii]->title.c_str()) > m-50)
					m = Graphics.font->textlen(items[i]->items[ii]->title.c_str())+50;
			}

			
			if (mouseX > x+items[i]->x && mouseX < x+items[i]->x+m)
			{
				items[i]->opacity = 0;
				items[i]->opened = !items[i]->opened;
			}
		}
	}
	else //if (opened)
	{
		m = w;
		if(parent != NULL)
		{
			if (parent->drawstyle == 0)
			{
				for(i = 0; i < items.size(); i++)
				{
					if (Graphics.font->textlen(items[i]->title.c_str()) > m-50)
						m = Graphics.font->textlen(items[i]->title.c_str())+50;

				}
			}
		}
		for(i = 0; i < items.size(); i++)
		{

			if (mouseX > x && mouseX < x+m && (mouseY) > y+20*i && (mouseY) < y+20*i+20)
			{
				if(items[i]->item)
				{
					if (items[i]->mouseoverproc)
					{
						if(!items[i]->mouseoverproc(items[i]))
							return;
					}
					lastmenu = this;
					menu->closemenu();
					closemenu();
					((cMenuItem*)items[i])->proc((cMenuItem*)items[i]);
					return;
				}
				else
				{
					if(!opened)
						opacity = 0;
					opened = true;
					for(unsigned int ii = 0; ii < items.size(); ii++)
					{
						if(i != ii)
							items[ii]->closemenu();
					}
					items[i]->opacity = 0;
					items[i]->opened = !items[i]->opened;
					if (!items[i]->opened)
						items[i]->closemenu();
				}
			}
		}
	}
}



bool cmp(cMenu* a, cMenu* b)
{
	if (a->item == b->item) 
		return a->title < b->title;
	else 
		return (a->item > b->item);
}


cMenu* cMenu::getNext(cMenu* curitem)
{
	for(unsigned int i = 0; i < items.size(); i++)
	{
		if(items[i] == curitem)
		{
			if (i < (items.size()) - 1)
			{
				if (items[i+1]->item)
					return items[i+1];
				else
					return items[i+1]->getFirstItem();
			}
		}
	}
	
	return parent->getNext(this);
	
}

cMenu* cMenu::getPrev(cMenu* curitem)
{
	for(unsigned int i = 0; i < items.size(); i++)
	{
		if(items[i] == curitem)
		{
			if (i > 0)
			{
				if (items[i-1]->item)
					return items[i-1];
				else
					return items[i-1]->getLastItem();
			}
		}
	}
	
	return parent->getPrev(this);
}


cMenu* cMenu::getFirstItem()
{
	if(items.size() == 0 || item)
		return this;
	if(items[0]->item)
		return items[0];
	else
		return items[0]->getFirstItem();
}

cMenu* cMenu::getLastItem()
{
	if(items.size() == 0 || item)
		return this;
	if(items[items.size()-1]->item)
		return items[items.size()-1];
	else
		return items[items.size()-1]->getLastItem();
	
}



cMenu* cMenu::finddata(std::string d)
{
	if(item)
	{
		if(((cMenuItem*)this)->data == d)
			return this;
	}
	for(unsigned int i = 0; i < items.size(); i++)
	{
		cMenu* m = items[i]->finddata(d);
		if (m != NULL)
			return m;
	}
	return NULL;
}
