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
class cBrowInterface;

class cPluginBase
{
public:
	char				name[128];
	char				menu[512];

	cPluginBase(const char*, const char*);

	void				setInterface(cBrowInterface*);
	virtual bool		action(cWorld*) = 0;
	cBrowInterface*		browInterface;
};


#endif
