#include "sprite.h"
#include "graphics.h"
#include "filesystem.h"
extern cFileSystem fs;
extern cGraphics Graphics;


#define SPRITESIZE 128


cSprite::cSprite()
{
	body = NULL;
	head = NULL;
}


void cSprite::loadbody(string filename)
{
	if(body != NULL)
		delete body;
	body = new cActSpr();
	body->load(filename);
}

void cSprite::loadhead(string filename)
{
	if(head != NULL)
		delete head;
	head = new cActSpr();
	head->load(filename);
}

void cSprite::addextra(string filename)
{
	cActSpr* spr = new cActSpr();
	spr->load(filename);
	extras.push_back(spr);

}


void cSprite::cActSpr::load(string filename)
{
	int i,ii,x,y;
	cFile* pFile = fs.open(filename + ".spr");
	if(pFile == NULL)
	{
		Log(1,0,"Error opening %s.spr", filename.c_str());
		return;
	}
	pFile->get(); pFile->get();
	BYTE version1 = pFile->get();
	BYTE version2 = pFile->get();
	BYTE framecount = pFile->get();
	pFile->get();
	BYTE framecount32 = pFile->get();
	pFile->get();

	for(i = 0; i < framecount; i++)
	{
		int width = ((BYTE)pFile->get()) | (((BYTE)pFile->get())<<8);
		int height = ((BYTE)pFile->get()) | (((BYTE)pFile->get())<<8);
		int framelen = width*height;
		if(version1 != 0)
			framelen = ((BYTE)pFile->get()) | (((BYTE)pFile->get())<<8);
		
		BYTE* data = new BYTE[width*height];
		int iii = 0;
		for(ii = 0; ii < framelen && iii < width*height; ii++)
		{
			BYTE c = pFile->get();
			data[iii] = c;			iii++;
			if(iii >= width*height)
				break;
			if(c == 0 && version1 != 0)
			{
				BYTE d = pFile->get();
				if(d == 0)
				{
					data[iii] = d; iii++;
					if(iii >= width*height)
						break;
				}
				else
				{
					for(int iiii = 1; iiii < d; iiii++)
					{
						data[iii] = c; iii++;
						if(iii >= width*height)
							break;
					}
				}
			}
		}

		
		char* image = new char[SPRITESIZE*SPRITESIZE*4];

		for(y = 0; y < SPRITESIZE; y++)
		{
			for(x = 0; x < SPRITESIZE; x++)
			{
				int xx = x * (width / (float)SPRITESIZE);
				int yy = y * (height / (float)SPRITESIZE);

				BYTE index = data[xx+width*yy];
				
				image[4*(x+SPRITESIZE*y)] = pFile->data[pFile->size-1024 + index*4];
				image[4*(x+SPRITESIZE*y)+1] = pFile->data[pFile->size-1024 + index*4+1];
				image[4*(x+SPRITESIZE*y)+2] = pFile->data[pFile->size-1024 + index*4+2];
				image[4*(x+SPRITESIZE*y)+3] = index==0 ? 0 : 255;
			}
		}
		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D,0,4,SPRITESIZE,SPRITESIZE,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		delete[] image;
		delete[] data;

		cFrame f;
		f.tex = tid;
		f.h = height;
		f.w = width;
		frames.push_back(f);

	}

	pFile->close();
// done reading sprite
	pFile = fs.open(filename + ".act");
	if(pFile == NULL)
	{
		Log(1,0,"Error opening %s.act", filename.c_str());
		return;
	}
	pFile->get(); pFile->get();

	version1 = pFile->get();
	pFile->get();
	int actioncount = ((BYTE)pFile->get()) | (((BYTE)pFile->get())<<8);
	char buf[100];
	pFile->read(buf, 10);

	for(i = 0; i < actioncount; i++)
	{
		cAction* action = new cAction();
		
		pFile->read((char*)&action->framecount,4);
		for(ii = 0; ii < action->framecount; ii++)
		{
			pFile->read(buf, 32);
			cAction::cFrame* frame = new cAction::cFrame();
			pFile->read((char*)&frame->subframecount, 4);
			for(int iii = 0; iii < frame->subframecount; iii++)
			{
				cAction::cFrame::cSubFrame* subframe = new cAction::cFrame::cSubFrame();
				pFile->read((char*)&subframe->offsetx,4);
				pFile->read((char*)&subframe->offsety,4);
				pFile->read((char*)&subframe->image,4);
				pFile->read((char*)&subframe->direction,4);
				pFile->read((char*)&subframe->color,4);
				if(version1 >= 2)
					pFile->read((char*)&subframe->scalex,4);
				if(version1 >= 4)
					pFile->read((char*)&subframe->scaley,4);
				else
					subframe->scaley = subframe->scalex;
				if(version1 >= 2)
				{
					pFile->read(buf, 4);
					pFile->read((char*)&subframe->dontjump,4);
					if(subframe->dontjump != 0)
						pFile->read(buf, 12);
					if(version1 >= 5)
					{
						pFile->read((char*)&subframe->sizex,4);
						pFile->read((char*)&subframe->sizey,4);
					}
				}

				frame->subframes.push_back(subframe);
			}

			pFile->read(buf,4);
			pFile->read((char*)&frame->extrainfo,4);
			if(frame->extrainfo != 0)
			{
				pFile->read((char*)&frame->extrarotation,4);
				pFile->read((char*)&frame->extrax,4);
				pFile->read((char*)&frame->extray,4);

			}
			pFile->read(buf,4);
			action->frames.push_back(frame);

		}
		actions.push_back(action);
	}



	pFile->close();



}


void cSprite::draw()
{
	glPushMatrix();
	glTranslatef(5*pos.x, pos.y, 5*pos.z);
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			modelview[i*4+j] = ((i == j) ? 1.0 : 0.0);
	glLoadMatrixf(modelview);
	glScalef(0.2f, 0.2f,1);





	int id = ((int)mousex/10) % body->actions.size();


	glEnable(GL_BLEND);
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);

	
	cActSpr::cAction::cFrame* bodyframe = body->actions[id]->frames[0];
	cActSpr::cAction::cFrame::cSubFrame* subframe = bodyframe->subframes[0];
	int frame = subframe->image;
	int direction = subframe->direction;
	
	glBindTexture(GL_TEXTURE_2D, body->frames[frame].tex);
	float width = body->frames[frame].w;
	float height = body->frames[frame].h;
	float bodyheight = height;

	width/=2;
	height/=2;

	glDisable(GL_DEPTH_TEST);
	glTranslatef(-subframe->offsetx, -subframe->offsety, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(direction,0);		glVertex2f(width,height);
		glTexCoord2f(1-direction,0);	glVertex2f(-width,height);
		glTexCoord2f(1-direction,1);	glVertex2f(-width,-height);
		glTexCoord2f(direction,1);		glVertex2f(width,-height);
	glEnd();
	glTranslatef(subframe->offsetx, subframe->offsety, 0);
	glEnable(GL_DEPTH_TEST);










	subframe = head->actions[id]->frames[0]->subframes[1];
	cActSpr::cAction::cFrame* myframe = head->actions[id]->frames[0];
	frame = subframe->image;
	//frame = 1;
	direction = subframe->direction;
	
	glBindTexture(GL_TEXTURE_2D, head->frames[frame].tex);
	width = head->frames[frame].w;
	height = head->frames[frame].h;

	width/=2;
	height/=2;

	glDisable(GL_DEPTH_TEST);
	glTranslatef(-(subframe->offsetx + bodyframe->extrax - myframe->extrax), -(subframe->offsety + bodyframe->extray - myframe->extray), 0);
	glBegin(GL_QUADS);
		glTexCoord2f(direction,0);		glVertex2f(width,height);
		glTexCoord2f(1-direction,0);	glVertex2f(-width,height);
		glTexCoord2f(1-direction,1);	glVertex2f(-width,-height);
		glTexCoord2f(direction,1);		glVertex2f(width,-height);
	glEnd();
	glEnable(GL_DEPTH_TEST);

	
	glPopMatrix();
}