#include "texture.h"

#ifdef __MINGW32__
#include <GL/glext.h>											// We use a define from this file: GL_BGRA_EXT
#endif

extern cFileSystem fs;

cTexture::cTexture(string pFilename, bool pFreedata)
{
	filename = pFilename;
	loaded = false;
	freedata = pFreedata;
	data = NULL;
	datatype = 0;
	tid = 0;
}

GLuint cTexture::texid()
{
	if(!loaded && filename != "")
	{
		cTextureLoaders::load(filename, this);
		loaded = true;
	}
	return tid;
}


void cTexture::unLoad()
{
	if (!freedata)
		delete[] data;
	if(tid != 0)
		glDeleteTextures(1, &tid);
	loaded = false;
}

cTextureLoaders& GetTextureLoaders()
{
	static cTextureLoaders* l = new cTextureLoaders();
	return *l;
}

//vector<cTextureLoader*> cTextureLoaders::loaders;
cTexture* cTextureLoaders::load(string filename, bool freedata)
{
	cTexture* t = new cTexture(filename, freedata);
	return t;
}

void cTexture::generate()
{
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	if(datatype == 0)
		datatype = GL_RGB;


	glTexImage2D(GL_TEXTURE_2D,0,bpp/8,width,height,0,datatype,GL_UNSIGNED_BYTE,data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);



	if(freedata)
		delete[] data;

}


void cTextureLoaders::load(string filename, cTexture* tex)
{
	if(filename.rfind(".") == string::npos)
	{
		Log(1,0,"Error loading texture %s, not able to determine filetype!", filename.c_str());
		return;
	}

	vector<cTextureLoader*> loaders = GetTextureLoaders().loaders;

	string ext = lcase(filename.substr(filename.rfind(".")));
	for(unsigned int i = 0; i < loaders.size(); i++)
	{
		for(unsigned int ii = 0; ii < loaders[i]->extensions.size(); ii++)
		{
			if(loaders[i]->extensions[ii] == ext)
			{
				cFile* pFile = fs.open(filename);
				loaders[i]->load(pFile->data, pFile->size, tex);
				if(tex->data == NULL)
					Log(2,0,"Error loading texture %s!", filename.c_str());
				else
				{
					pFile->close();
					tex->resizeToLog();
					tex->generate();
				}
				return;
			}
		}
	}
	Log(1,0,"Unknown texture type: %s at file %s", ext.c_str(), filename.c_str()); 
}




void cTexture::resizeToLog()
{
	// Resize Image To Closest Power Of Two
	GLint glMaxTexDim;
	
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
	if (width <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		width = 1 << (int)floor((log((double)width)/log(2.0f)) + 0.5f); 
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		width = glMaxTexDim;
	
	if (height <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		height = 1 << (int)floor((log((double)height)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		height = glMaxTexDim;

	while(height < heightOriginal)
		height*=2;
	while(width< widthOriginal)
		width*=2;

	if(width == widthOriginal && height == heightOriginal)
		return;

	float factorx = (float)widthOriginal / (float)width;
	float factory = (float)heightOriginal / (float)height;

	char* data2 = new char[width* height*bpp];
	ZeroMemory(data2, width*height*bpp);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			int tx = (int)floor(x*factorx);
			int ty = (int)floor(y*factory);
			int target = 4*(tx+(widthOriginal)*ty);
  			data2[4*(x+width*y)] = data[target];
			data2[4*(x+width*y)+1] = data[target+1];
			data2[4*(x+width*y)+2] = data[target+2];
			data2[4*(x+width*y)+3] = data[target+3];
		}
	}
	delete[] data;

	data = (BYTE*)data2;

}
