#include "bitmaptextureloader.h"
cBitmapTextureLoader	BitmapTextureLoader;


cBitmapTextureLoader::cBitmapTextureLoader()
{
	extensions.push_back(".bmp");
	cTextureLoaders::loaders.push_back(this);
}

void cBitmapTextureLoader::load(char* filedata,int datlen, cTexture* tex)
{
	char header[54];
	memcpy(header, filedata, 54);			//read the 54bit main header
	if(header[0]!='B' || header[1]!='M') 
	{
		Log(1,0,"BMP file %s is not a bmp file", tex->getfilename().c_str());
	}
	int offset=*(unsigned int*)(header+10);
	
	tex->widthOriginal = tex->width = *(int*)(header+18);
	tex->heightOriginal = tex->height = *(int*)(header+22);
	tex->bpp = int(header[28]);

	tex->data = new BYTE[tex->width*tex->height*4];
	tex->datatype = GL_BGRA_EXT;

	int x,y,c,i;
	int index = 0;
	BYTE cols[256*4];				//colourtable
	switch(tex->bpp)
	{
	case(24):
		index = offset;
		i = 0;
		for(y=0;y<tex->height;y++)
		{
			for(x = 0; x < tex->width; x++)
			{
				memcpy(tex->data+i, filedata+index, 3);

				if(tex->data[i] == 255 && tex->data[i+1] == 0 && tex->data[i+2] == 255)
					tex->data[i+3] = 0;
				else
					tex->data[i+3] = 255;
				i+=4;
			}
			int bytesleft = (tex->width*3)%4;
			for(int ii = 0; ii < (4-bytesleft)%4; ii++)
				index++;
		}
		break;

	case(8):
		memcpy(cols, filedata+54, 256*4);							//read colortable
		index = offset;
		for(y=0;y<tex->height;++y)						//(Notice 4bytes/col for some reason)
		{
			for(x=0;x<tex->width;++x)
			{
				BYTE byte = filedata[index];
				index++;
				for(int c=0;c<3;++c)
				{
					tex->data[(y*tex->width+x)*4+c] = cols[byte*4+c];
				}
				
				if(tex->data[(y*tex->width+x)*4] == 255 && tex->data[(y*tex->width+x)*4+1] == 0 && tex->data[(y*tex->width+x)*4+2] == 255)
					tex->data[(y*tex->width+x)*4+3] = 0;
				else
					tex->data[(y*tex->width+x)*4+3] = 255;
			}
			int bytesleft = tex->width%4;
			for(int ii = 0; ii < (4-bytesleft)%4; ii++)
				index++;
		}

		break;

/*	case(4):
		f->read((char*)cols,16*4);
		f->seek(offset,beg);
		for(y=0;y<256;++y)
			for(x=0;x<256;x+=2)
			{
				BYTE byte = f->get();
				for(c=0;c<3;++c)						//pixels from each byte
					bmp.pixel(x,y,c)=cols[byte/16*4+2-c];
				for(c=0;c<3;++c)
					bmp.pixel(x+1,y,c)=cols[byte%16*4+2-c];
				bmp.pixel(x+1,y,3) = 255;
			}
		break;

	case(1):
		f->read((char*)cols,8);
		f->seek(offset,beg);
		for(y=0;y<bmp.height;++y)
			for(x=0;x<bmp.width;x+=8)
			{
				BYTE byte = f->get();
				//Every byte is eight pixels
				//so I'm shifting the byte to the relevant position, then masking out
				//all but the lowest bit in order to get the index into the colourtable.
				for(int x2=0;x2<8;++x2)
				{
					for(int c=0;c<3;++c)
						bmp.pixel(x+x2,y,c)=cols[((byte>>(7-x2))&1)*4+2-c];
					bmp.pixel(x+x2,y,3)=255;
				}
			}
		break;
*/
	default:
		Log(3,0,"Unsupported bitdepth in bmp file: %s:%i", tex->getfilename().c_str(), tex->bpp);
		return;
	}


	tex->bpp = 32;

}