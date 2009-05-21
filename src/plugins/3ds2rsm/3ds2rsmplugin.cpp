#include <windows.h>
#include "3ds2rsmplugin.h"
#include "../base/interface.h"
#include "3ds.h"

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <stdio.h>
#include <fstream>

class cRSMModelFace
{
public:
				unsigned short v[3];
				unsigned short t[3];
				unsigned short texid;
				unsigned short todo;
				unsigned int todo2;
				unsigned int nsurf;
};



c3ds2rsmPluginPlugin::c3ds2rsmPluginPlugin() : cPluginBase("3DS to RSM", "tools/CONVERT")
{
	
}

bool c3ds2rsmPluginPlugin::action()
{
#ifdef WIN32
#if 1==0
 //FLATSHADING
	unsigned char rawData[25] =
	{
		0x80, 0x3E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	} ;
#else
	unsigned char rawData[25] =
	{
		0x80, 0x3E, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	} ;
#endif
	char zeroes[40];
	ZeroMemory(zeroes,40);
	int i;
	int one = 1;
	int zero = 0;
	float fOne = 1;
	float fZero = 0;
	

	std::string loadFileName = browInterface->getOpenFile("","All\0*.*\03ds Models\0*.3ds\0");

	if(loadFileName == "")
		return false;

	CLoad3DS g_Load3ds;
	t3DModel g_3DModel;

	g_Load3ds.Import3DS(&g_3DModel, (char*)loadFileName.c_str());
	printf("Loaded 3ds model, writing rsm");


	std::string saveFilename = browInterface->getSaveFile((loadFileName + ".rsm").c_str(), "All\0*.*\0RO Models\0*.rsm\0");
	if(saveFilename == "")
		return false;


	ofstream pFile(saveFilename.c_str(), ios_base::out | ios_base::binary);
	pFile.write("GRSM\1\4", 6);
	pFile.write((char*)rawData,25);
	pFile.write((char*)&g_3DModel.numOfMaterials,4);
	for(i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		for(int ii = strlen(g_3DModel.pMaterials[i].strFile); ii > 0; ii--)
		{
			if(g_3DModel.pMaterials[i].strFile[ii] == '.')
			{
				g_3DModel.pMaterials[i].strFile[ii] = 0;
				break;
			}
		}
		strcat(g_3DModel.pMaterials[i].strFile, ".bmp");
		int len = strlen(g_3DModel.pMaterials[i].strFile);
		pFile.write(g_3DModel.pMaterials[i].strFile, len);
		pFile.write(zeroes, 40-len);
	}

	//meshes
	pFile.write("borf",4);	pFile.write(zeroes, 36);	//mesh name
	pFile.write((char*)&one,4);							//todo
	pFile.write(zeroes, 40);							//parent name
	pFile.write(zeroes,40);	//10 floats, all 0

	pFile.write((char*)&g_3DModel.numOfMaterials,4);
	for(i = 0; i < g_3DModel.numOfMaterials; i++)
		pFile.write((char*)&i, 4);

	pFile.write((char*)&fOne, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);										//0,1,2
	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fOne, 4);	pFile.write((char*)&fZero, 4);										//3,4,5
	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fOne, 4);										//6,7,8

	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);										//9,10,11		translate
	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);										//12,13,14		translate
	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);	pFile.write((char*)&fZero, 4);		//15,16,17,18	rotate
	pFile.write((char*)&fOne, 4);	pFile.write((char*)&fOne, 4);	pFile.write((char*)&fOne, 4);										//19,20,21		scale


	vector<int> sumVertices;
	vector<int> sumTexVertices;
	int total = 0;
	for(i = 0; i < g_3DModel.numOfObjects; i++)
		total += g_3DModel.pObject[i].numOfVerts;
	pFile.write((char*)&total, 4);

	if(total > (long)((unsigned short)-1))
	{
		browInterface->Log(1,"Error: too many vertices!");
		return false;
	}

	total = 0;
	for(i = 0; i < g_3DModel.numOfObjects; i++)
	{
		for(int ii = 0; ii < g_3DModel.pObject[i].numOfVerts; ii++)
		{
			g_3DModel.pObject[i].pVerts[ii].x = -g_3DModel.pObject[i].pVerts[ii].x;
			g_3DModel.pObject[i].pVerts[ii].y = -g_3DModel.pObject[i].pVerts[ii].y;
			pFile.write((char*)&g_3DModel.pObject[i].pVerts[ii].x, 4);
			pFile.write((char*)&g_3DModel.pObject[i].pVerts[ii].y, 4);
			pFile.write((char*)&g_3DModel.pObject[i].pVerts[ii].z, 4);
		}
		sumVertices.push_back(total);
		total += g_3DModel.pObject[i].numOfVerts;
	}

	total = 0;
	for(i = 0; i < g_3DModel.numOfObjects; i++)
		total += g_3DModel.pObject[i].numTexVertex;
	pFile.write((char*)&total, 4);
	if(total > (long)((unsigned short)-1))
	{
		browInterface->Log(1,"Error: too many texture vertices!");
		return false;
	}


	total = 0;
	for(i = 0; i < g_3DModel.numOfObjects; i++)
	{
		for(int ii = 0; ii < g_3DModel.pObject[i].numTexVertex; ii++)
		{
			while(g_3DModel.pObject[i].pTexVerts[ii].x < 0)
			{
				g_3DModel.pObject[i].pTexVerts[ii].x++;
//				notifyOnce("Texture Vertices out of bounds, wrapping coordinats. This will make your model look bad");
			}
			while(g_3DModel.pObject[i].pTexVerts[ii].x > 1)
			{
				g_3DModel.pObject[i].pTexVerts[ii].x--;
//				notifyOnce("Texture Vertices out of bounds, wrapping coordinats. This will make your model look bad");
			}
			while(g_3DModel.pObject[i].pTexVerts[ii].y < 0)
			{
				g_3DModel.pObject[i].pTexVerts[ii].y++;
//				notifyOnce("Texture Vertices out of bounds, wrapping coordinats. This will make your model look bad");
			}
			while(g_3DModel.pObject[i].pTexVerts[ii].y > 1)
			{
				g_3DModel.pObject[i].pTexVerts[ii].y--;
//				notifyOnce("Texture Vertices out of bounds, wrapping coordinats. This will make your model look bad");
			}

			pFile.write((char*)&zero, 4);
			pFile.write((char*)&g_3DModel.pObject[i].pTexVerts[ii].x, 4);
			pFile.write((char*)&g_3DModel.pObject[i].pTexVerts[ii].y, 4);
		}
		sumTexVertices.push_back(total);
		total += g_3DModel.pObject[i].numTexVertex;
	}



	total = 0;
	for(i = 0; i < g_3DModel.numOfObjects; i++)
		total += g_3DModel.pObject[i].numOfFaces;
	pFile.write((char*)&total, 4);
	if(total > (long)((unsigned short)-1))
	{
		browInterface->Log(1,"Error: too many faces!");
		return false;
	}
	total = 0;

	int index = 0;

	for(i = 0; i < g_3DModel.numOfObjects; i++)
	{
		for(int ii = 0; ii < g_3DModel.pObject[i].numOfFaces; ii++)
		{
			cRSMModelFace f;
			f.v[0] = g_3DModel.pObject[i].pFaces[ii].vertIndex[0] + sumVertices[i];
			f.v[1] = g_3DModel.pObject[i].pFaces[ii].vertIndex[1] + sumVertices[i];
			f.v[2] = g_3DModel.pObject[i].pFaces[ii].vertIndex[2] + sumVertices[i];

			f.t[0] = g_3DModel.pObject[i].pFaces[ii].vertIndex[0] + sumTexVertices[i];
			f.t[1] = g_3DModel.pObject[i].pFaces[ii].vertIndex[1] + sumTexVertices[i];
			f.t[2] = g_3DModel.pObject[i].pFaces[ii].vertIndex[2] + sumTexVertices[i];

			f.texid = g_3DModel.pObject[i].materialID;
			if(g_3DModel.pObject[i].materialID == -1)
				f.texid = 0;
			f.todo = 0;
			f.todo2 = 0;
			f.nsurf = 0;

			pFile.write((char*)&f.v, sizeof(f.v));
			pFile.write((char*)&f.t, sizeof(f.t));
			pFile.write((char*)&f.texid, sizeof(f.texid));
			pFile.write((char*)&f.todo, sizeof(f.todo));
			pFile.write((char*)&f.todo2, sizeof(f.todo2));
			pFile.write((char*)&f.nsurf, sizeof(f.nsurf));
			index++;
		}
	}

	pFile.write((char*)&zero, 4);
	pFile.write((char*)&zero, 4);
	pFile.write((char*)&zero, 4);


	pFile.close();
	
#endif
	
	
	
	return true;
}
