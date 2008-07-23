#ifndef __PLUGIN_BASE_H__
#define __PLUGIN_BASE_H__

#include <string>
using namespace std;
class cWorld;
class cWindow;
class cTexture;
class cFont;
class TiXmlDocument;
class cWindow;


class cPluginBase
{
private:
	cWindow* (*addxml)(string);
public:
	string name;
	string menu;

	cPluginBase(string,string);

	void SetFunctions(cWindow* (*func)(string));
	virtual bool	action(cWorld*) = 0;

	cWindow* addWindow(string);

};


#endif
