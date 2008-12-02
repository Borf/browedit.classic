#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <string>
class cWindow;


class cBrowInterface
{
public:
	virtual void fixNormals() = 0 { return; };
	virtual cWindow* addXmlWindow(std::string) = 0 { return NULL; };
};


#endif
