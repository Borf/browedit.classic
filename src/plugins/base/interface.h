#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class cWindow;
class cRSMModel;

class cBrowInterface
{
public:
	virtual void fixNormals() = 0;
	virtual cWindow* addXmlWindow(const char*) = 0;
	virtual int  getSelectedTextureIndex() = 0;
	virtual void render() = 0;
	virtual const char* inputWindow(const char*, const char*) = 0;
	virtual void messageWindow(const char*) = 0;
	virtual cRSMModel* loadModel(const char*) = 0;
	virtual void removeModel(int) = 0;
	virtual void removeSprite(int) = 0;
};


#endif
