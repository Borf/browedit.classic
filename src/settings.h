#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <common.h>
#include <string>
#include <vector>
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
	static int				keyMap[SDLK_LAST-SDLK_FIRST];
	static unsigned int		undoSize;
	static float			cameraSmoothing;

	
	static std::vector<std::pair<std::string, std::string> >	translations;
	static std::vector<std::string>								objectFiles;
	static std::vector<std::string>								textureFiles;
	static std::vector<std::string>								soundFiles;

};

#endif
