#include "texturemodel.h"
#include "texturecache.h"

int cTextureModel::open(string filename)
{
	tex = TextureCache.load(filename);
	return 0;
}

int cTextureModel::close()
{
	TextureCache.unload(tex);
	return 0;
}

int cTextureModel::draw(float x, float y, float z, float scale)
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode
	glBindTexture(GL_TEXTURE_2D, tex->texid());

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glTexCoord2f(1,0);	glVertex3f(boundingbox1.x, boundingbox1.y, 0);
		glTexCoord2f(0,0);	glVertex3f(boundingbox2.x, boundingbox1.y, 0);
		glTexCoord2f(0,1);	glVertex3f(boundingbox2.x, boundingbox2.y, 0);
		glTexCoord2f(1,1);	glVertex3f(boundingbox1.x, boundingbox2.y, 0);

		glTexCoord2f(1,1);	glVertex3f(boundingbox1.x, boundingbox2.y, 0);
		glTexCoord2f(0,1);	glVertex3f(boundingbox2.x, boundingbox2.y, 0);
		glTexCoord2f(0,0);	glVertex3f(boundingbox2.x, boundingbox1.y, 0);
		glTexCoord2f(1,0);	glVertex3f(boundingbox1.x, boundingbox1.y, 0);


		glTexCoord2f(1,0);	glVertex3f(boundingbox1.x, 0, boundingbox1.z);
		glTexCoord2f(0,0);	glVertex3f(boundingbox2.x, 0, boundingbox1.z);
		glTexCoord2f(0,1);	glVertex3f(boundingbox2.x, 0, boundingbox2.z);
		glTexCoord2f(1,1);	glVertex3f(boundingbox1.x, 0, boundingbox2.z);

		glTexCoord2f(1,1);	glVertex3f(boundingbox1.x, 0, boundingbox2.z);
		glTexCoord2f(0,1);	glVertex3f(boundingbox2.x, 0, boundingbox2.z);
		glTexCoord2f(0,0);	glVertex3f(boundingbox2.x, 0, boundingbox1.z);
		glTexCoord2f(1,0);	glVertex3f(boundingbox1.x, 0, boundingbox1.z);

		
		glTexCoord2f(1,1);	glVertex3f(0,boundingbox1.y, boundingbox2.z);
		glTexCoord2f(0,1);	glVertex3f(0,boundingbox2.y, boundingbox2.z);
		glTexCoord2f(0,0);	glVertex3f(0,boundingbox2.y, boundingbox1.z);
		glTexCoord2f(1,0);	glVertex3f(0,boundingbox1.y, boundingbox1.z);

		glTexCoord2f(1,1);	glVertex3f(0,boundingbox1.y, boundingbox1.z);
		glTexCoord2f(0,1);	glVertex3f(0,boundingbox2.y, boundingbox1.z);
		glTexCoord2f(0,0);	glVertex3f(0,boundingbox2.y, boundingbox2.z);
		glTexCoord2f(1,0);	glVertex3f(0,boundingbox1.y, boundingbox2.z);

	glEnd();// vertex pos aanpassen

	glDisable(GL_TEXTURE_2D);
	return 0;
}

int cTextureModel::setstate(int state)
{

	return 0;
}

cTextureModel::cTextureModel()
{
	boundingbox1 = cVector3(-5,-5,-5);
	boundingbox2 = cVector3(5,5,5);
}

cTextureModel::~cTextureModel()
{

}
