#include "menu.h"
#include <GL/gl.h>

#include "graphics.h"
#include "font.h"

extern cMenu*			lastmenu;



cMenu::cMenu()
{
	ticked = false;
	opened = false;
	w = 100;
	mouseOver = false;
	maxlen = -1;
	mouseoutproc = NULL;
	mouseoverproc = NULL;
	parent = NULL;
	updatedChildrenPos = false;
	opacity = 0;
}

void cMenu::unMouseOver()
{
	mouseOver = false;
	for(int i = 0; i < (int)items.size(); i++)
		items[i]->unMouseOver();
}

void cMenu::closeMenu()
{
	opacity = 0;
	opened = false;
	if(mouseoutproc!= NULL)
		mouseoutproc(this);
	for(int i = 0; i < (int)items.size(); i++)
		items[i]->closeMenu();
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
	return (unsigned int) ((drawStyle==0) ? 20 : 20*items.size());
}

cMenuItem::cMenuItem() : cMenu()
{
	item = true;
	pdata = NULL;
}


void cMenu::draw()
{
	int i;
	if(drawStyle == 0)
	{
		bool oneopened = false;
		for(i = 0; i < (int)items.size(); i++)
			if (items[i]->opened)
				oneopened = true;
		
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			glVertex2f(0, cGraphics::h()-20);
			glVertex2f(cGraphics::w(), cGraphics::h()-20);
			glVertex2f(cGraphics::w(), cGraphics::h());
			glVertex2f(0, cGraphics::h());
		glEnd();
		glBegin(GL_LINES);
		for(i = 0; i < 10; i++)
		{
			glColor4f(i/3.0f,i/3.0f,i/3.0f,1);
			glVertex2f(0, cGraphics::h()-20+i);
			glVertex2f(cGraphics::w(), cGraphics::h()-20+i);
		}
		glEnd();
		glColor4f(0,0,0,1);
		glBegin(GL_QUADS);
			glVertex2f(0, cGraphics::h()-20);
			glVertex2f(cGraphics::w(), cGraphics::h()-20);
			glVertex2f(cGraphics::w(), cGraphics::h()-19);
			glVertex2f(0, cGraphics::h()-19);
		glEnd();
		for(i = 0; i < (int)items.size(); i++)
		{
			if (cGraphics::cMouse::x >= x + items[i]->x && cGraphics::cMouse::x < x + items[i]->x + items[i]->w && cGraphics::cMouse::y < 20)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,0.5);
				glBegin(GL_QUADS);
					glVertex2f(x+items[i]->x, 		cGraphics::h()-y-15);
					glVertex2f(x+items[i]->x+items[i]->w, 	cGraphics::h()-y-15);
					glVertex2f(x+items[i]->x+items[i]->w, 	cGraphics::h()-y);
					glVertex2f(x+items[i]->x, 		cGraphics::h()-y);
					if (oneopened && !items[i]->opened)
					{
						items[i]->opacity = 0;
						items[i]->opened = true;
					}
				glEnd();
				glColor4f(0,0,0,1);
			}
			else if (oneopened && items[i]->opened && cGraphics::cMouse::y < 20)
				items[i]->closeMenu();

			cGraphics::font->print(0,0,0,x+items[i]->x+3,cGraphics::h()-y-18,"%s",items[i]->title.c_str());

			if(items[i]->opened)
				items[i]->draw();
			glColor4f(1,1,1,1);
		}
		glColor4f(0,0,0,1);
	}
	else if (drawStyle == 1)
	{
		if(maxlen == -1)
		{
			for(i = 0; i < (int)items.size(); i++)
			{
				if (cGraphics::font->textLen(items[i]->title.c_str()) > maxlen-50)
					maxlen = cGraphics::font->textLen(items[i]->title.c_str())+50;
			}
		}
		if(opacity < 0.75f)
			opacity += (cGraphicsBase::getFrameTicks()/300.0f);
		else
			opacity = 0.75f;


		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,opacity);
		glBegin(GL_QUADS);
			glVertex2f(x, cGraphics::h()-y-20*items.size());
			glVertex2f(x+maxlen, cGraphics::h()-y-20*items.size());
			glVertex2f(x+maxlen, cGraphics::h()-y);
			glVertex2f(x, cGraphics::h()-y);
		glEnd();
		glColor4f(0,0,0,opacity+0.25f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x, cGraphics::h()-y-20*items.size());
			glVertex2f(x+maxlen, cGraphics::h()-y-20*items.size());
			glVertex2f(x+maxlen, cGraphics::h()-y);
			glVertex2f(x, cGraphics::h()-y);
		glEnd();
		glColor4f(1,1,1,opacity+0.25f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x+1, cGraphics::h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, cGraphics::h()-y-20*items.size()+1);
			glVertex2f(x+maxlen-1, cGraphics::h()-y-1);
			glVertex2f(x+1, cGraphics::h()-y-1);
		glEnd();
		glColor4f(0,0,0,opacity+0.25f);
		for(i = 0; i < (int)items.size(); i++)
		{
			if (cGraphics::font->textLen(items[i]->title.c_str()) > maxlen-50)
				maxlen = cGraphics::font->textLen(items[i]->title.c_str())+50;
			float color = 0;
			if ((cGraphics::cMouse::x > x && cGraphics::cMouse::x < x+maxlen && (cGraphics::cMouse::y) > y+i*20 && (cGraphics::cMouse::y) < y+i*20+20))
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.2f,0.2f,0.9f,opacity+0.25f);
				glBegin(GL_QUADS);
					glVertex2f(x+3, cGraphics::h()-y-20*i-18);
					glVertex2f(x+maxlen-3, cGraphics::h()-y-20*i-18);
					glVertex2f(x+maxlen-3, cGraphics::h()-y-20*i-2);
					glVertex2f(x+3, cGraphics::h()-y-20*i-2);
				glEnd();
				glColor4f(0,0,0,opacity+0.25f);
				color = 1;
			}
			
			if (items[i]->ticked)
				cGraphics::font->print(color,color,color,x+5,cGraphics::h()-y-20*i-18,"X");
			cGraphics::font->print(color,color,color,x+23,cGraphics::h()-y-20*i-18,"%s",items[i]->title.c_str());
			if(items[i]->items.size() > 0)
			{
				cGraphics::font->print(color,color,color,x+maxlen-12,cGraphics::h()-y-20*i-18,">");
				cGraphics::font->print(color,color,color,x+maxlen-12+1,cGraphics::h()-y-20*i-18-1,"%c",7);
			}
			if(items[i]->opened)
				items[i]->draw();
		}
		if(w != maxlen && !updatedChildrenPos && parent != NULL)
		{
			if(parent->drawStyle != 0)
			{
				w = maxlen;
			}
			else
				updatedChildrenPos = true;
			for(unsigned int ii = 0; ii < items.size(); ii++)
			{
				items[ii]->x = x+maxlen;
				items[ii]->y = y+ii*20;
				if (items[ii]->y + (int)items[ii]->items.size()*20 > cGraphics::h())
					items[ii]->y = cGraphics::h() - items[ii]->items.size()*20;
			}
		}
	}
}


cMenu* cMenu::inWindow(int xx, int yy)
{
	int i,m = w;
	if (parent != NULL && parent->drawStyle == 0)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			if (cGraphics::font->textLen(items[i]->title.c_str()) > m-50)
				m = cGraphics::font->textLen(items[i]->title.c_str())+50;

		}
	}
		
	mouseOver = false;
	if (xx > x && xx < x+m && cGraphics::h()-yy > y && cGraphics::h()-yy < y+(int)h() && opened)
	{
		mouseOver = true;
		return this;
	}
	if (opened)
	{
		for(i = 0; i < (int)items.size(); i++)
		{
			cMenu* m = items[i]->inWindow(xx, yy);
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
	if (drawStyle == 0)
	{
		for(i = 0; i < items.size(); i++)
		{
			m = items[i]->w;
			for(ii = 0; ii < items[i]->items.size(); ii++)
			{
				if (cGraphics::font->textLen(items[i]->items[ii]->title.c_str()) > m-50)
					m = cGraphics::font->textLen(items[i]->items[ii]->title.c_str())+50;
			}

			
			if (cGraphics::cMouse::x > x+items[i]->x && cGraphics::cMouse::x < x+items[i]->x+m)
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
			if (parent->drawStyle == 0)
			{
				for(i = 0; i < items.size(); i++)
				{
					if (cGraphics::font->textLen(items[i]->title.c_str()) > m-50)
						m = cGraphics::font->textLen(items[i]->title.c_str())+50;

				}
			}
		}
		for(i = 0; i < items.size(); i++)
		{

			if (cGraphics::cMouse::x > x && cGraphics::cMouse::x < x+m && (cGraphics::cMouse::y) > y+20*i && (cGraphics::cMouse::y) < y+20*i+20)
			{
				if(items[i]->item)
				{
					if (items[i]->mouseoverproc)
					{
						if(!items[i]->mouseoverproc(items[i]))
							return;
					}
					lastmenu = this;
					cGraphics::menu->closeMenu();
					closeMenu();
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
							items[ii]->closeMenu();
					}
					items[i]->opacity = 0;
					items[i]->opened = !items[i]->opened;
					if (!items[i]->opened)
						items[i]->closeMenu();
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



cMenu* cMenu::findData(std::string d)
{
	if(item)
	{
		if(((cMenuItem*)this)->data == d)
			return this;
	}
	for(unsigned int i = 0; i < items.size(); i++)
	{
		cMenu* m = items[i]->findData(d);
		if (m != NULL)
			return m;
	}
	return NULL;
}

void cMenu::update()
{
	for(unsigned int i = 0; i < items.size(); i++)
		items[i]->update();
}
void cMenuItemLinked::update()
{
	if(title == "Grid")
		sleep(0);
	if(pdata)
		ticked = *((bool*)pdata);
	cMenu::update();
}

cMenuItemLinked::cMenuItemLinked()
{
	cMenuItem();	
	pdata = NULL;
}