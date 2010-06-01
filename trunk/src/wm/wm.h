#ifndef __WM_H__
#define __WM_H__


#include <vector>
#include <string>
#include <wm/windows.h>

#include <bengine/forwards.h>
#include <json/json.h>

class cFont;
class cInputWindowCaller;
class cConfirmWindowCaller;
class cWindowObject;
class cWindow;

class cWM
{
public:
	~cWM();
	static int						init(std::string);
	static int						openWindow(WINDOW_TYPE);
	static int						closeWindow(WINDOW_TYPE, bool = false);
	static void						showWindow(WINDOW_TYPE);
	static void						hideWindow(WINDOW_TYPE);
	static void						toggleWindow(WINDOW_TYPE);
	static bool						existsWindow(WINDOW_TYPE);
	static int						draw();
	static void						save();
	static void						click(bool);
	static void						drag(cWindow*);
	static cWindow*					inWindow();
	static bool						onChar(char, bool shift);
	static bool						onKeyUp(int, bool shift);
	static bool						onKeyDown(int, bool shift);
	static void						onDoubleClick();
	static void						onRightClick();
	static cWindow*					getWindow(WINDOW_TYPE);
	static void						unLoad();

	static void						addWindow(cWindow*);
	static void						cleanWindows();

	static void						showMessage(std::string);
	static void						confirmWindow(std::string, cConfirmWindowCaller* caller);
	static bool						confirmWindow(std::string);
	static cWindow*					inputWindow(std::string, cInputWindowCaller* caller);
	static std::string				inputWindow(std::string, std::string = "");
	static cWindow*					JsonWindow(std::string);

	static void						printDebug();
	static void						defocus();

	//properties
	static std::vector<cWindow*>	windows; //vector of windows, 0 = topwindow
	static bEngine::cTexture*		texture;
	static cFont*					font;
	static Json::Value				skin;
	static float					color[4];
	static float					colorBlur[4];
	static int						focus;
	static int						movement;
	
	static cWindow*					draggingWindow;
	static cWindowObject*			draggingObject;


};

#endif
