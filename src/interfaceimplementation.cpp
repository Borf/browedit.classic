#include "interfaceimplementation.h"
#include <graphics.h>
#include <windows/xmlwindow.h>
#include <SDL/SDL.h>
#include <settings.h>

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

int cBrowInterfaceImplementation::getSelectedTextureIndex()
{
	return cGraphics::worldContainer->settings.texturestart + ((int)cGraphics::worldContainer->settings.selectionstart.y - 32) / 288;	
}


void cBrowInterfaceImplementation::render()
{
	cGraphics::draw();
	SDL_GL_SwapBuffers();
}

std::string cBrowInterfaceImplementation::inputWindow( std::string msg, std::string defaultText)
{
	return cWM::inputWindow(msg, defaultText);
}

void cBrowInterfaceImplementation::messageWindow( std::string msg)
{
	cWM::showMessage(msg);
}

cRSMModel* cBrowInterfaceImplementation::loadModel( std::string )
{
	cRSMModel* model = new cRSMModel();
	model->load(cSettings::roDir +  "data\\model\\���μ�ǰ\\ö�ٸ�.rsm");
	return model;	
}

void cBrowInterfaceImplementation::removeModel(int id)
{
	delete cGraphics::world->models[id];
	cGraphics::world->models.erase(cGraphics::world->models.begin() + id);
}