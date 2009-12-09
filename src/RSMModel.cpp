#include "RSMModel.h"
#include <filesystem.h>
#include <texture.h>
#include <texturecache.h>
#include <settings.h>
#include <graphics.h>
#include <world.h>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <map>


cRsmModelBase::cRsmModelBase( std::string pFilename)
{
	char header[4];
	int ver1 = 0;
	int ver2 = 0;
	int animLen = 0;
	int nTextures = 0;
	std::string mainNodeName = "";
	int numMeshes = 0;

	int i;
	root = NULL;

	cFile* pFile = cFileSystem::open(pFilename);
	if (pFile == NULL)
	{
		Log(2,0,"cRsmModel: Could not open '%s'", pFilename.c_str());
		return;
	}
	
	pFile->read(header, 4);
	if(memcmp(header, "GRSM", 4) != 0)
	{
		Log(2,0,"cRsmModel: %s is not a model, invalid header", pFilename.c_str());
		pFile->close();
		return;
	}
	ver1 = pFile->get();
	ver2 = pFile->get();
	animLen = pFile->getInt();
	shadeType = (eShadeType)pFile->getInt();
	if(ver1 >= 1 && ver2 >= 4)
		int alpha = pFile->get();

	char unknown[16];
	pFile->read(unknown, 16);
	nTextures = pFile->getInt();

	for(i = 0; i < nTextures; i++)
	{
		char textureName[40];
		pFile->read(textureName, 40);
		textures.push_back(cTextureCache::load(cSettings::roDir + "data\\texture\\" + textureName, (eTextureOptions)(TEX_NEARESTFILTER | TEX_NOCLAMP)));
	}
	
	char buf[40];
	pFile->read(buf, 40);
	mainNodeName = buf;
	numMeshes = pFile->getInt();
	if(numMeshes != 1)
		Sleep(0);
	std::map<std::string, cMesh*, std::less<std::string> > meshes;
	for(i = 0; i < numMeshes; i++)
	{
		cMesh* mesh = new cMesh(pFile, this, ver1, ver2);
		meshes[mesh->name] = mesh;
	}

	if(pFile->index > pFile->size)
	{
		Log(2,0,"Error: read too much data!");
		return;
	}

	if(meshes.find(mainNodeName) == meshes.end())
	{
		Log(2,0,"cRsmModel: Could not locate root mesh");
		root = meshes.begin()->second;
	}
	else
		root = meshes[mainNodeName];
	root->fetchChildren(meshes);



	bbmin = cVector3(999999, 999999, 999999);
	bbmax = cVector3(-999999, -999999, -9999999);
	root->setBoundingBox(bbmin, bbmax);
	bbrange = (bbmin + bbmax) / 2.0f;

	
	realbbmax = cVector3(-999999, -999999, -999999);
	realbbmin = cVector3(999999, 999999, 999999);
	cMatrix4x4 mat = cMatrix4x4::makeScale(1,-1,1);
	root->setBoundingBox2(mat, realbbmin, realbbmax);
	realbbrange = (realbbmax + realbbmin) / 2.0f;
	maxrange = max(max(realbbmax.x, -realbbmin.x), max(max(realbbmax.y, -realbbmin.y), max(realbbmax.z, -realbbmin.z)));

}

cRsmModel::cMesh::cMesh(cFile* pFile, cRsmModelBase* model, int ver1, int ver2)
{
	cache1 = false;
	cache2 = false;
	base = model;
	lastTick = 0;
	parent = NULL;
	char buf[40];
	int i;
	pFile->read(buf, 40);
	name = buf;
	pFile->read(buf, 40);
	parentName = buf;
	
	int nTextures = pFile->getInt();
	for(i = 0; i < nTextures; i++)
		textures.push_back(model->textures[pFile->getInt()]);

	offset.values[0] = pFile->getFloat();	//rotation
	offset.values[1] = pFile->getFloat();
	offset.values[2] = pFile->getFloat();

	offset.values[4] = pFile->getFloat();
	offset.values[5] = pFile->getFloat();
	offset.values[6] = pFile->getFloat();
	
	offset.values[8] = pFile->getFloat();
	offset.values[9] = pFile->getFloat();
	offset.values[10] = pFile->getFloat();

	pos_.x = pFile->getFloat();
	pos_.y = pFile->getFloat();
	pos_.z = pFile->getFloat();

	pos.x = pFile->getFloat();
	pos.y = pFile->getFloat();
	pos.z = pFile->getFloat();

	rotangle = pFile->getFloat();
	rotaxis.x = pFile->getFloat();
	rotaxis.y = pFile->getFloat();
	rotaxis.z = pFile->getFloat();

	scale.x = pFile->getFloat();
	scale.y = pFile->getFloat();
	scale.z = pFile->getFloat();


	nVertices = pFile->getInt();
	vertices = new cVector3[nVertices];
	for(i = 0; i < nVertices; i++)
	{
		vertices[i].x = pFile->getFloat();
		vertices[i].y = pFile->getFloat();
		vertices[i].z = pFile->getFloat();
	}

	nTexVertices = pFile->getInt();
	texVertices = new cVector2[nTexVertices];
	for(i = 0; i < nTexVertices; i++)
	{
		if(ver1 >= 1 && ver2 >= 2)
			pFile->getFloat();
		texVertices[i].x = pFile->getFloat();
		texVertices[i].y = 1-pFile->getFloat();
	}
	nFaces = pFile->getInt();
	faces = new cFace[nFaces];
	for(i = 0; i < nFaces; i++)
	{
		faces[i].vertices[0] = pFile->getword();
		faces[i].vertices[1] = pFile->getword();
		faces[i].vertices[2] = pFile->getword();

		faces[i].normal = (vertices[faces[i].vertices[1]] - vertices[faces[i].vertices[0]]).cross(vertices[faces[i].vertices[2]] - vertices[faces[i].vertices[0]]).getnormalized();

		faces[i].texvertices[0] = pFile->getword();
		faces[i].texvertices[1] = pFile->getword();
		faces[i].texvertices[2] = pFile->getword();

		faces[i].texIndex = pFile->getword();
		pFile->getword();
		faces[i].twoSide = pFile->getInt();
		faces[i].smoothGroup = pFile->getInt();
	}
	nAnimationFrames = pFile->getInt();
	animationFrames = new cFrame[nAnimationFrames];
	for(i = 0; i < nAnimationFrames; i++)
	{
		animationFrames[i].time = pFile->getInt();
		animationFrames[i].quat.values[0] = pFile->getFloat();
		animationFrames[i].quat.values[1] = pFile->getFloat();
		animationFrames[i].quat.values[2] = pFile->getFloat();
		animationFrames[i].quat.values[3] = pFile->getFloat();
	}
}





void cRsmModelBase::draw()
{
	if(root)
		root->draw();
}

cRsmModelBase::~cRsmModelBase()
{
	for(unsigned int i = 0; i < textures.size(); i++)
		cTextureCache::unload(textures[i]);
	textures.clear();
}

bool cRsmModelBase::collides( cMatrix4x4 &mat, cVector3 from, cVector3 to, cVector3* colPos )
{
	cVector3 triangle[4];
	float bla;
	
	bool intersects = false;

	while(true)
	{
		triangle[0] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[1] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}

		triangle[0] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		triangle[2] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
		triangle[3] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}

		triangle[0] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmin.z);
		triangle[1] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmin.z);
		triangle[2] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmin.z);
		triangle[3] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmin.z);
		if(LineIntersectPolygon(triangle, 4, from, to, bla))
		{
			intersects = true;
			break;
		}
		
		triangle[0] = mat * cVector3(realbbmin.x, -realbbmax.y, realbbmax.z);
		triangle[1] = mat * cVector3(realbbmin.x, -realbbmin.y, realbbmax.z);
		triangle[2] = mat * cVector3(realbbmax.x, -realbbmin.y, realbbmax.z);
		triangle[3] = mat * cVector3(realbbmax.x, -realbbmax.y, realbbmax.z);
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

void cRsmModel::setHeight()
{
	cMatrix4x4 mat;
	mat *= cMatrix4x4::makeTranslation(5*pos.x, -pos.y, 5*pos.z);
	mat *= cMatrix4x4::makeRotation(-rot.z, 0.0, 0.0, 1.0);
	mat *= cMatrix4x4::makeRotation(-rot.x, 1.0, 0.0, 0.0);	
	mat *= cMatrix4x4::makeRotation(rot.y, 0.0, 1.0, 0.0);
	mat *= cMatrix4x4::makeScale(scale.x, -scale.y, scale.z);
	mat *= cMatrix4x4::makeTranslation(-realbbrange.x, realbbmin.y, -realbbrange.z);

	cVector3 bboxmin = mat * realbbmin;
	cVector3 bboxmax = mat * realbbmax;

	for(int x = min(bboxmin.x, bboxmax.x); x < max(bboxmin.x, bboxmax.x); x++)
	{
		for(int y = min(bboxmin.z, bboxmax.z); y < max(bboxmin.z, bboxmax.z); y++)
		{
			cGraphics::world->cubes[y/10][x/10].minHeight = min(cGraphics::world->cubes[y/10][x/10].minHeight, min(bboxmin.y, bboxmax.y));
			cGraphics::world->cubes[y/10][x/10].maxHeight = max(cGraphics::world->cubes[y/10][x/10].maxHeight, max(bboxmin.y, bboxmax.y));
		}
	}


}
bool cRsmModelBase::cMesh::collides( cMatrix4x4 &mat, cVector3 from, cVector3 to, cVector3* colPos )
{
	cMatrix4x4 myMat = mat * getMatrix1(false);
	cMatrix4x4 mat2 = myMat * getMatrix2();
	cVector3 triangle[4];
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
		glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, colors);


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

bool cRsmModel::collides( cVector3 from, cVector3 to, cVector3* colPos )
{
	cMatrix4x4 mat;
	mat *= cMatrix4x4::makeTranslation(5*pos.x, -pos.y, 5*pos.z);
	mat *= cMatrix4x4::makeRotation(-rot.z, 0.0, 0.0, 1.0);
	mat *= cMatrix4x4::makeRotation(-rot.x, 1.0, 0.0, 0.0);	
	mat *= cMatrix4x4::makeRotation(rot.y, 0.0, 1.0, 0.0);
	mat *= cMatrix4x4::makeScale(scale.x, -scale.y, scale.z);
	mat *= cMatrix4x4::makeTranslation(-realbbrange.x, realbbmin.y, -realbbrange.z);

	return cRsmModelBase::collides(mat, from, to, colPos);
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
	
	int i;
	for(i = 0; i < nFaces; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[faces[i].texIndex]->texId());
		glBegin(GL_TRIANGLES);
		for(int ii = 0; ii < 3; ii++)
		{
			glNormal3fv(faces[i].normal.data);
			glTexCoord2f(texVertices[faces[i].texvertices[ii]].x, texVertices[faces[i].texvertices[ii]].y);
			glVertex3fv(vertices[faces[i].vertices[ii]].data);
		}
		glEnd();
	}
	glPopMatrix();
	for(i = 0; i < (int)children.size(); i++)
	{
		glPushMatrix();
		children[i]->draw();
		glPopMatrix();
	}
}

void cRsmModelBase::cMesh::setBoundingBox(cVector3 &_bbmin, cVector3 &_bbmax)
{
	int c;
	int i;
	bbmin = cVector3(9999999, 9999999, 9999999);
	bbmax = cVector3(-9999999, -9999999, -9999999);

	if(parent != NULL)
		bbmin = bbmax = cVector3(0,0,0);

	cMatrix4x4 myMat = offset;
	for(i = 0; i < nFaces; i++)
	{
		for(int ii = 0; ii < 3; ii++)
		{
			cVector3 v = myMat * vertices[faces[i].vertices[ii]];
			if(parent != NULL || children.size() != 0)
				v += pos + pos_;
			for(c = 0; c < 3; c++)
			{
				bbmin[c] = min(bbmin[c], v[c]);
				bbmax[c] = max(bbmax[c], v[c]);
			}
		}
	}
	bbrange = (bbmin + bbmax) / 2.0f;

	for(c = 0; c < 3; c++)
	{
		_bbmax[c] = max(_bbmax[c], bbmax[c]);
		_bbmin[c] = min(_bbmin[c], bbmin[c]);
	}

	for(i = 0; i < (int)children.size(); i++)
		children[i]->setBoundingBox(_bbmin, _bbmax);

}


void cRsmModelBase::cMesh::setBoundingBox2(cMatrix4x4 &mat, cVector3 &bbmin_, cVector3 &bbmax_)
{
	cMatrix4x4 myMat = mat * getMatrix1();
	cMatrix4x4 mat2 = myMat * getMatrix2();
	int i;
	for(i = 0; i < nFaces; i++)
	{
		for(int ii = 0; ii < 3; ii++)
		{
			cVector3 v = mat2 * vertices[faces[i].vertices[ii]];
			bbmin_.x = min(bbmin_.x, v.x);
			bbmin_.y = min(bbmin_.y, v.y);
			bbmin_.z = min(bbmin_.z, v.z);
			
			bbmax_.x = max(bbmax_.x, v.x);
			bbmax_.y = max(bbmax_.y, v.y);
			bbmax_.z = max(bbmax_.z, v.z);
		}
	}
	
	for(i = 0; i < (int)children.size(); i++)
		children[i]->setBoundingBox2(myMat, bbmin_, bbmax_);	
}

cMatrix4x4 cRsmModelBase::cMesh::getMatrix1(bool animate)
{
	if(cache1)
		return matrix1Cache;
	matrix1Cache.setIdentity();

	if(parent == NULL)
	{
		if(children.size() > 0)
			matrix1Cache *= cMatrix4x4::makeTranslation(-base->bbrange.x, -base->bbmax.y, -base->bbrange.z);
		else
			matrix1Cache *= cMatrix4x4::makeTranslation(0, -base->bbmax.y+base->bbrange.y, 0);
	}

	if(parent != NULL)
		matrix1Cache *= cMatrix4x4::makeTranslation(pos.x, pos.y, pos.z);
	
	if(nAnimationFrames == 0)
		matrix1Cache *= cMatrix4x4::makeRotation(rotangle*180.0/3.14159, rotaxis.x, rotaxis.y, rotaxis.z);
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
			cQuaternion quat(animationFrames[current].quat, animationFrames[next].quat, interval);
#else
			cQuaternion quat(
				(1-interval)*animationFrames[current].quat.x + interval*animationFrames[next].quat.x,
				(1-interval)*animationFrames[current].quat.y + interval*animationFrames[next].quat.y,
				(1-interval)*animationFrames[current].quat.z + interval*animationFrames[next].quat.z,
				(1-interval)*animationFrames[current].quat.w + interval*animationFrames[next].quat.w);
#endif
			quat = quat.normalize();

			matrix1Cache *= quat.getRotationMatrix();

			lastTick += cGraphicsBase::getFrameTicks();
			while(lastTick > animationFrames[nAnimationFrames-1].time)
				lastTick -= animationFrames[nAnimationFrames-1].time;
		}
		else
		{
			
			matrix1Cache *= animationFrames[0].quat.normalize().getRotationMatrix();

		}
	}
	
	matrix1Cache *= cMatrix4x4::makeScale(scale.x, scale.y, scale.z);
	
	if(nAnimationFrames == 0)
		cache1 = true;
	return matrix1Cache;
}

cMatrix4x4 cRsmModelBase::cMesh::getMatrix2()
{
	if(cache2)
		return matrix2Cache;

	matrix2Cache.setIdentity();

	if(parent == NULL && children.size() == 0)
		matrix2Cache *= cMatrix4x4::makeTranslation(-base->bbrange.x, -base->bbrange.y, -base->bbrange.z);
	
	if(parent != NULL || children.size() != 0)
		matrix2Cache *= cMatrix4x4::makeTranslation(pos_.x, pos_.y, pos_.z);
	
	matrix2Cache *= offset;
	cache2 = true;
	return matrix2Cache;
}
