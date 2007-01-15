#ifndef __MENU_H__
#define __MENU_H__
class cMenuItem;
class cMenu;
bool cmp(cMenu* a, cMenu* b);

class cMenu
{
	public:
		string title;
		bool item;
		vector<cMenu*> items;
		int drawstyle;
		int x, y, w;
		bool ticked;
		unsigned int h()
		{
			return (unsigned int) ((drawstyle==0) ? 20 : 20*items.size());
		}
		bool opened;
		
		void draw();
		bool mouseover;
		int maxlen;

		cMenu* parent;
		bool updatedchildrenpos;
		
		bool (*mouseoverproc) (cMenu* m);
		bool (*mouseoutproc) (cMenu* m);

		cMenu* getnext(cMenu* curitem);
		cMenu* getprev(cMenu* curitem);

		cMenu* getfirstitem();
		cMenu* getlastitem();

		cMenu()
		{
			ticked = false;
			opened = false;
			w = 100;
			mouseover = false;
			maxlen = -1;
			mouseoutproc = NULL;
			mouseoverproc = NULL;
			parent = NULL;
			updatedchildrenpos = false;
		}
		cMenu* inwindow(int, int);
		void click(int, int);
		void unmouseover()
		{
			mouseover = false;
			for(int i = 0; i < (int)items.size(); i++)
				items[i]->unmouseover();
		}
		void closemenu()
		{
			opened = false;
			if(mouseoutproc!= NULL)
				mouseoutproc(this);
			for(int i = 0; i < (int)items.size(); i++)
				items[i]->closemenu();
		}
		cMenu* find(string s)
		{
			int i;
			if (title == s)
				return this;
			else
			{
				cMenu* m = NULL;
				for(i = 0; i < (int)items.size() && m == NULL; i++)
					m = items[i]->find(s);
				if (m != NULL)
					return m;
			}
			return NULL;
		}

		void sort()
		{
			mergesort(items, cmp);
			for(unsigned int i = 0; i < items.size(); i++)
				items[i]->sort();
		}
		cMenu* finddata(string d);

};

class cMenuItem : public cMenu
{
public:
	bool (*proc) (cMenuItem* m);
	string data;
	string data2;
	cMenuItem()
	{
		item = true;
	}

};
 

#endif
