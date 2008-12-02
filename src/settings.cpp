#include "settings.h"

std::string			cSettings::configFileName;
TiXmlDocument		cSettings::config;
std::string			cSettings::fontName =		"tahoma";
float				cSettings::paintSpeed =		100;
std::string			cSettings::roDir =			"";
std::string			cSettings::skinFile =		"";
bool				cSettings::running =		true;
eMode				cSettings::editMode =		MODE_TEXTURE;
TiXmlDocument		cSettings::msgTable;
int					cSettings::keyMap[SDLK_LAST-SDLK_FIRST];
unsigned int		cSettings::undoSize = 50;
float				cSettings::cameraSmoothing = 0.75;
std::vector<std::string>							cSettings::objectFiles;
std::vector<std::pair<std::string, std::string> >	cSettings::translations;
std::vector<std::string>							cSettings::textureFiles;
std::vector<std::string>							cSettings::soundFiles;
