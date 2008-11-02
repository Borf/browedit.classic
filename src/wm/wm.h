#ifndef __WM_H__
#define __WM_H__

class cFont;
class cTexture;

#include <map>
//using namespace std;

#include <windows/confirmwindow.h>
#include <windows/inputwindow.h>


#include "window.h"

class cWM
{
public:
	TiXmlDocument			skin;
	~cWM();
	int						init(std::string);
	int						openwindow(WINDOW_TYPE);
	int						closewindow(WINDOW_TYPE, bool = false);
	void					showwindow(WINDOW_TYPE);
	void					hidewindow(WINDOW_TYPE);
	void					togglewindow(WINDOW_TYPE);
	bool					existswindow(WINDOW_TYPE);
	int						draw();
	void					save();
	void					click(bool);
	void					drag(cWindow*);
	cWindow*				inwindow();
	bool					onchar(char, bool shift);
	bool					onkeyup(int, bool shift);
	bool					onkeydown(int, bool shift);
	void					doubleclick();
	void					rightclick();
	cWindow*				getwindow(WINDOW_TYPE);
	void					unload();

//properties
	std::vector<cWindow*>	windows; //vector of windows, 0 = topwindow
	cTexture*				texture;
	cFont*					font;
	float					color[4];
	float					colorblur[4];

	void					addwindow(cWindow*);
	void					CleanWindows();

	int						focus;
	void					ShowMessage(std::string);
	void					ConfirmWindow(std::string, cConfirmWindow::cConfirmWindowCaller* caller);
	bool					ConfirmWindow(std::string);
	cWindow*				InputWindow(std::string, cInputWindow::cInputWindowCaller* caller);
	std::string				InputWindow(std::string, std::string = "");
	cWindow*				XmlWindow(std::string);

	void					printdebug();
	void					defocus();
};

#endif
