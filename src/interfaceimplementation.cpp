#include "interfaceimplementation.h"
#include <graphics.h>
#include <windows/xmlwindow.h>

void cBrowInterfaceImplementation::fixNormals()
{
	cGraphics::world->calcVertexNormals();
}

cWindow* cBrowInterfaceImplementation::addXmlWindow( std::string fileName)
{
	TiXmlDocument windowLayout = cFileSystem::getXml(fileName);
	cXmlWindow* w = new cXmlWindow(windowLayout);
	cWM::addWindow(w);
	return w;
}

