#ifndef __WINDOWCHATBOX_H__
#define __WINDOWCHATBOX_H__

#define FONTHEIGHT 13

#include "windowobject.h"
#include <string>
#include <vector>
using namespace std;

class cWindowChatBox : public cWindowObject
{
protected:
	vector<string>	values;
	int				liststart;
public:
	cWindowChatBox(cWindow* parent) : cWindowObject(parent)
	{
		w = 40;
		h = 4;
		x = 5;
		y = 20;
		alignment = ALIGN_BOTTOMLEFT;
		liststart = 0;
		type = OBJECT_CHATBOX;
	}
	virtual ~cWindowChatBox() {}
	virtual void draw(int, int, int, int);
	virtual void click();
	bool onkeydown(int);
//	bool onchar(char);

	void SetText(int, string);
	void SetInt(int, int);

	void drag();
	void doubleclick();
	cWindowObject* inobject();
};




#endif
