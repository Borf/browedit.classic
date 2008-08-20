#ifndef __PLUGIN_BASE_H__
#define __PLUGIN_BASE_H__

#include <string>
//using namespace std;
class cWorld;
class cWindow;
class cTexture;
class cFont;
class TiXmlDocument;
class cWindow;


class cPluginBase
{
private:
	cWindow*			(*addxml)(std::string);
public:
	std::string			name;
	std::string			menu;

	cPluginBase(std::string,std::string);

	void				SetFunctions(cWindow* (*func)(std::string));
	virtual bool		action(cWorld*) = 0;
	cWindow*			addWindow(std::string);

};


#endif
