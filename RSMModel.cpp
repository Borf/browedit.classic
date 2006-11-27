#include "RSMModel.h"
#include "Graphics.h"
#include "texturecache.h"
extern cFileSystem fs;
extern string rodir;
extern cGraphics Graphics;

void cRSMModel::load(string fname)
{
	filename = fname;
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

	do
	{
		cRSMModelMesh* mesh = new cRSMModelMesh();
		mesh->load(pFile, this, meshes.size() == 0);
		meshes.push_back(mesh);
	}
	while (pFile->data[pFile->index] != 0 && pFile->data[pFile->index+1] != 0 && pFile->data[pFile->index+2] != 0 && pFile->data[pFile->index+3] != 0 && pFile->index < pFile->size-32);

	pFile->close();

	fathers.push_back(0);

	for(i = 1; i < meshes.size(); i++)
	{
		bool found = false;
		for(int j = 0; j < meshes.size(); j++)
		{
			if (i != j && meshes[i]->name == meshes[j]->name)
			{
				fathers.push_back(j);
				found = true;
				break;
			}
		}

		if(!found)
		{
			fathers.push_back(0); //dunno if this is right
		}
	}
	boundingbox();
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

void MatrixMultVect(const float *M, cVector3 Vin, float *Vout) 
{
	Vout[0] = Vin[0]*M[0] + Vin[1]*M[4] + Vin[2]*M[8] + 1.0*M[12];
	Vout[1] = Vin[0]*M[1] + Vin[1]*M[5] + Vin[2]*M[9] + 1.0*M[13];
	Vout[2] = Vin[0]*M[2] + Vin[1]*M[6] + Vin[2]*M[10] + 1.0*M[14];
}



void cRSMModelMesh::boundingbox(float* ptransf)
{
	bool firstmesh = (ptransf == NULL);
	GLfloat Rot[16];
	int i;
	int j;

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

	bb.bbmin[0] = bb.bbmin[1] = bb.bbmin[2] = 99999999.0f;
	bb.bbmax[0] = bb.bbmax[1] = bb.bbmax[2] =-99999999.0f;

	for (i = 0; i < nVertices; i++)
	{
		float vout[3];

		MatrixMultVect(Rot, vertices[i], vout);
		for (j = 0; j < 3; j++) {
			GLfloat f;
			if (!firstmesh)
				f = vout[j]+trans[12+j]+trans[9+j];
			else
				f = vout[j];

			if (f < bb.bbmin[j])
				bb.bbmin[j] = f;
			if (f > bb.bbmax[j])
				bb.bbmax[j] = f;
		}
	}

	for (j=0; j < 3; j++)
		bb.bbrange[j] = (bb.bbmax[j]+bb.bbmin[j])/2.0;
}


void cRSMModel::draw(bool checkfrust)
{
	if (checkfrust)
	{
		if(!Graphics.frustum.PointInFrustum(5*pos.x, -pos.y, 5*(Graphics.world.height*2-pos.z)))
			return;
	}
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode
	glEnable(GL_BLEND);
	glTranslatef(5*pos.x, -pos.y, 5*pos.z);
	glRotatef(rot.x, 1.0, 0.0, 0.0);
	glRotatef(rot.y, 0.0, 1.0, 0.0);
	glRotatef(rot.z, 0.0, 0.0, 1.0);

	glScalef(scale.x, -scale.y, scale.z);
//	glScalef(0.5, 0.5, 0.5);
	draw2(&bb,0, NULL, meshes.size() == 1);

	glPopMatrix();
}

void cRSMModel::draw2(cBoundingbox* box, int mesh, float* transf, bool only)
{
	glPushMatrix();
	meshes[mesh]->draw(&bb,NULL, meshes.size() == 1);

	for(int i = 0; i < meshes.size(); i++)
	{
		if(i != mesh && fathers[i] == mesh)
			draw2((mesh == 0) ? box : NULL, i, meshes[mesh]->trans, only);
	}
	glPopMatrix();
}


void cRSMModelMesh::draw(cBoundingbox* box, float* ptransf, bool only)
{
	bool firstmesh = (ptransf == NULL);
	GLfloat Rot[16];
	int i;

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
	
	glScalef(trans[19], trans[20], trans[21]);
	if(firstmesh)
	{
		if(!only)
		{
			glTranslatef(-box->bbrange[0], -box->bbmax[1], -box->bbrange[2]);
		}
		else
		{
			glTranslatef(0, -box->bbmax[1]+box->bbrange[1], 0);
		}
	}

	if(!firstmesh)
		glTranslatef(trans[12], -trans[13], trans[14]);

	if(frames.size() == 0)
	{
		glRotatef(trans[15]*180.0/3.14159, trans[16], trans[17], trans[18]);
	}
	else
	{
	}

	glPushMatrix();

	if(firstmesh && only)
		glTranslatef(-box->bbrange[0], -box->bbrange[1], -box->bbrange[2]);

	if (!firstmesh || !only)
		glTranslatef(trans[9], trans[10], trans[11]);

	glMultMatrixf(Rot);

	for(i = 0; i < nFaces; i++)
	{
		cRSMModelFace* f = &faces[i];
		glBindTexture(GL_TEXTURE_2D, textures[f->texid]->texid());
		glBegin(GL_TRIANGLES);
			glTexCoord2f(texcoords[f->t[0]].y, 1-texcoords[f->t[0]].z); glVertex3f(vertices[f->v[0]].x, vertices[f->v[0]].y, vertices[f->v[0]].z);
			glTexCoord2f(texcoords[f->t[1]].y, 1-texcoords[f->t[1]].z); glVertex3f(vertices[f->v[1]].x, vertices[f->v[1]].y, vertices[f->v[1]].z);
			glTexCoord2f(texcoords[f->t[2]].y, 1-texcoords[f->t[2]].z); glVertex3f(vertices[f->v[2]].x, vertices[f->v[2]].y, vertices[f->v[2]].z);
		glEnd();


	}
	glPopMatrix();

}


void cRSMModel::boundingbox()
{
	int i;
	meshes[0]->boundingbox(NULL);
	

	for(i = 1; i < meshes.size(); i++)
	{
		if(fathers[i] == 0)
				meshes[i]->boundingbox(meshes[0]->trans);
	}

	for(i = 0; i < 3; i++)
	{
		bb.bbmax[i] = meshes[0]->bb.bbmax[i];
		bb.bbmin[i] = meshes[0]->bb.bbmin[i];
		for(int j = 1; j < meshes.size(); j++)
		{
			if (fathers[j] == 0)
			{
				if (bb.bbmax[i] < meshes[j]->bb.bbmax[i])
					bb.bbmax[i] = meshes[j]->bb.bbmax[i];
				if (bb.bbmin[i] > meshes[j]->bb.bbmin[i])
					bb.bbmin[i] = meshes[j]->bb.bbmin[i];
			}
		}
		bb.bbrange[i] = (bb.bbmax[i]+bb.bbmin[i]) /2.0f;
	}
}

cRSMModel::~cRSMModel()
{
	for(int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
		meshes[i] = NULL;
	}
	for(i = 0; i < textures.size(); i++)
	{
		TextureCache.unload(textures[i]);
	}
}