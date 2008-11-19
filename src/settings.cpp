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
