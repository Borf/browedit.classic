#ifndef __PLUGIN_BASE_H__
#define __PLUGIN_BASE_H__

#include <string>
using namespace std;
class cWorld;
class cWindow;
class cTexture;
class cFont;
class TiXmlDocument;

class cPluginBase
{
public:
	string name;
	string menu;

	cPluginBase(string,string);
	virtual bool	action(cWorld*) = 0;

	void addWindow(string);

};


#endif
