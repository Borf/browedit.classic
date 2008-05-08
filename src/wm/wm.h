#ifndef __WM_H__
#define __WM_H__

#include <common.h>
#include <font.h>
#include <texture.h>
#include <list>

#include <map>
using namespace std;

#include "confirmwindow.h"
#include "inputwindow.h"


#include "window.h"

class cWM
{
public:
	TiXmlDocument		skin;
	~cWM();
	int					init(string);
	int					openwindow(WINDOW_TYPE);
	int					closewindow(WINDOW_TYPE, bool = false);
	void				showwindow(WINDOW_TYPE);
	void				hidewindow(WINDOW_TYPE);
	void				togglewindow(WINDOW_TYPE);
	bool				existswindow(WINDOW_TYPE);
	int					draw();
	void				save();
	void				click(bool);
	void				drag(cWindow*);
	cWindow*			inwindow();
	bool				onchar(char, bool shift);
	bool				onkeyup(int, bool shift);
	bool				onkeydown(int, bool shift);
	void				doubleclick();
	void				rightclick();
	cWindow*			getwindow(WINDOW_TYPE);
	void				unload();

//properties
	vector<cWindow*>	windows; //vector of windows, 0 = topwindow
	cTexture*			texture;
	cFont				font;
	float				color[4];
	float				colorblur[4];

	void				addwindow(cWindow*);
	void				delwindow(cWindow*, bool = true);
	void				CleanWindows();

	int					focus;
	void				MessageBox(string);
	void				ConfirmWindow(string, cConfirmWindow::cConfirmWindowCaller* caller);
	bool				ConfirmWindow(string);
	cWindow*			InputWindow(string, cInputWindow::cInputWindowCaller* caller);
	string				InputWindow(string, string = "");

	void				printdebug();
	void				defocus();
};

#endif
