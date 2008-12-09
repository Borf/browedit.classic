#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class cWindow;
class cRSMModel;

class cBrowInterface
{
public:
	virtual void fixNormals() = 0;
	virtual cWindow* addXmlWindow(char*) = 0;
	virtual int  getSelectedTextureIndex() = 0;
	virtual void render() = 0;
	virtual char* inputWindow(char*, char*) = 0;
	virtual void messageWindow(char*) = 0;
	virtual cRSMModel* loadModel(char*) = 0;
	virtual void removeModel(int) = 0;
	virtual void removeSprite(int) = 0;
};


#endif
