#ifndef __SETTINGS_H__
#define __SETTINGS_H__


#include <string>
#include <tinyxml/tinyxml.h>

enum eMode
{
	MODE_TEXTURE,
	MODE_HEIGHTDETAIL,
	MODE_HEIGHTGLOBAL,
	MODE_WALLS,
	MODE_OBJECTS,
	MODE_GAT,
	MODE_WATER,
	MODE_EFFECTS,
	MODE_SOUNDS,
	MODE_LIGHTS,
	MODE_OBJECTGROUP,
	MODE_SPRITE,
	MODE_TEXTUREPAINT,
};


class cSettings
{
public:
	static std::string		configFileName;
	static TiXmlDocument	config;
	static std::string		fontName;
	static float			paintSpeed;
	static std::string		roDir;
	static std::string		skinFile;
	static bool				running;
	static eMode			editMode;
	static TiXmlDocument	msgTable;

};


#endif
