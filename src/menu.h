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
	static float		barColor[4];
	static float		barFontColor[4];
	static float		barHighlightColor[4];

	static float		menuColor[3];
	static float		menuFontColor[3];
	static float		menuHighlightColor[3];
	static float		opacityStart;
	static float		opacityEnd;
	static void			initSkin(Json::Value &);




	float				opacity;
	std::string			title;
	bool				item;
	std::vector<cMenu*>	items;
	int					drawStyle;
	int					x, y, w;
	bool				ticked;
	bool				opened;
		
	bool				mouseOver;
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
	virtual void		draw();
	virtual void		update();
	cMenu*				inWindow(int, int);
	void				click(int, int);
	void				unMouseOver();
	void				closeMenu();
	cMenu* find(std::string s, bool recurse = true);

	void				sort();
	cMenu*				findData(std::string d);
	~cMenu();
};

class cMenuItem : public cMenu
{
public:
	cMenuItem();
	bool (*proc) (cMenuItem* m);
	std::string		data;
	std::string		data2;
	void*			pdata;
	int				data3;
};

class cMenuItemLinked : public cMenuItem
{
public:
	cMenuItemLinked();
	void update();

};

#endif
