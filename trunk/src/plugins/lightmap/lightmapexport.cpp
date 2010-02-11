#include <windows.h>
#include "lightmapexport.h"
#include "../base/interface.h"
#include <gd/gd.h>
#include <fstream>


cLightMapExportPlugin::cLightMapExportPlugin() : cPluginBase("Export Lightmaps", "tools/lightmap/EXPORT")
{
}

bool cLightMapExportPlugin::action()
{
	bool exportWalls = browInterface->confirmWindow("Would you like to include walls?");

	std::string fileName = browInterface->getMapFile();
	fileName += ".lightmap1.png";
	const char* pFileName = browInterface->inputWindow("Please enter filename to export the addictive lightmap to (shadows)", fileName.c_str());
	fileName = pFileName;
	delete[] (char*)pFileName;
	if(fileName != "")
	{
		gdImagePtr img = gdImageCreateTrueColor(browInterface->getWorldWidth()*(exportWalls?12:6), browInterface->getWorldHeight()*(exportWalls?12:6));
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
								int intensity = (unsigned char)lightMap->buf[1+xx+8*yy+8];
								gdImageSetPixel(img, (exportWalls?12:6)*x+xx, (exportWalls?12:6)*y+yy, gdImageColorAllocate(img, intensity, intensity, intensity));
							}
						}
					}
				}
				if(c->tileSide != -1 && exportWalls)
				{
					cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileSide);
					if(tile->lightmap != -1)
					{
						cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
						
						for(int xx = 0; xx < 6; xx++)
						{
							for(int yy = 0; yy < 6; yy++)
							{
								int intensity = (unsigned char)lightMap->buf[1+xx+8*yy+8];
								gdImageSetPixel(img, 12*x+xx+6, 12*y+yy, gdImageColorAllocate(img, intensity, intensity, intensity));
							}
						}
					}
				}
				if(c->tileOtherSide != -1 && exportWalls)
				{
					cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileOtherSide);
					if(tile->lightmap != -1)
					{
						cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
						
						for(int xx = 0; xx < 6; xx++)
						{
							for(int yy = 0; yy < 6; yy++)
							{
								int intensity = (unsigned char)lightMap->buf[1+xx+8*yy+8];
								gdImageSetPixel(img, 12*x+xx, 12*y+yy+6, gdImageColorAllocate(img, intensity, intensity, intensity));
							}
						}
					}
				}
			}
		}
		int size = 0;
		void* data = gdImagePngPtr(img, &size);
		std::ofstream pFile(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
		pFile.write((char*)data, size);
		pFile.close();
		gdFree(data);
		gdImageDestroy(img);
	}



	fileName = browInterface->getMapFile();
	fileName += ".lightmap2.png";
	pFileName = browInterface->inputWindow("Please enter filename to export the multiplicative lightmap to (colours)", fileName.c_str());
	fileName = pFileName;
	delete[] (char*)pFileName;
	if(fileName != "")
	{
		gdImagePtr img = gdImageCreateTrueColor(browInterface->getWorldWidth()*(exportWalls?12:6), browInterface->getWorldHeight()*(exportWalls?12:6));
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
								int r = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3];
								int g = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+1];
								int b = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+2];
								gdImageSetPixel(img, (exportWalls?12:6)*x+xx, (exportWalls?12:6)*y+yy, gdImageColorAllocate(img, r,g,b));
							}
						}
					}
				}
				if(c->tileSide != -1 && exportWalls)
				{
					cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileSide);
					if(tile->lightmap != -1)
					{
						cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
						
						for(int xx = 0; xx < 6; xx++)
						{
							for(int yy = 0; yy < 6; yy++)
							{
								int r = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3];
								int g = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+1];
								int b = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+2];
								gdImageSetPixel(img, 12*x+xx+6, 12*y+yy, gdImageColorAllocate(img, r,g,b));
							}
						}
					}
				}
				if(c->tileOtherSide != -1 && exportWalls)
				{
					cBrowInterface::cPluginTile* tile = browInterface->getTile(c->tileOtherSide);
					if(tile->lightmap != -1)
					{
						cBrowInterface::cPluginLightmap* lightMap = browInterface->getLightmap(tile->lightmap);
						
						for(int xx = 0; xx < 6; xx++)
						{
							for(int yy = 0; yy < 6; yy++)
							{
								int r = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3];
								int g = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+1];
								int b = (unsigned char)lightMap->buf[64+(1+xx+8*yy+8)*3+2];
								gdImageSetPixel(img, 12*x+xx, 12*y+yy+6, gdImageColorAllocate(img, r,g,b));
							}
						}
					}
				}
			
			}
		}
		int size = 0;
		void* data = gdImagePngPtr(img, &size);
		std::ofstream pFile(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
		pFile.write((char*)data, size);
		pFile.close();
		gdFree(data);
		gdImageDestroy(img);
	}
		
	return true;
}
