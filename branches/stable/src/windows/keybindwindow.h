#ifndef __KEYBINDWINDOW_H__
#define __KEYBINDWINDOW_H__

#include <wm/window.h>
#include <wm/windowinputbox.h>
#include <wm/windowbutton.h>
#include <SDL/SDL.h>


std::string keytostring(SDLKey key);


class cKeyBindWindow :	public cWindow
{
public:
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};

	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};


	class cKeyBindBox :	public cWindowInputBox
	{
		SDLKey key;
	public:
		cKeyBindBox(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		bool onKeyDown(int keyid,bool);
		bool onKeyUp(int keyid, bool);
		bool onChar(char ch, bool);
		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);
		void setInt(int id, int val);
		int getInt(int id);
	};


	class cClearButton : public cWindowButton
	{
		cKeyBindBox* clearbox;
	public:
		cClearButton(cWindow* parent, cKeyBindBox* box, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};

	void addclearbutton(cKeyBindBox* box, TiXmlDocument* skin);
	cKeyBindBox* addbox(std::string name, int x, int y, int defval, TiXmlDocument* skin = &cWM::skin);
	cKeyBindWindow();

};








#endif

