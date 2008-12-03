#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <string>
class cWindow;
class cRSMModel;

class cBrowInterface
{
public:
	virtual void fixNormals() = 0;
	virtual cWindow* addXmlWindow(std::string) = 0;
	virtual int  getSelectedTextureIndex() = 0;
	virtual void render() = 0;
	virtual std::string inputWindow(std::string, std::string) = 0;
	virtual void messageWindow(std::string) = 0;
	virtual cRSMModel* loadModel(std::string) = 0;
	virtual void removeModel(int) = 0;
};


#endif
