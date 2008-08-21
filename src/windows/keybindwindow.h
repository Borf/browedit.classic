#ifndef __KEYBINDWINDOW_H__
#define __KEYBINDWINDOW_H__

#include <wm/window.h>
#include <wm/windowinputbox.h>
#include <wm/windowbutton.h>


std::string keytostring(SDLKey key);


class cKeyBindWindow :	public cWindow
{
public:
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};

	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, TiXmlDocument &skin);
		void click();
	};


	class cKeyBindBox :	public cWindowInputBox
	{
		SDLKey key;
	public:
		cKeyBindBox(cWindow* parent, TiXmlDocument &skin);
		bool onKeyDown(int keyid);
		bool onKeyUp(int keyid);
		bool onChar(char ch);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void setInt(int id, intptr_t val);
		intptr_t getInt(int id);
	};


	class cClearButton : public cWindowButton
	{
		cKeyBindBox* clearbox;
	public:
		cClearButton(cWindow* parent, cKeyBindBox* box, TiXmlDocument &skin);
		void click();
	};

	void addclearbutton(cKeyBindBox* box, TiXmlDocument &skin);
	cKeyBindBox* addbox(std::string name, int x, int y, int defval, TiXmlDocument &skin);
	cKeyBindWindow(cTexture* t, cFont* f, TiXmlDocument &skin);

};








#endif

