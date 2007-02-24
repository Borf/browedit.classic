#ifndef __WM_H__
#define __WM_H__

#include "../common.h"
#include "../font.h"
#include "../texture.h"
#include <list>

#include <map>
using namespace std;

#include "confirmwindow.h"


#include "window.h"

class cWM
{
private:
public:
	~cWM();
	int					init();
	int					openwindow(WINDOW_TYPE);
	int					closewindow(WINDOW_TYPE);
	void				showwindow(WINDOW_TYPE);
	void				hidewindow(WINDOW_TYPE);
	void				togglewindow(WINDOW_TYPE);
	bool				existswindow(WINDOW_TYPE);
	int					draw();
	void				save();
	void				click(bool);
	void				drag(cWindow*);
	cWindow*			inwindow();
	bool				onchar(char);
	bool				onkeyup(int);
	bool				onkeydown(int);
	void				doubleclick();
	void				rightclick();
	cWindow*			getwindow(WINDOW_TYPE);
//properties
	vector<cWindow*>	windows; //vector of windows, 0 = topwindow
	cTexture			texture;
	cFont				font;

	void				addwindow(cWindow*);
	void				delwindow(cWindow*, bool = true);
	void				CleanWindows();

	int					focus;
	void				MessageBox(string);
	void				ConfirmWindow(string, cConfirmWindow::cConfirmWindowCaller* caller);

	void				printdebug();
	void				defocus();
};

#endif
