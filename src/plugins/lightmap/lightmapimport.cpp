#include <windows.h>
#include "lightmapimport.h"
#include "../base/interface.h"
#include <fstream>
#include <gd/gd.h>

cLightMapImportPlugin::cLightMapImportPlugin() : cPluginBase("Import Lightmaps", "tools/lightmap/IMPORT")
{
	
}

bool cLightMapImportPlugin::action()
{
	std::string fileName = browInterface->getMapFile();
	fileName += ".lightmap1.png";
	const char* pFileName = browInterface->inputWindow("Please enter filename to export the addictive lightmap to (shadows)", fileName.c_str());
	fileName = pFileName;
	delete[] (char*)pFileName;
	if(fileName != "")
	{
		char* data;
		unsigned int size;
		std::ifstream pFile(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
		if(!pFile.bad() && !pFile.eof() && pFile.good())
		{
			pFile.seekg(0, std::ios_base::end);
			size = (unsigned int)pFile.tellg();
			pFile.seekg(0, std::ios_base::beg);
			data = new char[size+1];
			pFile.read(data, size);
			gdImagePtr img = gdImageCreateFromPngPtr(size, data);
			delete[] data;

			if(img)
			{
				if((img->sy == 6*browInterface->getWorldHeight() || img->sy == 12*browInterface->getWorldHeight()) && (img->sx == 6*browInterface->getWorldWidth() || img->sx == 12*browInterface->getWorldWidth()))
				{
					bool importWalls = img->sy == 12*browInterface->getWorldHeight();
					browInterface->makeTilesUnique();
					browInterface->makeLightmapsUnique();

					for(int x = 0; x < browInterface->getWorldWidth(); x++)
					{
						for(int y = 0; y < browInterface->getWorldHeight(); y++)
						{
							cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
							if(c->tileUp != -1)
							{
								cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileUp);
								if(tile->lightmap != -1)
								{
									cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
									
									for(int xx = 0; xx < 6; xx++)
									{
										for(int yy = 0; yy < 6; yy++)
										{
											lightMap->buf[1+xx+8*yy+8] = gdTrueColorGetBlue(gdImageGetPixel(img, (importWalls?12:6)*x+xx, (importWalls?12:6)*y+yy));
										}
									}
								}
							}
							if(c->tileSide != -1 && importWalls)
							{
								cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileSide);
								if(tile->lightmap != -1)
								{
									cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
									
									for(int xx = 0; xx < 6; xx++)
									{
										for(int yy = 0; yy < 6; yy++)
										{
											lightMap->buf[1+xx+8*yy+8] = gdTrueColorGetBlue(gdImageGetPixel(img, (importWalls?12:6)*x+xx+6, (importWalls?12:6)*y+yy));
										}
									}
								}
							}
							if(c->tileOtherSide != -1 && importWalls)
							{
								cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileOtherSide);
								if(tile->lightmap != -1)
								{
									cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
									
									for(int xx = 0; xx < 6; xx++)
									{
										for(int yy = 0; yy < 6; yy++)
										{
											lightMap->buf[1+xx+8*yy+8] = gdTrueColorGetBlue(gdImageGetPixel(img, (importWalls?12:6)*x+xx, (importWalls?12:6)*y+yy)+6);
										}
									}
								}
							}
						}
					}


					browInterface->fixGridding();
				}
				else
					browInterface->messageWindow("This image is too large");
			}
			else
				browInterface->messageWindow("This is not a valid imagefile");
		}
		else
			browInterface->messageWindow("This is not a valid file");
	}	
		
	return true;
}
