#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <common.h>
#include <string>
#include <vector>
#include <json/json.h>

enum eMode
{
	MODE_TEXTURE		 = 0,
	MODE_HEIGHTDETAIL	 = 1,
	MODE_HEIGHTGLOBAL	 = 2,
	MODE_WALLS			 = 3,
	MODE_OBJECTS		 = 4,
	MODE_GAT			 = 5,
	MODE_WATER			 = 6,
	MODE_EFFECTS		 = 7,
	MODE_SOUNDS			 = 8,
	MODE_LIGHTS			 = 9,
	MODE_OBJECTGROUP	 = 10,
	MODE_SPRITE			 = 11,
	MODE_TEXTUREPAINT	 = 12,
};


class cSettings
{
public:
	static std::string		configFileName;
	static Json::Value		config;
	static std::string		fontName;
	static float			paintSpeed;
	static std::string		roDir;
	static std::string		skinFile;
	static bool				running;
	static eMode			editMode;
	static Json::Value		msgTable;
	static int				keyMap[SDLK_LAST-SDLK_FIRST];
	static unsigned int		undoSize;
	static float			cameraSmoothing;

	
	static std::vector<std::pair<std::string, std::string> >	translations;
	static std::vector<std::string>								objectFiles;
	static std::vector<std::string>								textureFiles;
	static std::vector<std::string>								soundFiles;

};

#endif
