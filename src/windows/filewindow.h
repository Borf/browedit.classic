#ifndef __FILEWINDOW_H__
#define __FILEWINDOW_H__

#include <wm/window.h>

#include <wm/windowroundbutton.h>
#include <wm/windowbutton.h>
#include <wm/windowinputbox.h>


class cFileWindow : public cWindow
{

	class cOkButton : public cWindowButton
	{
	public:
		void (*callback)(std::string);
		cOkButton(cWindow* parent, void (*pCallback)(std::string), TiXmlDocument &skin);
		void click();
	};
public:
	class cWindowFilterBox : public cWindowInputBox
	{
	public:
		cWindowFilterBox(cWindow* parent, TiXmlDocument &skin);
		void onChange();
	};

	std::vector<std::string> mapnames;

	cFileWindow(cTexture* t, cFont* f, void (*pCallback)(std::string), TiXmlDocument &skin);
	void resizeTo(int ww, int hh);
};

#endif
