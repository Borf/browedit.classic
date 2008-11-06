#ifndef __SETTINGS_H__
#define __SETTINGS_H__


#include <string>
#include <tinyxml/tinyxml.h>

class cSettings
{
public:
	static std::string		configFileName;
	static TiXmlDocument	config;
	static std::string		fontName;
	static float			paintSpeed;
	static std::string		roDir;
	static std::string		skinFile;

};


#endif
