#ifndef __MENU_H__
#define __MENU_H__


#include "common.h"
#include <string>

class cMenuItem;
class cMenu;
bool cmp(cMenu* a, cMenu* b);

class cMenu
{
	public:
		float				opacity;
		std::string			title;
		bool				item;
		std::vector<cMenu*>	items;
		int					drawstyle;
		int					x, y, w;
		bool				ticked;
		bool				opened;
			
		bool				mouseover;
		int					maxlen;

		cMenu*				parent;
		bool				updatedChildrenPos;
			
		bool				(*mouseoverproc) (cMenu* m);
		bool				(*mouseoutproc) (cMenu* m);

		cMenu*				getNext(cMenu* curitem);
		cMenu*				getPrev(cMenu* curitem);

		cMenu*				getFirstItem();
		cMenu*				getLastItem();

		cMenu();
		unsigned int		h();
		void				draw();
		cMenu*				inwindow(int, int);
		void				click(int, int);
		void				unmouseover();
		void				closemenu();
		cMenu* find(std::string s, bool recurse = true);

		void				sort();
		cMenu*				finddata(std::string d);

};

class cMenuItem : public cMenu
{
public:
	bool (*proc) (cMenuItem* m);
	std::string		data;
	std::string		data2;
	void*			pdata;
	cMenuItem();

};
 

#endif
