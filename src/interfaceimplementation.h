#ifndef __INTERFACEIMPLEMENTATION_H__
#define __INTERFACEIMPLEMENTATION_H__

#include "plugins/base/interface.h"

class cBrowInterfaceImplementation : public cBrowInterface
{
public:
	void fixNormals();
	cWindow* addXmlWindow(char*);
	int getSelectedTextureIndex();
	void render();
	char* inputWindow(char*, char*);
	void messageWindow(char*);
	cRSMModel* loadModel(char*);
	void removeModel(int);
	void removeSprite(int);
};


#endif
