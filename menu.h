#ifndef __MENU_H__
#define __MENU_H__

class cMenu
{
	public:
		string title;
		bool item;
		vector<cMenu*> items;
		int drawstyle;
		int x, y, w;
		bool ticked;
		int h()
		{
			return (drawstyle==0) ? 20 : 20*items.size();
		}
		bool opened;
		
		void draw();
		bool mouseover;
		int maxlen;
	
		cMenu()
		{
			ticked = false;
			opened = false;
			w = 100;
			mouseover = false;
			maxlen = -1;
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
};

class cMenuItem : public cMenu
{
public:
	bool (*proc) (cMenuItem* m);
	cMenuItem()
	{
		item = true;
	}
};
 

#endif
