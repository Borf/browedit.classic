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
	std::string			name;
	std::string			menu;

	cPluginBase(std::string,std::string);

	void				setInterface(cBrowInterface*);
	virtual bool		action(cWorld*) = 0;
	cBrowInterface*		interface;
};


#endif
