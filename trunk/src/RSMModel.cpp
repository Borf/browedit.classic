#include <bengine/forwards.h>
#include <bengine/util.h>
#include <bengine/math/math.h>
#include <bengine/texture.h>
#include <bengine/texturecache.h>

#include <common.h>
#include "RSMModel.h"
#include <settings.h>
#include <graphics.h>
#include <world.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <map>

extern int tmpVersion;

cRsmModelBase::cRsmModelBase( std::string pFilename)
{
	root = NULL;	
	bEngine::util::cInStream* pFile = bEngine::util::cFileSystem::open(pFilename);
	*pFile>>this;
	delete pFile;
}


bEngine::util::cOutStream &cRsmModelBase::writeData(bEngine::util::cOutStream &outstream)
{
	return outstream;
}

bEngine::util::cInStream &cRsmModelBase::readData(bEngine::util::cInStream &instream)
{
	char header[4];
	int animLen = 0;
	int nTextures = 0;
	std::string mainNodeName = "";
	int numMeshes = 0;
	char buf[40];
	std::map<std::string, cMesh*, std::less<std::string> > meshes;

	int i;
	
	instream.read(header, 4);
	if(memcmp(header, "GRSM", 4) != 0)
	{
		Log(2,0,"cRsmModel: not a model, invalid header");
		return instream;
	}
	instream>>ver1;
	instream>>ver2;
	instream>>animLen;
	shadeType = (eShadeType)instream.readInt();

	if(ver1 >= 1 && ver2 >= 4)
		instream>>alpha;
	else
		alpha = 0;

	char unknown[16];
	instream.read(unknown, 16);
	instream>>nTextures;

	for(i = 0; i < nTextures; i++)
	{
		char textureName[40];
		instream.read(textureName, 40);
		textures.push_back(bEngine::cTextureCache::load(cSettings::roDir + "data\\texture\\" + textureName));//, (bEngine::eTextureOptions)(bEngine::TEX_NEARESTFILTER)));
	}
	
	instream.read(buf, 40);
	mainNodeName = buf;
	instream>>numMeshes;

	for(i = 0; i < numMeshes; i++)
	{
		cMesh* mesh = new cMesh(this, ver1, ver2);
		instream>>mesh;
		meshes[mesh->name] = mesh;
	}
	//done reading, calculate boundingboxes
	
	if(meshes.find(mainNodeName) == meshes.end())
	{
		Log(2,0,"cRsmModel: Could not locate root mesh");
		root = meshes.begin()->second;
	}
	else
		root = meshes[mainNodeName];
	root->fetchChildren(meshes);



	bbmin = bEngine::math::cVector3(999999, 999999, 999999);
	bbmax = bEngine::math::cVector3(-999999, -999999, -9999999);
	root->setBoundingBox(bbmin, bbmax);
	bbrange = (bbmin + bbmax) / 2.0f;

	
	realbbmax = bEngine::math::cVector3(-999999, -999999, -999999);
	realbbmin = bEngine::math::cVector3(999999, 999999, 999999);
	bEngine::math::cMatrix4x4 mat = bEngine::math::cMatrix4x4::makeScale(1,-1,1);
	root->setBoundingBox2(mat, realbbmin, realbbmax);
	realbbrange = (realbbmax + realbbmin) / 2.0f;
	maxrange = bEngine::math::max(bEngine::math::max(realbbmax.x, -realbbmin.x), bEngine::math::max(bEngine::math::max(realbbmax.y, -realbbmin.y), bEngine::math::max(realbbmax.z, -realbbmin.z)));

	return instream;
}

cRsmModel::cMesh::cMesh(cRsmModelBase* model, int ver1, int ver2)
{
	selected = false;
	cache1 = false;
	cache2 = false;
	base = model;
	lastTick = 0;
	parent = NULL;
	this->model = model;
}

bEngine::util::cInStream &cRsmModel::cMesh::readData(bEngine::util::cInStream &instream)
{
	char buf[40];
	int i;
	int nTextures;

	instream.read(buf, 40);
	name = buf;
	instream.read(buf, 40);
	parentName = buf;
	
	instream>>nTextures;
	for(i = 0; i < nTextures; i++)
		textures.push_back(model->textures[instream.readInt()]);

	instream>>offset.values[0];//rotation
	instream>>offset.values[1];
	instream>>offset.values[2];

	instream>>offset.values[4];
	instream>>offset.values[5];
	instream>>offset.values[6];
	
	instream>>offset.values[8];
	instream>>offset.values[9];
	instream>>offset.values[10];

	instream>>pos_;
	instream>>pos;
	instream>>rotangle;
	instream>>rotaxis;
	instream>>scale;

	instream>>nVertices;
	vertices = new bEngine::math::cVector3[nVertices];
	for(i = 0; i < nVertices; i++)
		instream>>vertices[i];

	instream>>nTexVertices;
	texVertices = new bEngine::math::cVector2[nTexVertices];
	for(i = 0; i < nTexVertices; i++)
	{
		if(model->ver1 >= 1 && model->ver2 >= 2)
			instream.readFloat();
		instream>>texVertices[i];
		texVertices[i].y = 1-texVertices[i].y;
	}
	instream>>nFaces;
	faces = new cFace[nFaces];
	for(i = 0; i < nFaces; i++)
	{
		instream>>faces[i];
		faces[i].normal = (vertices[faces[i].vertices[1]] - vertices[faces[i].vertices[0]]).cross(vertices[faces[i].vertices[2]] - vertices[faces[i].vertices[0]]).getNormalized();
	}
	
	instream>>nAnimationFrames;
	animationFrames = new cFrame[nAnimationFrames];
	for(i = 0; i < nAnimationFrames; i++)
		instream>>animationFrames[i];

	return instream;
}

bEngine::util::cOutStream &cRsmModel::cMesh::writeData(bEngine::util::cOutStream &outstream)
{
	char buf[40];
	int i;
	
	ZeroMemory(buf, 40);
	strcpy(buf, name.c_str());
	outstream.write(buf, 40);

	ZeroMemory(buf, 40);
	strcpy(buf, parentName.c_str());
	outstream.write(buf, 40);

	outstream<<(int)textures.size();
	for(i = 0; i < textures.size(); i++)
		outstream<<0;//TODOtextures.push_back(model->textures[instream.readInt()]);
	
	outstream<<offset.values[0];//rotation
	outstream<<offset.values[1];
	outstream<<offset.values[2];
	
	outstream<<offset.values[4];
	outstream<<offset.values[5];
	outstream<<offset.values[6];
	
	outstream<<offset.values[8];
	outstream<<offset.values[9];
	outstream<<offset.values[10];
	
	outstream<<pos_;
	outstream<<pos;
	outstream<<rotangle;
	outstream<<rotaxis;
	outstream<<scale;
	
	outstream<<nVertices;
	for(i = 0; i < nVertices; i++)
		outstream<<vertices[i];
	
	outstream<<nTexVertices;
	for(i = 0; i < nTexVertices; i++)
	{
		outstream<<0.0f;
		texVertices[i].y = 1-texVertices[i].y;
		outstream<<texVertices[i];
		texVertices[i].y = 1-texVertices[i].y;
	}
	outstream<<nFaces;
	faces = new cFace[nFaces];
	for(i = 0; i < nFaces; i++)
		outstream<<faces[i];
	
	outstream<<nAnimationFrames;
	for(i = 0; i < nAnimationFrames; i++)
		outstream<<animationFrames[i];
	
	return outstream;
}



bEngine::util::cInStream& cRsmModel::readData( bEngine::util::cInStream &instream )
{
	if(tmpVersion >= 0x103)
	{
		name.readData(instream);
		int animType = instream.readInt();
		float animSpeed = instream.readFloat();
		int blockType = instream.readInt();
	}

	filename.readData(instream);

	bEngine::util::cInStream* pFile = bEngine::util::cFileSystem::open(cSettings::roDir + "data\\model\\" + filename);
	if(pFile)
	{
		cRsmModelBase::readData(*pFile);
		delete pFile;
	}
	
	CHAR buf[80];
	instream.read(buf, 80);			//nodeName
	instream>>pos;
	instream>>rot;
	instream>>scale;

	return instream;
}

bEngine::util::cOutStream& cRsmModel::writeData( bEngine::util::cOutStream &outstream )
{
	return outstream;
}



bEngine::util::cInStream &cRsmModelBase::cMesh::cFace::readData(bEngine::util::cInStream &instream)
{
	vertices[0] = instream.readWord();
	vertices[1] = instream.readWord();
	vertices[2] = instream.readWord();
	

	texvertices[0] = instream.readWord();
	texvertices[1] = instream.readWord();
	texvertices[2] = instream.readWord();
	
	texIndex = instream.readWord();
	instream.readWord();
	twoSide = instream.readInt();
	smoothGroup = instream.readInt();
	return instream;
}


bEngine::util::cOutStream &cRsmModelBase::cMesh::cFace::writeData(bEngine::util::cOutStream &outstream)
{
	outstream.writeWord(vertices[0]);
	outstream.writeWord(vertices[1]);
	outstream.writeWord(vertices[2]);
	
	outstream.writeWord(texvertices[0]);
	outstream.writeWord(texvertices[1]);
	outstream.writeWord(texvertices[2]);
	
	outstream.writeWord(texIndex);
	outstream.writeWord(0); //TODO
	outstream.writeInt(twoSide);
	outstream.writeInt(smoothGroup);
	return outstream;
}


bEngine::util::cInStream &cRsmModelBase::cMesh::cFrame::readData(bEngine::util::cInStream &instream)
{
	return instream>>time>>quat;
}

bEngine::util::cOutStream &cRsmModelBase::cMesh::cFrame::writeData(bEngine::util::cOutStream &outstream)
{
	return outstream<<time<<quat;
}


void cRsmModelBase::draw()
{
	if(root)
		root->draw();
}

cRsmModelBase::~cRsmModelBase()
{
	for(unsigned int i = 0; i < textures.size(); i++)
		bEngine::cTextureCache::unload(textures[i]);
	textures.clear();
}

bool cRsmModelBase::collides( bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* colPos )
{
	bEngine::math::cVector3 triangle[4];
	float bla;
	
	bool intersects = false;

	while(true)
	{
		triangle[0] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}

		triangle[0] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}

		triangle[0] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		triangle[1] = mat * bEngine::math::cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		triangle[2] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * bEngine::math::cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		

		break;
	}
	if(intersects)
	{
		return root->collides(mat, from, to, colPos);
	}
	return intersects;
}

void cRsmModelBase::setTexture( bEngine::cTexture* oldTexture, bEngine::cTexture* newTexture )
{
	root->setTexture(oldTexture, newTexture);
	for(unsigned int i = 0; i < textures.size(); i++)
	{
		if(textures[i] == oldTexture)
			textures[i] = newTexture;
	}
}

void cRsmModelBase::setSelection( cMesh* mesh )
{
	root->setSelection(mesh);
}

void cRsmModel::setHeight()
{
	bEngine::math::cMatrix4x4 mat;
	mat *= bEngine::math::cMatrix4x4::makeTranslation(5*pos.x, -pos.y, 5*pos.z);
	mat *= bEngine::math::cMatrix4x4::makeRotation(-rot.z, 0.0, 0.0, 1.0);
	mat *= bEngine::math::cMatrix4x4::makeRotation(-rot.x, 1.0, 0.0, 0.0);	
	mat *= bEngine::math::cMatrix4x4::makeRotation(rot.y, 0.0, 1.0, 0.0);
	mat *= bEngine::math::cMatrix4x4::makeScale(scale.x, -scale.y, scale.z);
	mat *= bEngine::math::cMatrix4x4::makeTranslation(-realbbrange.x, realbbmin.y, -realbbrange.z);

	bEngine::math::cVector3 bboxmin = mat * realbbmin;
	bEngine::math::cVector3 bboxmax = mat * realbbmax;

	for(int x = bEngine::math::min(bboxmin.x, bboxmax.x); x < bEngine::math::max(bboxmin.x, bboxmax.x); x++)
	{
		for(int y = bEngine::math::min(bboxmin.z, bboxmax.z); y < bEngine::math::max(bboxmin.z, bboxmax.z); y++)
		{
			cGraphics::world->cubes[y/10][x/10].minHeight = bEngine::math::min(cGraphics::world->cubes[y/10][x/10].minHeight, bEngine::math::min(bboxmin.y, bboxmax.y));
			cGraphics::world->cubes[y/10][x/10].maxHeight =bEngine::math:: max(cGraphics::world->cubes[y/10][x/10].maxHeight, bEngine::math::max(bboxmin.y, bboxmax.y));
		}
	}


}
bool cRsmModelBase::cMesh::collides( bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* colPos )
{
	bEngine::math::cMatrix4x4 myMat = mat * getMatrix1(false);
	bEngine::math::cMatrix4x4 mat2 = myMat * getMatrix2();
	bEngine::math::cVector3 triangle[4];
	float bla;
	int i;
	for(i = 0; i < nFaces; i++)
	{
		for(int ii = 0; ii < 3; ii++)
		{
			triangle[ii] = mat2 * vertices[faces[i].vertices[ii]];
		}
		if(LineIntersectPolygon(triangle, 3, from, to, bla))
		{
			if(colPos)
			{
				colPos->x = from.x * (1-bla) + to.x * (bla);
				colPos->y = from.y * (1-bla) + to.y * (bla);
				colPos->z = from.z * (1-bla) + to.z * (bla);
			}
			return true;
		}
	}
	
	bool b = false;
	for(i = 0; i < (int)children.size(); i++)
	{
		b = children[i]->collides(myMat, from, to, colPos);
		if(b)
			return true;
	}
	return false;
}

void cRsmModel::draw()
{
	glPushMatrix();
	glTranslatef(5*pos.x, -pos.y, 5*pos.z);
	
	glRotatef(-rot.z, 0.0, 0.0, 1.0);
	glRotatef(-rot.x, 1.0, 0.0, 0.0);	
	glRotatef(rot.y, 0.0, 1.0, 0.0);
	
	glScalef(scale.x, -scale.y, scale.z);

	glTranslatef(-realbbrange.x, realbbmin.y, -realbbrange.z);

	if(cGraphics::view.showBoundingBoxes)
	{
		float oldLineWidth;
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);
		glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);


		glDisable(GL_TEXTURE_2D);
		glColor3f(1,0,0);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
			glVertex3f(realbbmin.x, -realbbmin.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmin.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmin.y, realbbmax.z);
			glVertex3f(realbbmin.x, -realbbmin.y, realbbmax.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(realbbmin.x, -realbbmax.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmax.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmax.y, realbbmax.z);
			glVertex3f(realbbmin.x, -realbbmax.y, realbbmax.z);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(realbbmin.x, -realbbmin.y, realbbmin.z);
			glVertex3f(realbbmin.x, -realbbmax.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmin.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmax.y, realbbmin.z);
			glVertex3f(realbbmax.x, -realbbmin.y, realbbmax.z);
			glVertex3f(realbbmax.x, -realbbmax.y, realbbmax.z);
			glVertex3f(realbbmin.x, -realbbmin.y, realbbmax.z);
			glVertex3f(realbbmin.x, -realbbmax.y, realbbmax.z);
		glEnd();
		glLineWidth(oldLineWidth);
		glColor4fv(colors);
		glEnable(GL_TEXTURE_2D);
	}


	cRsmModelBase::draw();
	glPopMatrix();
}

bool cRsmModel::collides( bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* colPos )
{
	bEngine::math::cMatrix4x4 mat;
	mat *= bEngine::math::cMatrix4x4::makeTranslation(5*pos.x, -pos.y, 5*pos.z);
	mat *= bEngine::math::cMatrix4x4::makeRotation(-rot.z, 0.0, 0.0, 1.0);
	mat *= bEngine::math::cMatrix4x4::makeRotation(-rot.x, 1.0, 0.0, 0.0);	
	mat *= bEngine::math::cMatrix4x4::makeRotation(rot.y, 0.0, 1.0, 0.0);
	mat *= bEngine::math::cMatrix4x4::makeScale(scale.x, -scale.y, scale.z);
	mat *= bEngine::math::cMatrix4x4::makeTranslation(-realbbrange.x, realbbmin.y, -realbbrange.z);

	return cRsmModelBase::collides(mat, from, to, colPos);
}

cRsmModel::cRsmModel( bEngine::util::cInStream &instream )
{
	lightopacity = 1;
	readData(instream);
}





void cRsmModelBase::cMesh::fetchChildren( std::map<std::string, cMesh*, std::less<std::string> > &meshes)
{
	for(std::map<std::string, cMesh*, std::less<std::string> >::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		if(it->second->parentName == name && it->second != this)
		{
			it->second->parent = this;
			children.push_back(it->second);
		}
	}
	for(unsigned int i = 0; i < children.size(); i++)
		children[i]->fetchChildren(meshes);
}

void cRsmModelBase::cMesh::draw()
{
	glMultMatrixf(getMatrix1().values);
	glPushMatrix();
	glMultMatrixf(getMatrix2().values);

	
	GLfloat colors[4];
	if(selected)
	{
		glGetFloatv(GL_CURRENT_COLOR, colors);
		glColor4f(1,0,0,1);
	}

	int i;
	for(i = 0; i < nFaces; i++)
	{
		if(textures[faces[i].texIndex])
			glBindTexture(GL_TEXTURE_2D, textures[faces[i].texIndex]->texId());
		glBegin(GL_TRIANGLES);
		for(int ii = 0; ii < 3; ii++)
		{
			glNormal3fv(faces[i].normal.v);
			glTexCoord2f(texVertices[faces[i].texvertices[ii]].x, texVertices[faces[i].texvertices[ii]].y);
			glVertex3fv(vertices[faces[i].vertices[ii]].v);
		}
		glEnd();
	}
	if(selected)
		glColor4fv(colors);
	glPopMatrix();
	for(i = 0; i < (int)children.size(); i++)
	{
		glPushMatrix();
		children[i]->draw();
		glPopMatrix();
	}
}

void cRsmModelBase::cMesh::setBoundingBox(bEngine::math::cVector3 &_bbmin, bEngine::math::cVector3 &_bbmax)
{
	int c;
	int i;
	bbmin = bEngine::math::cVector3(9999999, 9999999, 9999999);
	bbmax = bEngine::math::cVector3(-9999999, -9999999, -9999999);

	if(parent != NULL)
		bbmin = bbmax = bEngine::math::cVector3(0,0,0);

	bEngine::math::cMatrix4x4 myMat = offset;
	for(i = 0; i < nFaces; i++)
	{
		for(int ii = 0; ii < 3; ii++)
		{
			bEngine::math::cVector3 v = myMat * vertices[faces[i].vertices[ii]];
			if(parent != NULL || children.size() != 0)
				v += pos + pos_;
			for(c = 0; c < 3; c++)

			{
				bbmin.v[c] = bEngine::math::min(bbmin.v[c], v.v[c]);
				bbmax.v[c] = bEngine::math::max(bbmax.v[c], v.v[c]);
			}
		}
	}
	bbrange = (bbmin + bbmax) / 2.0f;

	for(c = 0; c < 3; c++)
	for(i = 0; i < 3; i++)
	{
		_bbmax.v[c] = bEngine::math::max(_bbmax.v[c], bbmax.v[c]);
		_bbmin.v[c] = bEngine::math::min(_bbmin.v[c], bbmin.v[c]);
	}

	for(i = 0; i < (int)children.size(); i++)
		children[i]->setBoundingBox(_bbmin, _bbmax);

}


void cRsmModelBase::cMesh::setBoundingBox2(bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 &bbmin_, bEngine::math::cVector3 &bbmax_)
{
	bEngine::math::cMatrix4x4 myMat = mat * getMatrix1();
	bEngine::math::cMatrix4x4 mat2 = myMat * getMatrix2();
	int i;
	for(i = 0; i < nFaces; i++)
	{
		for(int ii = 0; ii < 3; ii++)
		{
			bEngine::math::cVector3 v = mat2 * vertices[faces[i].vertices[ii]];
			bbmin_.x = bEngine::math::min(bbmin_.x, v.x);
			bbmin_.y = bEngine::math::min(bbmin_.y, v.y);
			bbmin_.z = bEngine::math::min(bbmin_.z, v.z);
			
			bbmax_.x = bEngine::math::max(bbmax_.x, v.x);
			bbmax_.y = bEngine::math::max(bbmax_.y, v.y);
			bbmax_.z = bEngine::math::max(bbmax_.z, v.z);
		}
	}
	
	for(i = 0; i < (int)children.size(); i++)
		children[i]->setBoundingBox2(myMat, bbmin_, bbmax_);	
}

bEngine::math::cMatrix4x4 cRsmModelBase::cMesh::getMatrix1(bool animate)
{
	if(cache1)
		return matrix1Cache;
	matrix1Cache.setIdentity();

	if(parent == NULL)
	{
		if(children.size() > 0)
			matrix1Cache *= bEngine::math::cMatrix4x4::makeTranslation(-base->bbrange.x, -base->bbmax.y, -base->bbrange.z);
		else
			matrix1Cache *= bEngine::math::cMatrix4x4::makeTranslation(0, -base->bbmax.y+base->bbrange.y, 0);
	}

	if(parent != NULL)
		matrix1Cache *= bEngine::math::cMatrix4x4::makeTranslation(pos.x, pos.y, pos.z);
	
	if(nAnimationFrames == 0)
		matrix1Cache *= bEngine::math::cMatrix4x4::makeRotation(rotangle*180.0/3.14159, rotaxis.x, rotaxis.y, rotaxis.z);
	else
	{
		if(animate)
		{
			int current = 0;
			for(int i = 0; i < nAnimationFrames; i++)
			{
				if(animationFrames[i].time > lastTick)
				{
					current = i-1;
					break;
				}
			}
			if(current < 0)
				current = 0;
			
			int next = current+1;
			if(next >= nAnimationFrames)
				next = 0;

			float interval = ((float) (lastTick-animationFrames[current].time)) / ((float) (animationFrames[next].time-animationFrames[current].time));
#if 1
			bEngine::math::cQuaternion quat(animationFrames[current].quat, animationFrames[next].quat, interval);
#else
			bEngine::math::cQuaternion quat(
				(1-interval)*animationFrames[current].quat.x + interval*animationFrames[next].quat.x,
				(1-interval)*animationFrames[current].quat.y + interval*animationFrames[next].quat.y,
				(1-interval)*animationFrames[current].quat.z + interval*animationFrames[next].quat.z,
				(1-interval)*animationFrames[current].quat.w + interval*animationFrames[next].quat.w);
#endif
			quat = quat.getNormalized();

			matrix1Cache *= quat.getRotationMatrix();

			lastTick += cGraphicsBase::getFrameTicks();
			while(lastTick > animationFrames[nAnimationFrames-1].time)
				lastTick -= animationFrames[nAnimationFrames-1].time;
		}
		else
		{
			
			matrix1Cache *= animationFrames[0].quat.getNormalized().getRotationMatrix();

		}
	}
	
	matrix1Cache *= bEngine::math::cMatrix4x4::makeScale(scale.x, scale.y, scale.z);
	
	if(nAnimationFrames == 0)
		cache1 = true;
	return matrix1Cache;
}

bEngine::math::cMatrix4x4 cRsmModelBase::cMesh::getMatrix2()
{
	if(cache2)
		return matrix2Cache;

	matrix2Cache.setIdentity();

	if(parent == NULL && children.size() == 0)
		matrix2Cache *= bEngine::math::cMatrix4x4::makeTranslation(-base->bbrange.x, -base->bbrange.y, -base->bbrange.z);
	
	if(parent != NULL || children.size() != 0)
		matrix2Cache *= bEngine::math::cMatrix4x4::makeTranslation(pos_.x, pos_.y, pos_.z);
	
	matrix2Cache *= offset;
	cache2 = true;
	
	return matrix2Cache;
}

void cRsmModelBase::cMesh::setTexture( bEngine::cTexture* oldTexture, bEngine::cTexture* newTexture )
{
	unsigned int i;
	for(i = 0; i < textures.size(); i++)
	{
		if(textures[i] == oldTexture)
			textures[i] = newTexture;
	}
	for(i = 0; i < children.size(); i++)
		children[i]->setTexture(oldTexture, newTexture);
}

void cRsmModelBase::cMesh::setSelection( cMesh* mesh )
{
	selected = this == mesh;
	for(unsigned int i = 0; i < children.size(); i++)
		children[i]->setSelection(mesh);
}




void cRsmModelBase::save(std::string pFilename)
{
	char header[4] = { 'G','R','S','M' };
	int ver1 = 0;
	int ver2 = 0;
	int animLen = 0;
	int nTextures = 0;
	std::string mainNodeName = "";
	int numMeshes = 0;
	
	int i;
	root = NULL;
	
	bEngine::util::cFileSystem::cWriteFilePhysical* pFile = new bEngine::util::cFileSystem::cWriteFilePhysical(pFilename);
	
	pFile->write(header, 4);//header
	pFile->put(1); // version1
	pFile->put(5); // version2

	i = 0;
	pFile->write((char*)&i, 4);			//animlen
	pFile->write((char*)&shadeType,4);
	pFile->put(alpha);
	pFile->write(unknown, 16);
	pFile->write((char*)&nTextures,4);
	
	for(i = 0; i < nTextures; i++)
	{
		char textureName[40];
		strcpy(textureName, bEngine::util::replace(textures[i]->getFilename(), cSettings::roDir + "data\\texture\\", "").c_str());
		pFile->write(textureName, 40);
	}


	delete pFile;
	
/*	char buf[40];
	pFile->read(buf, 40);
	mainNodeName = buf;
	numMeshes = pFile->readInt();
	if(numMeshes != 1)
		Sleep(0);
	std::map<std::string, cMesh*, std::less<std::string> > meshes;
	for(i = 0; i < numMeshes; i++)
	{
		cMesh* mesh = new cMesh(pFile, this, ver1, ver2);
		meshes[mesh->name] = mesh;
	}*/
}

cRsmModelBase::cMesh::cFrame::cFrame() : quat(0,0,0,0)
{
	
}