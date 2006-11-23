#include "RSMModel.h"
#include "Graphics.h"
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
		cTexture* tex = new cTexture();
		tex->Load(rodir + "texture\\" + filename);
		textures.push_back(tex);
	}

	while(pFile->data[pFile->index] != NULL)
	{
		cRSMModelMesh* mesh = new cRSMModelMesh();
		mesh->load(pFile, this);
		meshes.push_back(mesh);
		break;
	}

	pFile->close();
	Log(3,0,"Done loading %s", filename.c_str());
}

void cRSMModelMesh::load(cFile* pFile, cRSMModel* model)
{
	char buffer[100];
	pFile->read(buffer, 40);
	name = buffer;

	pFile->read(buffer, 4);

	pFile->read(buffer, 40);
	parent = buffer;

	pFile->read(buffer, 40);

	int nTextures;
	pFile->read((char*)&nTextures, 4);

	for(int i = 0; i < nTextures; i++)
	{
		int id;
		pFile->read((char*)&id, 4);
		textures.push_back(model->textures[id]);
	}

	pFile->read((char*)trans, 22*4);
	
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


void cRSMModel::draw()
{
	if(!Graphics.frustum.PointInFrustum(5*pos.x, 5*pos.y, 5*pos.z))
		return;
	glPushMatrix();
	glTranslatef(5*pos.x, -5*pos.y, 5*pos.z);
	glRotatef(rot.x+90, 1.0, 0.0, 0.0);
	glRotatef(rot.y, 0.0, 1.0, 0.0);
	glRotatef(rot.z+90, 0.0, 0.0, 1.0);

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
			glTexCoord2f(texcoords[f->t[0]].y, texcoords[f->t[0]].z); glVertex3f(vertices[f->v[0]].x, vertices[f->v[0]].y, vertices[f->v[0]].z);
			glTexCoord2f(texcoords[f->t[1]].y, texcoords[f->t[1]].z); glVertex3f(vertices[f->v[1]].x, vertices[f->v[1]].y, vertices[f->v[1]].z);
			glTexCoord2f(texcoords[f->t[2]].y, texcoords[f->t[2]].z); glVertex3f(vertices[f->v[2]].x, vertices[f->v[2]].y, vertices[f->v[2]].z);
		glEnd();


	}


}