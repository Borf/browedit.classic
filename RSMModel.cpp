#include "RSMModel.h"
#include "Graphics.h"
#include "texturecache.h"
extern cFileSystem fs;
extern string rodir;
extern cGraphics Graphics;

void cRSMModel::load(string filename)
{
	Log(3,0,"Loading %s", filename.c_str());
	cFile* pFile = fs.open(filename);

	char buffer[100];
	pFile->read(buffer, 6); // header
	pFile->read(buffer, 25); // unknown

	pFile->read(buffer, 4); // ntextures;

	long ntextures = *((long*)buffer);

	for(int i = 0; i < ntextures; i++)
	{
		pFile->read(buffer, 40);
		string filename = buffer;
		cTexture* tex = TextureCache.load(rodir + "texture\\" + filename);
		textures.push_back(tex);
	}

	while(pFile->data[pFile->index] != 0 && pFile->data[pFile->index+1] != 0 && pFile->data[pFile->index+2] != 0 && pFile->data[pFile->index+3] != 0 && pFile->index < pFile->size-32)
	{
		Log(4,0,"Pos: %i, %i", pFile->index, pFile->size);
		cRSMModelMesh* mesh = new cRSMModelMesh();
		mesh->load(pFile, this, meshes.size() == 0);
		meshes.push_back(mesh);
	}

	pFile->close();
	Log(3,0,"Done loading %s", filename.c_str());
}

void cRSMModelMesh::load(cFile* pFile, cRSMModel* model, bool firstmesh)
{
	char buffer[100];
	pFile->read(buffer, 40);			//	naam
	name = buffer;

	if (firstmesh)
		pFile->read((char*)&todo, 4);	// todo

	pFile->read(buffer, 40);			//parentname
	parent = buffer;

	if(firstmesh)
		pFile->read((char*)&ftodo, 40);			// ftodo

	int nTextures;
	pFile->read((char*)&nTextures, 4);

	for(int i = 0; i < nTextures; i++)
	{
		int id;
		pFile->read((char*)&id, 4);
		textures.push_back(model->textures[id]);
	}

	pFile->read((char*)trans, 22*4);	// trans
	
	pFile->read((char*)&nVertices, 4);
	for(i = 0; i < nVertices; i++)
	{
		cVector3 v;
		pFile->read((char*)&v.x, 4);
		pFile->read((char*)&v.y, 4);
		pFile->read((char*)&v.z, 4);
		vertices.push_back(v);
	}

	pFile->read((char*)&nTexCoord, 4);
	for(i = 0; i < nTexCoord; i++)
	{
		cVector3 v;
		pFile->read((char*)&v.x, 4);
		pFile->read((char*)&v.y, 4);
		pFile->read((char*)&v.z, 4);
		texcoords.push_back(v);
	}

	pFile->read((char*)&nFaces, 4);
	for(i = 0; i < nFaces; i++)
	{
		cRSMModelFace f;
		pFile->read((char*)&f.v, sizeof(f.v));
		pFile->read((char*)&f.t, sizeof(f.t));
		pFile->read((char*)&f.texid, sizeof(f.texid));
		pFile->read((char*)&f.todo, sizeof(f.todo));
		pFile->read((char*)&f.todo2, sizeof(f.todo2));
		pFile->read((char*)&f.nsurf, sizeof(f.nsurf));
		faces.push_back(f);
	}
	pFile->read((char*)&nFrameAnimations, 4);
	for(i = 0; i < nFrameAnimations; i++)
	{
		cRSMModelFrame f;
		pFile->read((char*)&f.time, sizeof(f.time));
		pFile->read((char*)&f.quat, sizeof(f.quat));
		frames.push_back(f);
	}




}

void MatrixMultVect(const float *M, cVector3 *Vin, float *Vout) 
{
	Vout[0] = Vin[0]*M[0] + Vin[1]*M[4] + Vin[2]*M[8] + 1.0*M[12];
	Vout[1] = Vin[0]*M[1] + Vin[1]*M[5] + Vin[2]*M[9] + 1.0*M[13];
	Vout[2] = Vin[0]*M[2] + Vin[1]*M[6] + Vin[2]*M[10] + 1.0*M[14];
}



void cRSMModelMesh::boundingbox(float* ptransf)
{
	bool firstmesh = (ptransf == NULL);
	GLfloat Rot[16];
	GLfloat *Transf;
	int i;
	int j;
	int k;
	GLfloat pmax[3], pmin[3];

	Rot[0] = trans[0];
	Rot[1] = trans[1];
	Rot[2] = trans[2];
	Rot[3] = 0.0;

	Rot[4] = trans[3];
	Rot[5] = trans[4];
	Rot[6] = trans[5];
	Rot[7] = 0.0;

	Rot[8] = trans[6];
	Rot[9] = trans[7];
	Rot[10] = trans[8];
	Rot[11] = 0.0;

	Rot[12] = 0.0;
	Rot[13] = 0.0;
	Rot[14] = 0.0;
	Rot[15] = 1.0;

	bbmin[0] = bbmin[1] = bbmin[2] = 99999999.0f;
	bbmax[0] = bbmax[1] = bbmax[2] =-99999999.0f;

	for (i = 0; i < nVertices; i++)
	{
		float vtemp[3], vout[3];

		MatrixMultVect(Rot, vertices[i], vout);
		for (j = 0; j < 3; j++) {
			GLfloat f;
			if (!firstmesh)
				f = vout[j]+transf.todo[12+j]+transf.todo[9+j];
			else
				f = vout[j];

			bbmin[j] = min(f, bbmin[j]);
			bbmax[j] = max(f, bbmax[j]);
		}
	}

	for (j=0; j < 3; j++)
		range[j] = (max[j]+min[j])/2.0;
}


void cRSMModel::draw()
{
	if(!Graphics.frustum.PointInFrustum(5*pos.x, 5*pos.y, 5*pos.z))
		return;
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode
	glEnable(GL_BLEND);
	glTranslatef(5*pos.x, -5*pos.y, 5*pos.z);
	glRotatef(rot.x, 1.0, 0.0, 0.0);
	glRotatef(rot.y, 0.0, 1.0, 0.0);
	glRotatef(rot.z, 0.0, 0.0, 1.0);

	glScalef(scale.x/5.0, scale.y/5.0, scale.z/5.0);
//	glScalef(0.5, 0.5, 0.5);
	for(int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw();
	}


	glPopMatrix();
}


void cRSMModelMesh::draw()
{
	for(int i = 0; i < nFaces; i++)
	{
		cRSMModelFace* f = &faces[i];
		glBindTexture(GL_TEXTURE_2D, textures[f->texid]->texid());
		glBegin(GL_TRIANGLES);
			glTexCoord2f(texcoords[f->t[0]].y, 1-texcoords[f->t[0]].z); glVertex3f(vertices[f->v[0]].x, vertices[f->v[0]].z, vertices[f->v[0]].y);
			glTexCoord2f(texcoords[f->t[1]].y, 1-texcoords[f->t[1]].z); glVertex3f(vertices[f->v[1]].x, vertices[f->v[1]].z, vertices[f->v[1]].y);
			glTexCoord2f(texcoords[f->t[2]].y, 1-texcoords[f->t[2]].z); glVertex3f(vertices[f->v[2]].x, vertices[f->v[2]].z, vertices[f->v[2]].y);
		glEnd();


	}


}