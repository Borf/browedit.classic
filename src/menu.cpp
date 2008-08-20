#include "menu.h"


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
	mergesort(items, cmp);
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