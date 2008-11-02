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
	~cWM();
	static int						init(std::string);
	static int						openwindow(WINDOW_TYPE);
	static int						closewindow(WINDOW_TYPE, bool = false);
	static void						showwindow(WINDOW_TYPE);
	static void						hidewindow(WINDOW_TYPE);
	static void						togglewindow(WINDOW_TYPE);
	static bool						existswindow(WINDOW_TYPE);
	static int						draw();
	static void						save();
	static void						click(bool);
	static void						drag(cWindow*);
	static cWindow*					inwindow();
	static bool						onchar(char, bool shift);
	static bool						onkeyup(int, bool shift);
	static bool						onkeydown(int, bool shift);
	static void						doubleclick();
	static void						rightclick();
	static cWindow*					getwindow(WINDOW_TYPE);
	static void						unload();

	static void						addwindow(cWindow*);
	static void						CleanWindows();

	static void						ShowMessage(std::string);
	static void						ConfirmWindow(std::string, cConfirmWindow::cConfirmWindowCaller* caller);
	static bool						ConfirmWindow(std::string);
	static cWindow*					InputWindow(std::string, cInputWindow::cInputWindowCaller* caller);
	static std::string				InputWindow(std::string, std::string = "");
	static cWindow*					XmlWindow(std::string);

	static void						printdebug();
	static void						defocus();

	//properties
	static std::vector<cWindow*>	windows; //vector of windows, 0 = topwindow
	static cTexture*				texture;
	static cFont*					font;
	static TiXmlDocument			skin;
	static float					color[4];
	static float					colorblur[4];
	static int						focus;
	


};

#endif
