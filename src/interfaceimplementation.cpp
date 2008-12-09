#include "interfaceimplementation.h"
#include <graphics.h>
#include <windows/xmlwindow.h>
#include <SDL/SDL.h>
#include <settings.h>

void cBrowInterfaceImplementation::fixNormals()
{
	cGraphics::world->calcVertexNormals();
}

cWindow* cBrowInterfaceImplementation::addXmlWindow( char* fileName)
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

char* cBrowInterfaceImplementation::inputWindow( char* msg, char* defaultText)
{
	return (char*)cWM::inputWindow(msg, defaultText).c_str();
}

void cBrowInterfaceImplementation::messageWindow( char* msg)
{
	cWM::showMessage(msg);
}

cRSMModel* cBrowInterfaceImplementation::loadModel( char* )
{
	cRSMModel* model = new cRSMModel();
	model->load(cSettings::roDir +  "data\\model\\郴何家前\\枚促府.rsm");
	return model;	
}

void cBrowInterfaceImplementation::removeModel(int id)
{
	delete cGraphics::world->models[id];
	cGraphics::world->models.erase(cGraphics::world->models.begin() + id);
}

void cBrowInterfaceImplementation::removeSprite(int id)
{
	delete cGraphics::world->sprites[id];
	cGraphics::world->sprites.erase(cGraphics::world->sprites.begin() + id);
}
