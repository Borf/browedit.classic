#include "RSMModel.h"
#include "Graphics.h"
#include "texturecache.h"
extern cFileSystem fs;
extern string rodir;
extern cGraphics Graphics;

void cRSMModel::load(string fname)
{
	recalcbb = true;
	filename = fname;
	rofilename = filename.substr(filename.find("model\\") + 6);
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
	pos = cVector3(0,0,0);
	rot = cVector3(0,0,0);
	scale = cVector3(1,1,1);
	draw(false,false);
}

void cRSMModelMesh::load(cFile* pFile, cRSMModel* model, bool main)
{
	char buffer[100];
	pFile->read(buffer, 40);			//	naam
	name = buffer;

	if (main)
		pFile->read((char*)&todo, 4);	// todo

	pFile->read(buffer, 40);			//parentname
	parent = buffer;

	if(main)
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



void cRSMModelMesh::boundingbox(float* ptransf, bool only)
{
	bool main = (ptransf == NULL);
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
//		vout[0] = vertices[i][0];
//		vout[1] = vertices[i][1];
//		vout[2] = vertices[i][2];

		for (j = 0; j < 3; j++) {
			GLfloat f;
			if (!only)
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


void cRSMModel::draw(bool checkfrust, bool dodraw)
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

	if(dodraw)
	{
		glTranslatef(-bb2.bbrange[0], bb2.bbmin[1], -bb2.bbrange[2]);
	}

	if(dodraw && Graphics.showboundingboxes)
	{
		cVector3 v1 = cVector3(bb2.bbmin[0], bb2.bbmin[1], bb2.bbmin[2]);
		cVector3 v2 = cVector3(bb2.bbmax[0], bb2.bbmax[1], bb2.bbmax[2]);
		glColor4f(1,0,1,1);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex3f(v1.x, -v1.y, v1.z);
			glVertex3f(v2.x, -v1.y, v1.z);
			glVertex3f(v2.x, -v1.y, v2.z);
			glVertex3f(v1.x, -v1.y, v2.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(v1.x, -v2.y, v1.z);
			glVertex3f(v2.x, -v2.y, v1.z);
			glVertex3f(v2.x, -v2.y, v2.z);
			glVertex3f(v1.x, -v2.y, v2.z);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(v1.x, -v1.y, v1.z);
			glVertex3f(v1.x, -v2.y, v1.z);
			glVertex3f(v2.x, -v1.y, v1.z);
			glVertex3f(v2.x, -v2.y, v1.z);
			glVertex3f(v2.x, -v1.y, v2.z);
			glVertex3f(v2.x, -v2.y, v2.z);
			glVertex3f(v1.x, -v1.y, v2.z);
			glVertex3f(v1.x, -v2.y, v2.z);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
	}


	draw2(&bb,0, NULL, meshes.size() == 1, dodraw);
	recalcbb = false;


	if(!dodraw)
	{
		bb2.bbmin[0] = bb2.bbmin[1] = bb2.bbmin[2] = 999999;
		bb2.bbmax[0] = bb2.bbmax[1] = bb2.bbmax[2] = -999999;
		for(int i = 0; i < meshes.size(); i++)
		{
			for(int ii = 0; ii < 3; ii++)
			{
				bb2.bbmin[ii] = min(bb2.bbmin[ii], meshes[i]->realbb.bbmin[ii]);
				bb2.bbmax[ii] = max(bb2.bbmax[ii], meshes[i]->realbb.bbmax[ii]);
			}
		}
		for (int j=0; j < 3; j++)
			bb2.bbrange[j] = (bb2.bbmax[j]+bb2.bbmin[j])/2.0;
	}

	glPopMatrix();
}

void cRSMModel::draw2(cBoundingbox* box, int mesh, float* transf, bool only, bool dodraw)
{
	glPushMatrix();
	meshes[mesh]->draw(box,transf, meshes.size() == 1, this, dodraw);

	for(int i = 0; i < meshes.size(); i++)
	{
		if(i != mesh && fathers[i] == mesh)
			draw2((mesh == 0) ? box : NULL, i, meshes[mesh]->trans, only, dodraw);
	}
	glPopMatrix();
}


void cRSMModelMesh::draw(cBoundingbox* box, float* ptransf, bool only, cRSMModel* model, bool dodraw)
{
	bool main = (ptransf == NULL);
	GLfloat Rot[16];
	GLfloat Ori[16];
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

	if(frames.size() > 0)
	{	int i;
		int current = 0;
		int next;
		GLfloat t;

		for (i = 0; i < frames.size(); i++) {
			if (nstep < frames[i].time) {
				current = i-1;
				break;
			}
		}

		next = current + 1;
		if (next == frames.size())
			next = 0;

		t = ((GLfloat) (nstep-frames[current].time))
			/((GLfloat) (frames[next].time-frames[current].time));

		//for (i = 0; i < 4; i++) {
		//	q[i] = frames[current].orientation[i] * (1-t) + t * frames[next].orientation[i];
		//}

		float x = frames[current].quat[0] * (1-t) + t * frames[next].quat[0];
		float y = frames[current].quat[1] * (1-t) + t * frames[next].quat[1];
		float z = frames[current].quat[2] * (1-t) + t * frames[next].quat[2];
		float w = frames[current].quat[3] * (1-t) + t * frames[next].quat[3];

		GLfloat norm;
		norm = sqrtf(x*x+y*y+z*z+w*w);

		//for (i = 0; i < 4; i++)
		//	q[i] /= norm;
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;

		// First row
		Ori[ 0] = 1.0f - 2.0f * ( y * y + z * z ); 
		Ori[ 1] = 2.0f * (x * y + z * w);
		Ori[ 2] = 2.0f * (x * z - y * w);
		Ori[ 3] = 0.0f;  

		// Second row
		Ori[ 4] = 2.0f * ( x * y - z * w );  
		Ori[ 5] = 1.0f - 2.0f * ( x * x + z * z ); 
		Ori[ 6] = 2.0f * (z * y + x * w );  
		Ori[ 7] = 0.0f;  

		// Third row
		Ori[ 8] = 2.0f * ( x * z + y * w );
		Ori[ 9] = 2.0f * ( y * z - x * w );
		Ori[10] = 1.0f - 2.0f * ( x * x + y * y );  
		Ori[11] = 0.0f;  

		// Fourth row
		Ori[12] = 0;  
		Ori[13] = 0;  
		Ori[14] = 0;  
		Ori[15] = 1.0f;

		nstep += 100;
		if (nstep >= frames[frames.size()-1].time)
			nstep = 0;
	}
	
	glScalef(trans[19], trans[20], trans[21]);

	if(main)
	{	if(!only)
			glTranslatef(-box->bbrange[0], -box->bbmax[1], -box->bbrange[2]);
		else
			glTranslatef(0, -box->bbmax[1]+box->bbrange[1], 0);
	}

	if(!main)
		glTranslatef(trans[12], trans[13], trans[14]);

	if(frames.size() == 0)
		glRotatef(trans[15]*180.0/3.14159, trans[16], trans[17], trans[18]);
	else
		glMultMatrixf(Ori);



	glPushMatrix();

	if(main && only)
		glTranslatef(-box->bbrange[0], -box->bbrange[1], -box->bbrange[2]);

	if (!main || !only)
		glTranslatef(trans[9], trans[10], trans[11]);

	glMultMatrixf(Rot);

	cVector3 v1 = cVector3(999999,999999,999999);
	cVector3 v2 = cVector3(-999999,-999999,-999999);

	if(!dodraw)
	{
		float ModelMatrix[16]; 
		glGetFloatv(GL_MODELVIEW_MATRIX, ModelMatrix);
		float ProjMatrix[16];
		glGetFloatv(GL_PROJECTION_MATRIX, ProjMatrix);
		for(i = 0; i < nFaces; i++)
		{
			cRSMModelFace* f = &faces[i];
			v1.x = min(v1.x, vertices[f->v[0]].x);
			v1.y = min(v1.y, vertices[f->v[0]].y);
			v1.z = min(v1.z, vertices[f->v[0]].z);
			v1.x = min(v1.x, vertices[f->v[1]].x);
			v1.y = min(v1.y, vertices[f->v[1]].y);
			v1.z = min(v1.z, vertices[f->v[1]].z);
			v1.x = min(v1.x, vertices[f->v[2]].x);
			v1.y = min(v1.y, vertices[f->v[2]].y);
			v1.z = min(v1.z, vertices[f->v[2]].z);

			v2.x = max(v2.x, vertices[f->v[0]].x);
			v2.y = max(v2.y, vertices[f->v[0]].y);
			v2.z = max(v2.z, vertices[f->v[0]].z);
			v2.x = max(v2.x, vertices[f->v[1]].x);
			v2.y = max(v2.y, vertices[f->v[1]].y);
			v2.z = max(v2.z, vertices[f->v[1]].z);
			v2.x = max(v2.x, vertices[f->v[2]].x);
			v2.y = max(v2.y, vertices[f->v[2]].y);
			v2.z = max(v2.z, vertices[f->v[2]].z);
		}
		MatrixMultVect(ModelMatrix, v1, realbb.bbmin);
		MatrixMultVect(ModelMatrix, v2, realbb.bbmax);
	}

	if(dodraw)
	{
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
	}	
	glPopMatrix();

}


void cRSMModel::boundingbox()
{
	int i;
	meshes[0]->boundingbox(NULL, meshes.size() == 1);
	

	for(i = 1; i < meshes.size(); i++)
	{
		if(fathers[i] == 0)
				meshes[i]->boundingbox(meshes[0]->trans, meshes.size() == 1);
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