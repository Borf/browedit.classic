#include "common.h"
#include "RSMModel.h"
#include "graphics.h"
#include "texturecache.h"
#include <frustum.h>
#include "settings.h"
bool boundingBoxCollisions = false;

void cRSMModel::load(std::string fname)
{
	glPushMatrix();
	glLoadIdentity();
	unsigned int i;
//	Log(3,0,"Loading %s", fname.c_str());
	recalcbb = true;
	filename = fname;
	rofilename = filename.substr(filename.find("model\\") + 6);
	cFile* pFile = cFileSystem::open(filename);
	if (pFile == NULL)
	{
		glPopMatrix();
		return;
	}


	char buffer[100];
	pFile->read(buffer, 6); // header
	if (buffer[5] == 4)
		pFile->read(buffer, 25); // unknown
	else
		pFile->read(buffer, 24); // unknown

	pFile->read(buffer, 4); // ntextures;

	unsigned long ntextures = *((long*)buffer);

	animated = false;
	if(ntextures < 1 || ntextures > 1000)
	{
		glPopMatrix();
		return;
	}

	for(i = 0; i < ntextures; i++)
	{
		pFile->read(buffer, 40);
		std::string filename = buffer;
		cTexture* tex = cTextureCache::load(cSettings::roDir + "data\\texture\\" + filename, (eTextureOptions)(TEX_NEARESTFILTER | TEX_NOCLAMP));
		textures.push_back(tex);
	}

	do
	{
		cRSMModel::cMesh* mesh = new cRSMModel::cMesh();
		mesh->load(pFile, this, meshes.size() == 0);
		meshes.push_back(mesh);
	}
	while (pFile->data[pFile->index] != 0 && pFile->data[pFile->index+1] != 0 && pFile->data[pFile->index+2] != 0 && pFile->data[pFile->index+3] != 0 && pFile->index < pFile->size-32);

	pFile->close();

	fathers.push_back(0);

	for(i = 1; i < meshes.size(); i++)
	{
		bool found = false;
		for(unsigned int j = 0; j < meshes.size(); j++)
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
	glPopMatrix();
}

void cRSMModel::cMesh::load(cFile* pFile, cRSMModel* model, bool main)
{
	unsigned int i;
	char buffer[100];
	pFile->read(buffer, 40);			//	naam
	name = buffer;

	if (main)
		pFile->read((char*)&todo, 4);	// todo

	pFile->read(buffer, 40);			//parentname
	parent = buffer;

	if(main)
		pFile->read((char*)&ftodo, 40);			// ftodo

	unsigned int nTextures;
	pFile->read((char*)&nTextures, 4);
	if (nTextures > 1000 || nTextures < 0)
	{
		glPopMatrix();
		return;
	}
	for(i = 0; i < nTextures; i++)
	{
		unsigned int id;
		pFile->read((char*)&id, 4);
		if (id < 0 || id > model->textures.size())
			textures.push_back(0);
		else
			textures.push_back(id);
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
		cRSMModel::cFace f;
		pFile->read((char*)&f.v, sizeof(f.v));
		pFile->read((char*)&f.t, sizeof(f.t));
		pFile->read((char*)&f.texid, sizeof(f.texid));
		if(f.texid > textures.size())
			Log(2,0,"Wrong texture offset on face %i", f.texid); 
		pFile->read((char*)&f.todo, sizeof(f.todo));
		pFile->read((char*)&f.todo2, sizeof(f.todo2));
		pFile->read((char*)&f.nsurf, sizeof(f.nsurf));

		
		cVector3 b1, b2;
		b1 = vertices[f.v[0]] - vertices[f.v[2]];
		b2 = vertices[f.v[1]] - vertices[f.v[2]];
		f.normal = b1.cross(b2).getnormalized();//cVector3(b1.y * b2.z - b1.z * b2.y, b1.z * b2.x - b1.x * b2.z, b1.x * b2.y - b1.y * b2.x);

		faces.push_back(f);
	}
	pFile->read((char*)&nFrameAnimations, 4);
	if(nFrameAnimations != 0)
		model->animated = true;
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



void cRSMModel::cMesh::boundingbox(float* ptransf, bool only)
{
	GLfloat Rot[16];
	unsigned int i;
	unsigned int j;

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


void cRSMModel::draw(bool checkfrust, bool dodraw, bool setheight, bool dolightmaps)
{
	if (checkfrust)
	{
		if(!cFrustum::cubeInFrustum(5*pos.x, -pos.y, 5*(cGraphics::world->height*2-pos.z), max(max(bb.bbmax[0],bb.bbmax[1]), bb.bbmax[2])))
//		if(!cGraphics::frustum.PointInFrustum(5*pos.x, -pos.y, 5*(cGraphics::world->height*2-pos.z)))
		{
			return;
		}
	}
	if (meshes.size() == 0)
		return;
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode
	glEnable(GL_BLEND);
	glTranslatef(5*pos.x, -pos.y, 5*pos.z);

	//Fixed rotation order. by Henko
	glRotatef(-rot.z, 0.0, 0.0, 1.0);
	glRotatef(-rot.x, 1.0, 0.0, 0.0);	
	glRotatef(rot.y, 0.0, 1.0, 0.0);

	glScalef(scale.x, -scale.y, scale.z);
//	glScalef(0.5, 0.5, 0.5);

	if(dodraw || dolightmaps)
	{
		if (bb2.bbmin[1] != 0 && fabs(bb2.bbmax[1]) < 1)
		{
			bb2.bbmax[1] -=bb2.bbmin[1];
			bb2.bbmin[1] = 0;
		}
			glTranslatef(-bb2.bbrange[0], bb2.bbmin[1], -bb2.bbrange[2]);
	}

	if(dodraw && cGraphics::view.showBoundingBoxes)
	{
		bb2.Draw();
	}

#ifdef DISPLAYLIST
	if(!displaylisted && !setheight && !dolightmaps && dodraw && !animated)
	{
		displaylist=glGenLists(1);
		glNewList(displaylist,GL_COMPILE);	
		draw2(&bb,0, NULL, meshes.size() == 1, dodraw, setheight, dolightmaps);
		glEndList();
		displaylisted = true;
		draw2(&bb,0, NULL, meshes.size() == 1, dodraw, setheight, dolightmaps);
	}
	else if (displaylisted && dodraw && !setheight && !dolightmaps)
	{
		glCallList(displaylist);
	}
	else
		draw2(&bb,0, NULL, meshes.size() == 1, dodraw, setheight, dolightmaps);
#else


	//TODO: this should be given the models real bounding box calculated from the meshes real bounding boxes, 
	//that is from the boundaries of transformed used(!) vertices. The bounging box should not be rotated! by Henko

	draw2(&bb,0, NULL, meshes.size() == 1, dodraw, setheight, dolightmaps);

#endif

	
	recalcbb = false;


	if(!dodraw)
	{
		bb2.bbmin[0] = bb2.bbmin[1] = bb2.bbmin[2] = 999999;
		bb2.bbmax[0] = bb2.bbmax[1] = bb2.bbmax[2] = -999999;
		for(unsigned int i = 0; i < meshes.size(); i++)
		{
			for(unsigned int ii = 0; ii < 3; ii++)
			{
				bb2.bbmin[ii] = min(bb2.bbmin[ii], meshes[i]->realbb.bbmin[ii]);
				bb2.bbmax[ii] = max(bb2.bbmax[ii], meshes[i]->realbb.bbmax[ii]);
			}
		}
		for (unsigned int j=0; j < 3; j++)
			bb2.bbrange[j] = (bb2.bbmax[j]+bb2.bbmin[j])/2.0;
	}

	glPopMatrix();
}

void cRSMModel::draw2(cBoundingbox* box, int mesh, float* transf, bool only, bool dodraw, bool setheight, bool dolightmaps)
{
	glPushMatrix();
	meshes[mesh]->draw(box,transf, meshes.size() == 1, this, dodraw, setheight, dolightmaps);

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		if((int)i != mesh && fathers[i] == mesh)
			draw2((mesh == 0) ? box : NULL, i, meshes[mesh]->trans, only, dodraw, setheight, dolightmaps);
	}
	glPopMatrix();
}







void cRSMModel::cMesh::draw(cBoundingbox* box, float* ptransf, bool only, cRSMModel* model, bool dodraw, bool setheight, bool dolightmaps)
{
	bool main = (ptransf == NULL);
	GLfloat Rot[16];
	GLfloat Ori[16];
	unsigned int i;

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
	{	unsigned int i;
		int current = 0;
		unsigned int next;
		GLfloat t;

		for (i = 0; i < frames.size(); i++) {
			if (nstep < frames[i].time) {
				current = i-1;
				break;
			}
		}
		if(current < 0)
			current = 0;

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

		nstep += cGraphicsBase::getFrameTicks();
		if (nstep >= frames[frames.size()-1].time)
			nstep = 0;
	}

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

	glScalef(trans[19], trans[20], trans[21]);


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

		float mmin = min(min(model->bb2.bbmin[0], model->bb.bbmin[1]), model->bb2.bbmin[2]) / 5;
		mmin -= 4;
		float mmax = max(max(model->bb2.bbmax[0], model->bb.bbmax[1]), model->bb2.bbmax[2]) / 5;
		mmax += 4;

		for(i = 0; i < nFaces; i++)
		{
			cRSMModel::cFace* f = &faces[i];
			if(!setheight && !dolightmaps)
			{
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
			if(setheight)
			{
				float v1_[3];
				float v2_[3];
				float v3_[3];
				MatrixMultVect(ModelMatrix, vertices[f->v[0]], v1_);
				MatrixMultVect(ModelMatrix, vertices[f->v[1]], v2_);
				MatrixMultVect(ModelMatrix, vertices[f->v[2]], v3_);

				
				cVector3 v1 = cVector3(v1_[0], v1_[1], v1_[2]);
				cVector3 v2 = cVector3(v2_[0], v2_[1], v2_[2]);
				cVector3 v3 = cVector3(v3_[0], v3_[1], v3_[2]);

				cVector3 xinc = v2 - v1;
				cVector3 yinc = v3 - v1;

				for(float xi = 0; xi < 1; xi+=0.1f)
				{
					for(float yi = 0; yi < xi; yi+=0.1f)
					{
						
						int y2 = (int)floor((v1.z+xinc.z*xi+yinc.z*yi)/10.0);
						int x2 = (int)floor((v1.x+xinc.x*xi+yinc.x*yi)/10.0);

					//	if (y1 < 0 || y2 < 0 || x1 < 0 || x2 < 0 ||
					//		y1 >= cGraphics::world->height || y2 >= cGraphics::world->height || x1 >= cGraphics::world->width || y2 >= cGraphics::world->width)
					//		continue;

						if (y2 < 0 || y2 >= cGraphics::world->height || x2 < 0 || x2 >= cGraphics::world->width)
							continue;

						float h = (v1.y+xinc.y*xi+yinc.y*yi);
						/*cGraphics::world->cubes[y1][x1].minh = min(cGraphics::world->cubes[y1][x1].minh, -h);
						cGraphics::world->cubes[y1][x1].maxh = max(cGraphics::world->cubes[y1][x1].maxh, -h);
						cGraphics::world->cubes[y1][x2].minh = min(cGraphics::world->cubes[y1][x2].minh, -h);
						cGraphics::world->cubes[y1][x2].maxh = max(cGraphics::world->cubes[y1][x2].maxh, -h);*/
						cGraphics::world->cubes[y2][x2].minHeight = min(cGraphics::world->cubes[y2][x2].minHeight, -h);
						cGraphics::world->cubes[y2][x2].maxHeight = max(cGraphics::world->cubes[y2][x2].maxHeight, -h);
						/*cGraphics::world->cubes[y2][x1].minh = min(cGraphics::world->cubes[y2][x1].minh, -h);
						cGraphics::world->cubes[y2][x1].maxh = max(cGraphics::world->cubes[y2][x1].maxh, -h);*/
					}
				}
			}
			if(dolightmaps)
			{
/*old lightmapping code, can be removed
				float v[3];
				cVector3 triangle[3];
				MatrixMultVect(ModelMatrix, vertices[f->v[0]], v);
				triangle[0] = cVector3(v[0], v[1], v[2]);
				MatrixMultVect(ModelMatrix, vertices[f->v[1]], v);
				triangle[1] = cVector3(v[0], v[1], v[2]);
				MatrixMultVect(ModelMatrix, vertices[f->v[2]], v);
				triangle[2] = cVector3(v[0], v[1], v[2]);
				

//				cVector3 normal = Normal(triangle);
//				cVector3 camera = cVector3(0,1000,0);

//				if (camera.Dot(normal) < 0)
//					continue;

				printf(".");
				float t = 0;

				int x2 = (int)min((int)cGraphics::world->width, (int)(model->pos.x/2+mmax*2));
				int y2 = (int)min((int)cGraphics::world->height, (int)(model->pos.z/2+ mmax*2));

				for(int x = (int)max(0, (int)(model->pos.x/2 + mmin*2)); x < x2; x++)
				{
					for(int y = (int)max(0, (int)(model->pos.z/2 + mmin*2)); y < y2; y++)
					{
						for(int xx = 0; xx < 6; xx++)
						{
							for(int yy = 0; yy < 6; yy++)
							{
								cVector3 v = cVector3(10*x+10*(xx/6.0),-cGraphics::world->cubes[y][x].cell1, 10*y+10*(yy/6.0));
								if (LineIntersectPolygon(triangle, 3, lightpos, v, t))
								{
									int tile = cGraphics::world->cubes[y][x].tileUp;
									cLightmap* l = cGraphics::world->lightmaps[cGraphics::world->tiles[tile].lightmap];
									
									l->buf[xx + (8*yy)+1+8] =(char) (((BYTE)l->buf[xx + (8*yy)+1+8]) / 1.3);
								}
							}
						}
					}
				}
*/

			}

		}
		if(!setheight && !dolightmaps)
		{
			MatrixMultVect(ModelMatrix, v1, realbb.bbmin);
			MatrixMultVect(ModelMatrix, v2, realbb.bbmax);
		}


	}

	if(dodraw)
	{
		for(i = 0; i < nFaces; i++)
		{
			cRSMModel::cFace* f = &faces[i];
			glBindTexture(GL_TEXTURE_2D, model->textures[textures[f->texid]]->texId());
			glNormal3f(f->normal.x, f->normal.y, f->normal.z);
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
	unsigned int i;
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
		for(unsigned int j = 1; j < meshes.size(); j++)
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
	unsigned int i;
	for(i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
		meshes[i] = NULL;
	}
	for(i = 0; i < textures.size(); i++)
	{
		cTextureCache::unload(textures[i]);
	}
}


bool cRSMModel::collides(cVector3 start, cVector3 end)
{
/* optimize that should be tested
	if(start.x > end.x && pos.x > start.x)
		return false;
	if(start.x > end.x && pos.x < end.x)
		return false;
	if(start.x < end.x && pos.x < start.x)
		return false;
	if(start.x < end.x && pos.x > end.x)
		return false;

  if(start.z > end.z && pos.z > start.z)
		return false;
	if(start.z > end.z && pos.z < end.z)
		return false;
	if(start.z < end.z && pos.z < start.z)
		return false;
	if(start.z < end.z && pos.z > end.z)
		return false;

*/
	if(meshes.size() == 0)
		return false;
		
	cVector3 triangle[3] = { cVector3(0,0,0), cVector3(0,0,0), cVector3(0,0,0) };
	float t = 0;

	bool collide = false;
	do
	{
		//bottom
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv2_[0], -absolutebbv2_[1], absolutebbv2_[2]);
		triangle[2] = cVector3(absolutebbv3_[0], -absolutebbv3_[1], absolutebbv3_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv3_[0], -absolutebbv3_[1], absolutebbv3_[2]);
		triangle[2] = cVector3(absolutebbv4_[0], -absolutebbv4_[1], absolutebbv4_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		//top
		triangle[0] = cVector3(absolutebbv5_[0], -absolutebbv5_[1], absolutebbv5_[2]);
		triangle[1] = cVector3(absolutebbv6_[0], -absolutebbv6_[1], absolutebbv6_[2]);
		triangle[2] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv5_[0], -absolutebbv5_[1], absolutebbv5_[2]);
		triangle[1] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		triangle[2] = cVector3(absolutebbv8_[0], -absolutebbv8_[1], absolutebbv8_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}


		//front
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv2_[0], -absolutebbv2_[1], absolutebbv2_[2]);
		triangle[2] = cVector3(absolutebbv6_[0], -absolutebbv6_[1], absolutebbv6_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv5_[0], -absolutebbv5_[1], absolutebbv5_[2]);
		triangle[2] = cVector3(absolutebbv6_[0], -absolutebbv6_[1], absolutebbv6_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}

		//back
		triangle[0] = cVector3(absolutebbv4_[0], -absolutebbv4_[1], absolutebbv4_[2]);
		triangle[1] = cVector3(absolutebbv3_[0], -absolutebbv3_[1], absolutebbv3_[2]);
		triangle[2] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv4_[0], -absolutebbv5_[1], absolutebbv4_[2]);
		triangle[1] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		triangle[2] = cVector3(absolutebbv8_[0], -absolutebbv8_[1], absolutebbv8_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}

		//left
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv5_[0], -absolutebbv5_[1], absolutebbv5_[2]);
		triangle[2] = cVector3(absolutebbv8_[0], -absolutebbv8_[1], absolutebbv8_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv1_[0], -absolutebbv1_[1], absolutebbv1_[2]);
		triangle[1] = cVector3(absolutebbv4_[0], -absolutebbv4_[1], absolutebbv4_[2]);
		triangle[2] = cVector3(absolutebbv8_[0], -absolutebbv8_[1], absolutebbv8_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}

		//right
		triangle[0] = cVector3(absolutebbv2_[0], -absolutebbv2_[1], absolutebbv2_[2]);
		triangle[1] = cVector3(absolutebbv3_[0], -absolutebbv3_[1], absolutebbv3_[2]);
		triangle[2] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
		triangle[0] = cVector3(absolutebbv2_[0], -absolutebbv2_[1], absolutebbv2_[2]);
		triangle[1] = cVector3(absolutebbv6_[0], -absolutebbv6_[1], absolutebbv6_[2]);
		triangle[2] = cVector3(absolutebbv7_[0], -absolutebbv7_[1], absolutebbv7_[2]);
		if(LineIntersectPolygon(triangle, 3, start, end, t))
		{
			collide = true;
			break;
		}
	} while(1 == 0);

	if(!collide)
		return false;


	if(boundingBoxCollisions)
		return true;

	glPushMatrix();
	glTranslatef(5*pos.x, -pos.y, 5*pos.z);
	glRotatef(-rot.x, 1.0, 0.0, 0.0);
	glRotatef(-rot.z, 0.0, 0.0, 1.0);
	glRotatef(rot.y, 0.0, 1.0, 0.0);

	glScalef(scale.x, -scale.y, scale.z);

	glTranslatef(-bb2.bbrange[0], bb2.bbmin[1], -bb2.bbrange[2]);
	bool b = collides2(&bb,0, NULL, meshes.size() == 1, start, end);
	recalcbb = false;

	glPopMatrix();
	
	return b;
}


bool cRSMModel::collides2(cBoundingbox* box, int mesh, float* transf, bool only, cVector3 start, cVector3 end)
{
	glPushMatrix();
	bool b = meshes[mesh]->collides(box,transf, meshes.size() == 1, this, start, end);
	if(b)
	{
		glPopMatrix();
		return true;
	}

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		if((int)i != mesh && fathers[i] == mesh)
		{
			b = collides2((mesh == 0) ? box : NULL, i, meshes[mesh]->trans, only, start, end);
			if (b)
			{
				glPopMatrix();
				return true;
			}
		}
	}
	glPopMatrix();
	return false;
}





bool cRSMModel::cMesh::collides(cBoundingbox* box, float* ptransf, bool only, cRSMModel* model, cVector3 start, cVector3 end)
{
	bool main = (ptransf == NULL);
	GLfloat Rot[16];
	GLfloat Ori[16];
	unsigned int i;

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
	{	
		unsigned int i;
		int current = 0;
		unsigned int next;
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

	glScalef(trans[19], trans[20], trans[21]);


	glPushMatrix();

	if(main && only)
		glTranslatef(-box->bbrange[0], -box->bbrange[1], -box->bbrange[2]);

	if (!main || !only)
		glTranslatef(trans[9], trans[10], trans[11]);

	glMultMatrixf(Rot);

	cVector3 v1 = cVector3(999999,999999,999999);
	cVector3 v2 = cVector3(-999999,-999999,-999999);

	float ModelMatrix[16]; 
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelMatrix);
	float mmin = min(min(model->bb2.bbmin[0], model->bb.bbmin[1]), model->bb2.bbmin[2]) / 5;
	mmin -= 4;
	float mmax = max(max(model->bb2.bbmax[0], model->bb.bbmax[1]), model->bb2.bbmax[2]) / 5;
	mmax += 4;

	for(i = 0; i < nFaces; i++)
	{
		cRSMModel::cFace* f = &faces[i];
		float v[3];
		cVector3 triangle[3];
		MatrixMultVect(ModelMatrix, vertices[f->v[0]], v);
		triangle[0] = cVector3(v[0], v[1], v[2]);
		MatrixMultVect(ModelMatrix, vertices[f->v[1]], v);
		triangle[1] = cVector3(v[0], v[1], v[2]);
		MatrixMultVect(ModelMatrix, vertices[f->v[2]], v);
		triangle[2] = cVector3(v[0], v[1], v[2]);

		float t = 0;

		if (LineIntersectPolygon(triangle, 3, end, start, t))
		{
			glPopMatrix();
			return true;
		}
	}

	glPopMatrix();
	return false; 
}

cRSMModel::cMesh::cMesh()
{
	nstep = 0;
}

void cBoundingbox::Draw()
{
	cVector3 v1 = cVector3(bbmin[0], bbmin[1], bbmin[2]);
	cVector3 v2 = cVector3(bbmax[0], bbmax[1], bbmax[2]);
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
}


void cRSMModel::precollides()
{
	if (meshes.size() == 0)
		return;
	glPushMatrix();
	glLoadIdentity();
	//glTranslatef(0,0,cGraphics::world->height*10);
	//glScalef(1,1,-1);
	glTranslatef(5*pos.x, pos.y, 5*pos.z);
	glRotatef(-rot.x, 1.0, 0.0, 0.0);
	glRotatef(-rot.z, 0.0, 0.0, 1.0);
	glRotatef(rot.y, 0.0, 1.0, 0.0);

	glScalef(scale.x, -scale.y, scale.z);

	glTranslatef(-bb2.bbrange[0], -bb2.bbmin[1], -bb2.bbrange[2]);

	float ModelMatrix[16]; 
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelMatrix);
	float ProjMatrix[16]; 
	glGetFloatv(GL_PROJECTION_MATRIX, ProjMatrix);

	cVector3 v1 = cVector3(bb2.bbmin[0], bb2.bbmin[1], bb2.bbmin[2]);
	cVector3 v2 = cVector3(bb2.bbmax[0], bb2.bbmax[1], bb2.bbmax[2]);

	MatrixMultVect(ModelMatrix, cVector3(v1.x, v1.y, v1.z), absolutebbv1_);
	MatrixMultVect(ModelMatrix, cVector3(v2.x, v1.y, v1.z), absolutebbv2_);
	MatrixMultVect(ModelMatrix, cVector3(v2.x, v1.y, v2.z), absolutebbv3_);
	MatrixMultVect(ModelMatrix, cVector3(v1.x, v1.y, v2.z), absolutebbv4_);
	MatrixMultVect(ModelMatrix, cVector3(v1.x, v2.y, v1.z), absolutebbv5_);
	MatrixMultVect(ModelMatrix, cVector3(v2.x, v2.y, v1.z), absolutebbv6_);
	MatrixMultVect(ModelMatrix, cVector3(v2.x, v2.y, v2.z), absolutebbv7_);
	MatrixMultVect(ModelMatrix, cVector3(v1.x, v2.y, v2.z), absolutebbv8_);
	glPopMatrix();	
	return;
}
