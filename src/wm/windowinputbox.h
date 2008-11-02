#ifndef __WINDOWINPUTBOX_H__
#define __WINDOWINPUTBOX_H__

#include "windowobject.h"
#include <string>
//using namespace std;

class cWindowInputBox : public cWindowObject
{
protected:
	std::string text;
	int cursor;
	int startindex;
	std::string mask;
	bool editable;
	int selectionstart;
	int defaultheight;
public:
	cWindowInputBox(cWindow* parent, TiXmlDocument* skin = NULL);
	virtual ~cWindowInputBox() {}
	virtual void draw(int,int,int,int);
	virtual void click();
	virtual void doubleClick();
	bool onKeyDown(int,bool);
	bool onChar(char,bool);

	void setText(int, std::string);
	void setInt(int, int);
	virtual void onChange() {};
	int getInt(int);
	std::string getText(int);
};


class cWindowFloatInputBox : public cWindowInputBox
{
	float lastvalue;
	bool firstTime;
public:
	float* floatje;
	cWindowFloatInputBox(cWindow* parent, TiXmlDocument* skin = NULL);
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
	void setInt(int id, int val);
	bool onKeyDown(int keyid, bool shift);
};

class cWindowStringInputBox : public cWindowInputBox
{
	std::string lastvalue;
	bool firstTime;
public:
	std::string* stringetje;
	cWindowStringInputBox(cWindow* parent, TiXmlDocument* skin = NULL);
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
	void setInt(int id, int val);
	bool onKeyDown(int keyid, bool shift);
};






class cWindowLongInputBox : public cWindowInputBox
{
	long lastvalue;
	bool firstdraw;
public:
	long* longje;
	cWindowLongInputBox(cWindow* parent, TiXmlDocument* skin = NULL);
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
	void setInt(int id, int val);
	bool onKeyDown(int keyid, bool shift);
};




#endif
