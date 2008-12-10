#ifndef __INTERFACEIMPLEMENTATION_H__
#define __INTERFACEIMPLEMENTATION_H__

#include "plugins/base/interface.h"

class cBrowInterfaceImplementation : public cBrowInterface
{
public:
	void fixNormals();
	cWindow* addXmlWindow(const char*);
	int getSelectedTextureIndex();
	void render();
	const char* inputWindow(const char*, const char*);
	void messageWindow(const char*);
	cRSMModel* loadModel(const char*);
	void removeModel(int);
	void removeSprite(int);
};


#endif
