#include "main.h"
#include "world.h"
#include "graphics.h"
#include "filesystem.h"
#include <math.h>
#include <list>
#include "texturecache.h"
#ifdef _WIN32
#include <gd/gd.h>
#else
#include <gd.h>
#endif

#define floatheight 0.1f



extern cGraphics Graphics;

extern double mouse3dx, mouse3dy, mouse3dz;
extern float mousex, mousey;
extern cFileSystem fs;
extern eMode	editmode;
extern int brushsize;
extern string rodir;

extern long userid;


void cWorld::load()
{
	int i;
	if(light == NULL)
	{
		light = new cTextureModel();
		light->open("data/bulb.tga");
	}
	if(effect == NULL)
	{
		effect = new cTextureModel();
		effect->open("data/explosion.tga");
	}
	if(sound == NULL)
	{
		sound = new cTextureModel();
		sound->open("data/Speaker.tga");
	}

	for(i = 0; i < textures.size(); i++)
		TextureCache.unload(textures[i]->texture);
	textures.clear();

	tiles.clear();
	for(i = 0; i < cubes.size(); i++)
		cubes[i].clear();
	cubes.clear();

	for(i = 0; i < lightmaps.size(); i++)
	{
		lightmaps[i]->del();
		lightmaps[i]->del2();
		delete lightmaps[i];
	}
	lightmaps.clear();

	for(i = 0; i < models.size(); i++)
		delete models[i];
	models.clear();
	delete root;
	root = NULL;
	quadtreefloats.clear();
	sounds.clear();
	effects.clear();

	Log(3,0,"Loading %s", filename);
	cFile* pFile = fs.open(string(filename) + ".gnd");
	if(pFile == NULL)
	{
		Log(1,0,"Could not open %s!", (string(filename) + ".gnd").c_str());
	}
	char buf[512];
	pFile->read(buf, 26);

	width = *((int*)(buf+6));
	height = *((int*)(buf+10));
	tilescale = *((float*)(buf+14));

	int nTextures = *((int*)(buf+18));
	for(i = 0; i < nTextures; i++)
	{
		pFile->read(buf, 80);
		string s = string(buf);
		cTextureContainer* t = new cTextureContainer();
		t->RoFilename = s;
		t->RoFilename2 = string(buf+40);
		t->texture = TextureCache.load(rodir + "texture\\" + s);
		textures.push_back(t);
	}

	pFile->read(buf, 16);
	lightmapWidth = *((int*)(buf+4));
	lightmapHeight = *((int*)(buf+8));
	gridSizeCell = *((int*)(buf+12));


	int nLightmaps = *((int*)buf);
	for(i = 0; i < nLightmaps; i++)
	{
		cLightmap* l = new cLightmap();
		pFile->read(l->buf, 256);
		lightmaps.push_back(l);
	}

	long nTiles;
	pFile->read((char*)&nTiles, 4);
	for(i = 0; i < nTiles; i++)
	{
		pFile->read(buf, 40);
		cTile t;
		memcpy((char*)&t.u1, buf, 4);
		memcpy((char*)&t.u2, buf+4, 4);
		memcpy((char*)&t.u3, buf+8, 4);
		memcpy((char*)&t.u4, buf+12, 4);
		memcpy((char*)&t.v1, buf+16, 4);
		memcpy((char*)&t.v2, buf+20, 4);
		memcpy((char*)&t.v3, buf+24, 4);
		memcpy((char*)&t.v4, buf+28, 4);

		t.texture = ((BYTE)buf[32]) | (((BYTE)buf[33])<<8);
		t.lightmap = ((BYTE)buf[34]) | (((BYTE)buf[35])<<8);

		if(t.lightmap < 0 || t.lightmap > lightmaps.size())
			t.lightmap = 0;
		if(t.texture < 0 || t.texture > textures.size())
			t.texture = 0;

		memcpy(t.color, buf+36, 4);
		tiles.push_back(t);
	}

	for(int y = 0; y < height; y++)
	{
		vector<cCube> row;
		row.clear();
		for(int x = 0; x < width; x++)
		{
			pFile->read(buf, 28);
			cCube c;
			c.maxh = -99999;
			c.minh = 99999;
			memcpy((char*)&c.cell1, buf, 4);
			memcpy((char*)&c.cell2, buf+4, 4);
			memcpy((char*)&c.cell3, buf+8, 4);
			memcpy((char*)&c.cell4, buf+12, 4);
			c.tileup = *((int*)(buf+16));
			c.tileside = *((int*)(buf+20));
			c.tileaside = *((int*)(buf+24));
			row.push_back(c);
		}
		cubes.push_back(row);
	}

	pFile->close();
	loaded = true;
	clean();

	Log(3,0,"Done loading gnd");

	Log(3,0,"Loading rsw...");
	pFile = fs.open(string(filename) + ".rsw");

	pFile->read(buf, 242);
	useless = string(buf+166, 76);

	char* w = (char*)useless.c_str();
	water.height = *((float*)(w));
	water.type = *((int*)(w+4));
	water.amplitude = *((float*)(w+8));
	water.phase = *((float*)(w+12));
	water.surfacecurve = *((float*)(w+16));
	water.texcycle = *((int*)(w+20));
	
	

	pFile->read(buf, 4);
	long nObjects = *((long*)buf);


	for(i = 0; i < nObjects; i++)
	{
		pFile->read(buf, 4);
		long type = *((long*)buf);
		switch(type)
		{
		case 1:
			{
			pFile->read(buf, 248);
			string filename = buf+52;
			cRSMModel* m = new cRSMModel();
			m->load(rodir+ "model\\" + filename);
			m->id = models.size();


			m->pos.x = *((float*)(buf+212));
			m->pos.y = *((float*)(buf+216));
			m->pos.z = *((float*)(buf+220));


			m->pos.x = (m->pos.x / 5) + width;
			m->pos.z = (m->pos.z / 5) + height;

			m->rot.x = *((float*)(buf+224));
			m->rot.y = *((float*)(buf+228));
			m->rot.z = *((float*)(buf+232));

			m->scale.x = *((float*)(buf+236));
			m->scale.y = *((float*)(buf+240));
			m->scale.z = *((float*)(buf+244));
			models.push_back(m);
			}
			break;
		case 2:
			{
			pFile->read(buf, 108);
			cLight l;
			l.name = string(buf);
			l.pos.x = *((float*)(buf+40));
			l.pos.y = *((float*)(buf+44));
			l.pos.z = *((float*)(buf+48));

			l.pos.x = (l.pos.x / 5) + width;
			l.pos.z = (l.pos.z / 5) + height;

			l.todo = string(buf+52, 40);
			l.color.x = *((float*)(buf+92));
			l.color.y = *((float*)(buf+96));
			l.color.z = *((float*)(buf+100));
			l.todo2 = *((float*)(buf+104));
			lights.push_back(l);
			}
			break;
		case 3:
			{
			pFile->read(buf, 192);
			cSound s;			
			s.name = string(buf);
			s.todo1 = string(buf+40, 40);
			s.filename = string(buf+80);
			s.todo2 = string(buf+120,20);
			s.pos.x = *((float*)(buf+140));
			s.pos.y = *((float*)(buf+144));
			s.pos.z = *((float*)(buf+148));
			s.id = string(buf+152, 40);
			s.pos.x = (s.pos.x / 5) + width;
			s.pos.z = (s.pos.z / 5) + height;
			sounds.push_back(s);
			}
			break;
		case 4:
			{
			pFile->read(buf, 116);
			cEffect e;
			e.name = string(buf);
			e.todo1 = *((float*)(buf+40));
			e.todo2 = *((float*)(buf+44));
			e.todo3 = *((float*)(buf+48));
			e.todo4 = *((float*)(buf+52));
			e.todo5 = *((float*)(buf+56));
			e.todo6 = *((float*)(buf+60));
			e.todo7 = *((float*)(buf+64));
			e.todo8 = *((float*)(buf+68));
			e.todo9 = *((float*)(buf+72));
			e.category = string(buf+76, 4);
			e.pos.x = *((float*)(buf+80));
			e.pos.y = *((float*)(buf+84));
			e.pos.z = *((float*)(buf+88));
			e.type = *((int*)(buf+92));
			e.loop = *((float*)(buf+96));
			e.todo10 = *((float*)(buf+100));
			e.todo11 = *((float*)(buf+104));
			e.todo12 = *((int*)(buf+108));
			e.todo13 = *((int*)(buf+112));
			e.pos.x = (e.pos.x / 5) + width;
			e.pos.z = (e.pos.z / 5) + height;
			effects.push_back(e);
			}
			break;
		default:
			Log(2,0,"Unknown type: %i", type);
			pFile->close();
			return;
		};
	}
	quadtreefloats.clear();
	while(!pFile->eof())
	{
		cVector3 f;
		pFile->read((char*)&f.x, 4);
		pFile->read((char*)&f.y, 4);
		pFile->read((char*)&f.z, 4);
		quadtreefloats.push_back(f);
	}
	pFile->close(); 

	
	if(quadtreefloats.size() > 0)
	{
		root = new cQuadTreeNode();
		root->load(quadtreefloats, 0, 0);
	}


	pFile = fs.open(string(filename) + ".gat");

	pFile->read(buf, 14);
	
	for(y = 0; y < height*2; y++)
	{
		vector<cGatTile> row;
		for(int x = 0; x < width*2; x++)
		{
			cGatTile g;
			pFile->read((char*)&g.cell1, 4);
			pFile->read((char*)&g.cell2, 4);
			pFile->read((char*)&g.cell3, 4);
			pFile->read((char*)&g.cell4, 4);
			g.type = pFile->get();

			pFile->get();pFile->get();pFile->get();
			row.push_back(g);
		}
		gattiles.push_back(row);
	}

	pFile->close();



	Log(3,0,"Done Loading %s", filename);

}


int cQuadTreeNode::load(vector<cVector3>& data, int index, int level)
{
	child1 = NULL;
	child2 = NULL;
	child3 = NULL;
	child4 = NULL;

	box1 = data[index];
	box2 = data[index+1];
	range1 = data[index+2];
	range2 = data[index+3];
	index+=4;

	if (level < 5)
	{
		child1 = new cQuadTreeNode();
		index = child1->load(data, index, level+1);
		child2 = new cQuadTreeNode();
		index = child2->load(data, index, level+1);
		child3 = new cQuadTreeNode();
		index = child3->load(data, index, level+1);
		child4 = new cQuadTreeNode();
		index = child4->load(data, index, level+1);
	}
	return index;
}


void cQuadTreeNode::draw(int level)
{
	if (level == 0)
	{
		cVector3 v = box1;
		cVector3 v2 = box2;

		v.x += Graphics.world.width*5;
		v.y = -v.y;
		v.z = Graphics.world.height*5 - v.z;

		v2.x += Graphics.world.width*5;
		v2.y = -v2.y;
		v2.z = Graphics.world.height*5 - v2.z;

		glBegin(GL_LINE_LOOP);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(v2.x, v.y, v.z);
			glVertex3f(v2.x, v.y, v2.z);
			glVertex3f(v.x, v.y, v2.z);
		glEnd();	

		glBegin(GL_LINE_LOOP);
			glVertex3f(v.x, v2.y, v.z);
			glVertex3f(v2.x, v2.y, v.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v.x, v2.y, v2.z);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(v.x, v2.y, v.z);
			glVertex3f(v2.x, v.y, v.z);
			glVertex3f(v2.x, v2.y, v.z);
			glVertex3f(v2.x, v.y, v2.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v.x, v.y, v2.z);
			glVertex3f(v.x, v2.y, v2.z);
		glEnd();
		return;
	}
	if (level > 0)
	{
		if(child1 != NULL)
			child1->draw(level-1);
		if(child2 != NULL)
			child2->draw(level-1);
		if(child3 != NULL)
			child3->draw(level-1);
		if(child4 != NULL)
			child4->draw(level-1);
	}
}

void cWorld::exportheight()
{
	/*cFile* pFile = fs.open(filename);
	//jpegmap[x+y*(tilespertex)] = gdImageCreateFromJpegPtr(pFile->size, pFile->data);
	pFile->close();*/

}

void cWorld::save()
{
	srand(GetTickCount());
	if (!loaded)
		return;
	{
		clean();
		quadtreefloats.clear();
		if(root != NULL)
		{
			int x,y;
			for(x = 0; x < width; x++)
				for(y = 0; y < height; y++)
				{
					cubes[y][x].maxh = -99999;
					cubes[y][x].minh = 99999;
				}

			for(int i = 0; i < models.size(); i++)
			{
				if(i % 10 == 0)
					Log(3,0,"Quadtree: looking at model %i out of %i (%.2f%%)", i, Graphics.world.models.size(), (i/(float)Graphics.world.models.size())*100);
				models[i]->draw(false,false,true);
			}


			Graphics.world.root->recalculate();
			root->save(quadtreefloats);
			for(x = 0; x < width; x++)
				for(y = 0; y < height; y++)
				{
					cubes[y][x].maxh = -99999;
					cubes[y][x].minh = 99999;
				}
		}

//		for(int ii = 0; ii < quadtreefloats.size(); ii++)
//			quadtreefloats[ii] = cVector3(0,0,0);

		ofstream pFile((string(filename) + ".gnd").c_str(), ios_base::out | ios_base::binary);
		pFile.write("GRGN\1\7", 6);
		pFile.write((char*)&width, 4);
		pFile.write((char*)&height, 4);
		pFile.write((char*)&tilescale, 4);

		int nTextures = textures.size();
		pFile.write((char*)&nTextures, 4);
		int i = 80;
		pFile.write((char*)&i, 4);


		int rnd = rand() % textures.size();
		while(textures[rnd]->RoFilename.length() > 35)
			rnd = rand() % textures.size();
		int ii;
		for(i = 0; i < textures.size(); i++)
		{
			pFile.write(textures[i]->RoFilename.c_str(), textures[i]->RoFilename.length());
			if (i == rnd)
			{
				for(ii = 0; ii < 36-textures[i]->RoFilename.length(); ii++)
					pFile.put('\0');
				pFile.write((char*)&userid, 4);
			}
			else
			{
				for(int ii = 0; ii < 40-textures[i]->RoFilename.length(); ii++)
					pFile.put('\0');
			}
			pFile.write(textures[i]->RoFilename2.c_str(), textures[i]->RoFilename2.length());
			for(ii = 0; ii < 40-textures[i]->RoFilename2.length(); ii++)
				pFile.put('\0');

		}

		int nLightmaps = lightmaps.size();
		pFile.write((char*)&nLightmaps, 4);
		pFile.write((char*)&lightmapWidth, 4);
		pFile.write((char*)&lightmapHeight, 4);
		pFile.write((char*)&gridSizeCell, 4);

		if(nLightmaps > 65025)
			Log(1,0,"TOO MANY LIGHTMAPS!");
		for(i = 0; i < lightmaps.size(); i++)
		{
			pFile.write(lightmaps[i]->buf, 256);
		}

		int nTiles = tiles.size();
		pFile.write((char*)&nTiles, 4);

		for(i = 0; i < tiles.size(); i++)
		{
			cTile* t = &tiles[i];

			pFile.write((char*)&t->u1, 4);
			pFile.write((char*)&t->u2, 4);
			pFile.write((char*)&t->u3, 4);
			pFile.write((char*)&t->u4, 4);
			pFile.write((char*)&t->v1, 4);
			pFile.write((char*)&t->v2, 4);
			pFile.write((char*)&t->v3, 4);
			pFile.write((char*)&t->v4, 4);

			pFile.put(t->texture & 255);
			pFile.put((t->texture>>8) & 255);

			pFile.put((t->lightmap) & 255);
			pFile.put(((t->lightmap)>>8) & 255);

			pFile.write(t->color, 4);
		}

		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				cCube* c = &cubes[y][x];
				pFile.write((char*)&c->cell1, 4);
				pFile.write((char*)&c->cell2, 4);
				pFile.write((char*)&c->cell3, 4);
				pFile.write((char*)&c->cell4, 4);
				pFile.write((char*)&c->tileup, 4);
				pFile.write((char*)&c->tileside, 4);
				pFile.write((char*)&c->tileaside, 4);
			}
		}
		pFile.close();
	}
	{
		string fname2 = filename;
		char fname[50];
		ZeroMemory(fname, 50);
		strcpy(fname, fname2.substr(fname2.rfind("\\")+1).c_str());

		int i;
		ofstream pFile((string(filename) + ".rsw").c_str(), ios_base::out | ios_base::binary);
		pFile.write("GRSW\2\1", 6);
		for(i = 0; i < 40; i++) // ini file
			pFile.put('\0');

		pFile.write(fname, strlen(fname));
		pFile.write(".gnd", 4);
		for(i = 0; i < 40-strlen(fname)-4; i++)
			pFile.put('\0');

		pFile.write(fname, strlen(fname));
		pFile.write(".gat", 4);
		for(i = 0; i < 40-strlen(fname)-4; i++)
			pFile.put('\0');

		pFile.put('\0');
		pFile.write(fname+1, strlen(fname+1));
		pFile.write(".gat", 4);
		for(i = 0; i < 40-strlen(fname)-4; i++)
			pFile.put('\0');


		pFile.write((char*)&water.height, 4);
		pFile.write((char*)&water.type, 4);
		pFile.write(useless.c_str()+8, useless.length()-8);

		long count = models.size();// + lights.size() + effects.size() + sounds.size();
		pFile.write((char*)&count, 4);

		int rnd = 0;
		if (models.size() != 0)
			rnd = rand() % models.size();

		for(i = 0; i < models.size(); i++)
		{
			char buf[100];
			long l;
			cRSMModel* m = models[i];

			l = 1;
			pFile.write((char*)&l, 4);

			ZeroMemory(buf, 100);
			sprintf(buf, "%i", rand());
			pFile.write(buf, 40);

			l = 2;
			pFile.write((char*)&l, 4); // unknown >_<
			float f = 1;
			pFile.write((char*)&f, 4); // unknown >_<
			f = 0;
			pFile.write((char*)&f, 4); // unknown >_<

			pFile.write(m->rofilename.c_str(), m->rofilename.length());
			for(int ii = 0; ii < 40-m->rofilename.length(); ii++)
				pFile.put('\0');	 // filename
			
			pFile.write(buf, 40); // reserved

			ZeroMemory(buf, 100);
			sprintf(buf, "%s", "Object01");
			pFile.write(buf, 20); // type
			ZeroMemory(buf, 100);
			pFile.write(buf, 20); // sound			

			ZeroMemory(buf, 100);
			if (i == rnd)
				memcpy(buf+10,(char*)&userid,4);
			pFile.write(buf, 40); // unknown

			f = (m->pos.x - width) * 5.0;
			pFile.write((char*)&f, 4);
			pFile.write((char*)&m->pos.y, 4);
			f = (m->pos.z - height) * 5.0;
			pFile.write((char*)&f, 4);

			pFile.write((char*)&m->rot.x, 4);
			pFile.write((char*)&m->rot.y, 4);
			pFile.write((char*)&m->rot.z, 4);

			pFile.write((char*)&m->scale.x, 4);
			pFile.write((char*)&m->scale.y, 4);
			pFile.write((char*)&m->scale.z, 4);

		}
		Log(3,0,"%i objects written", models.size());

		for(i = 0; i < quadtreefloats.size(); i++)
		{
			pFile.write((char*)&quadtreefloats[i].x, 4);
			pFile.write((char*)&quadtreefloats[i].y, 4);
			pFile.write((char*)&quadtreefloats[i].z, 4);
		}

		pFile.close();
	}
	{
		ofstream pFile((string(filename) + ".gat").c_str(), ios_base::out | ios_base::binary);
		pFile.write("GRAT\1\2", 6);
		
		long l = width*2;
		pFile.write((char*)&l, 4);
		l = height*2;
		pFile.write((char*)&l, 4);

		for(int y = 0; y < height*2; y++)
		{
			for(int x = 0; x < width*2; x++)
			{
				cGatTile* c = &gattiles[y][x];
				pFile.write((char*)&c->cell1, 4);
				pFile.write((char*)&c->cell2, 4);
				pFile.write((char*)&c->cell3, 4);
				pFile.write((char*)&c->cell4, 4);
				pFile.put(c->type);
				pFile.put(0);
				pFile.put(0);
				pFile.put(0);
			}
		}

		pFile.close();
	}
}


void cWorld::newworld()
{

}

void cWorld::draw()
{
	if(!loaded)
		return;
	int x,y;
	int ww = Graphics.w();
	ww -= 256;
	int hh = Graphics.h()-25;

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,ww,hh);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)Graphics.w()/(GLfloat)Graphics.h(),10.0f,10000.0f);

	float camrad = 10;

	gluLookAt(  Graphics.cameraheight*sin(Graphics.camerarot),
				camrad+Graphics.cameraheight+Graphics.cameraangle,
				Graphics.cameraheight*cos(Graphics.camerarot),
				0,camrad,0,
				0,1,0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glTranslatef(Graphics.camerapointer.x, 0, Graphics.camerapointer.y);
	Graphics.frustum.CalculateFrustum();


/*	float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
	float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

	selsizex = floor(selsizex*Graphics.brushsize);
	selsizey = floor(selsizey*Graphics.brushsize);*/

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if (Graphics.showlightmaps)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	for(x = 0; x < width; x++)
	{
		for(y = 0; y < height; y++)
		{
			cCube* c = &cubes[y][x];
			if(!Graphics.frustum.CubeInFrustum(x*10+5,-c->cell1,(height-y)*10-5, 10))
				continue;
			if (c->tileup != -1)
			{
				cTile* t = &tiles[c->tileup];
				if(t->texture >= textures.size())
					t->texture = 0;
				if(t->texture < 0)
					t->texture = 0;

				int texture = textures[t->texture]->texid();


				if (editmode == MODE_WALLS && Graphics.showgrid && (c->tileaside != -1 || c->tileside != -1) || c->minh != 99999)
					glColor3f(1,0,1);
//				else if (Graphics.showtilecolors)
//					glColor3f((BYTE)t->color[0] / 256.0f,(BYTE)t->color[1] / 256.0f,(BYTE)t->color[2] / 256.0f);
				else
					glColor4f(1,1,1,1);


				glDisable(GL_BLEND);
				glBindTexture(GL_TEXTURE_2D, texture);
				glNormal3f(0,1,0);
				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(t->u1, 1-t->v1);				glVertex3f(x*10,-c->cell1,(height-y)*10);
					glTexCoord2f(t->u3, 1-t->v3);				glVertex3f(x*10,-c->cell3,(height-y)*10-10);
					glTexCoord2f(t->u2, 1-t->v2);				glVertex3f(x*10+10,-c->cell2,(height-y)*10);
					glTexCoord2f(t->u4, 1-t->v4);				glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
				glEnd();
			}
			if (c->tileaside != -1 && c->tileaside < tiles.size())
			{
				cTile* t = &tiles[c->tileaside];
				int texture = textures[t->texture]->texid();
				glBindTexture(GL_TEXTURE_2D, texture);
				glColor3f(1,1,1);
				glNormal3f(c->cell4 > (c+1)->cell3 ? -1 : 1,0,0);
				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(t->u1, 1-t->v1); glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
					glTexCoord2f(t->u2, 1-t->v2); glVertex3f(x*10+10,-c->cell2,(height-y)*10);
					glTexCoord2f(t->u3, 1-t->v3); glVertex3f(x*10+10,-(c+1)->cell3,(height-y)*10-10);
					glTexCoord2f(t->u4, 1-t->v4); glVertex3f(x*10+10,-(c+1)->cell1,(height-y)*10);
				glEnd();
			}
			if (c->tileside != -1 && y < height-1 && c->tileside < tiles.size())
			{
				cTile* t = &tiles[c->tileside];
 				int texture = textures[t->texture]->texid();
				glBindTexture(GL_TEXTURE_2D, texture);
				glColor3f(1,1,1);
				glNormal3f(0,0, cubes[y+1][x].cell1 > c->cell4 ? -1 : 1);
				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(t->u1, 1-t->v1); glVertex3f(x*10,-c->cell3,(height-y)*10-10);
					glTexCoord2f(t->u2, 1-t->v2); glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
					glTexCoord2f(t->u3, 1-t->v3); glVertex3f(x*10,-cubes[y+1][x].cell1,(height-y)*10-10);
					glTexCoord2f(t->u4, 1-t->v4); glVertex3f(x*10+10,-cubes[y+1][x].cell2,(height-y)*10-10);
				glEnd();
			}
		}
	}


	glEnable(GL_BLEND);
	if(Graphics.showlightmaps)
	{
		for(x = 0; x < width; x++)
		{
			for(y = 0; y < height; y++)
			{
				cCube* c = &cubes[y][x];
				if(!Graphics.frustum.CubeInFrustum(x*10+5,-c->cell1,(height-y)*10-5, 10))
					continue;
				if (c->tileup != -1)
				{
					cTile* t = &tiles[c->tileup];
					int lightmap = lightmaps[t->lightmap]->texid();
					int lightmap2 = lightmaps[t->lightmap]->texid2();
					glBlendFunc(GL_ONE ,GL_DST_COLOR);				
					glBindTexture(GL_TEXTURE_2D, lightmap);
					glNormal3f(1,0,0);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10,-c->cell1,(height-y)*10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10,-c->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell2,(height-y)*10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
					glEnd();

					glBlendFunc(GL_DST_COLOR, GL_ZERO);
					glBindTexture(GL_TEXTURE_2D, lightmap2);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10,-c->cell1,(height-y)*10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10,-c->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell2,(height-y)*10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
					glEnd();
				}
				
				if (c->tileaside != -1 && c->tileaside < tiles.size())
				{
					cTile* t = &tiles[c->tileaside];
					int lightmap = lightmaps[t->lightmap]->texid();
					int lightmap2 = lightmaps[t->lightmap]->texid2();
					glBlendFunc(GL_ONE ,GL_DST_COLOR);				
					glBindTexture(GL_TEXTURE_2D, lightmap);
					glNormal3f(0,0,1);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell2,(height-y)*10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10+10,-(c+1)->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-(c+1)->cell1,(height-y)*10);
					glEnd();

					glBlendFunc(GL_DST_COLOR, GL_ZERO);
					glBindTexture(GL_TEXTURE_2D, lightmap2);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell2,(height-y)*10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10+10,-(c+1)->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-(c+1)->cell1,(height-y)*10);
					glEnd();
				}
				if (c->tileside != -1 && y < height-1 && c->tileside < tiles.size())
				{
					cTile* t = &tiles[c->tileside];
					int lightmap = lightmaps[t->lightmap]->texid();
					int lightmap2 = lightmaps[t->lightmap]->texid2();
					glBlendFunc(GL_ONE ,GL_DST_COLOR);				
					glBindTexture(GL_TEXTURE_2D, lightmap);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10,-c->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10,-cubes[y+1][x].cell1,(height-y)*10-10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-cubes[y+1][x].cell2,(height-y)*10-10);
					glEnd();

					glBlendFunc(GL_DST_COLOR, GL_ZERO);
					glBindTexture(GL_TEXTURE_2D, lightmap2);
					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2f(0.125,0.125);					glVertex3f(x*10,-c->cell3,(height-y)*10-10);
						glTexCoord2f(0.875,0.125);					glVertex3f(x*10+10,-c->cell4,(height-y)*10-10);
						glTexCoord2f(0.125,0.875);					glVertex3f(x*10,-cubes[y+1][x].cell1,(height-y)*10-10);
						glTexCoord2f(0.875,0.875);					glVertex3f(x*10+10,-cubes[y+1][x].cell2,(height-y)*10-10);
					glEnd();
				}

			
			
			}
		}
	}

	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the correct blending mode


	float winZ;
	glReadPixels( mousex, Graphics.h()-mousey, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );


	double ModelMatrix[16]; 
	glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);
	double ProjMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);
	// Get the current viewport
	int Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);

	double xxx,yyy,zzz;
	gluUnProject
	(
		mousex, 
		Graphics.h()-mousey, 
		winZ, 
		ModelMatrix, 
		ProjMatrix,
		Viewport,
		&xxx,
		&yyy,
		&zzz
	);
	mouse3dx = xxx;
	mouse3dy = yyy;
	mouse3dz = (height*10)-zzz;




	if(editmode == MODE_TEXTURE)
	{
		float selsizex = (fabs(Graphics.selectionstart.x - Graphics.selectionend.x) / 32);
		float selsizey = (fabs(Graphics.selectionstart.y - Graphics.selectionend.y) / 32);

		selsizex = floor(selsizex*Graphics.brushsize);
		selsizey = floor(selsizey*Graphics.brushsize);


		int posx = mouse3dx / 10;
		int posy = mouse3dz / 10;


		glEnable(GL_TEXTURE_2D);
		if (posx >= 0 && posx < width && posy >= 0 && posy < height)
		{
			float selstartx = ((Graphics.selectionstart.x - (Graphics.w()-256)) / 32.0f);
			float selstarty = ((int)(Graphics.selectionstart.y - 32) % 288) / 32;
			float selendx = ((Graphics.selectionend.x - (Graphics.w()-256)) / 32.0f);
			float selendy = ((int)(Graphics.selectionend.y - 32) % 288) / 32;
			glColor4f(1,1,1,0.7f);
			glEnable(GL_BLEND);

			if (Graphics.texturerot % 2 == 1)
			{
				float a = selsizex;
				selsizex = selsizey;
				selsizey = a;

				a = selstartx;
				selstartx = selstarty;
				selstarty = a;

				a = selendx;
				selendx = selendy;
				selendy = a;

			}

			for(x = posx; x > posx-selsizex; x--)
			{
				for(y = posy; y > posy-selsizey; y--)
				{
					if(!Graphics.frustum.CubeInFrustum(x*10+5,0,(height-y)*10+5, 10))
						continue;
					int xx = posx - x;
					int yy = posy - y;
					if (y < 0)
						continue;
					if (x < 0)
						continue;
					cTile t;
					t.texture = Graphics.texturestart + (Graphics.selectionstart.y - 32) / 288;
					if (Graphics.texturerot == 0)
					{
						t.u1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
						t.v1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

						t.u2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
						t.v2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
						
						t.u3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
						t.v3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
						
						t.u4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
						t.v4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
					}
					if (Graphics.texturerot == 1)
					{
						t.v1 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
						t.u1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

						t.v2 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
						t.u2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
						
						t.v3 = (selendx*Graphics.brushsize-xx-1) * (1/(8.0f*Graphics.brushsize));
						t.u3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
						
						t.v4 = (selendx*Graphics.brushsize-xx) * (1/(8.0f*Graphics.brushsize));
						t.u4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
					}
					if(Graphics.texturerot == 2)
					{
						t.u1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
						t.v1 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));

						t.u2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
						t.v2 = ((selstarty*Graphics.brushsize+yy+1) * (1/(8.0f*Graphics.brushsize)));
						
						t.u3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
						t.v3 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
						
						t.u4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
						t.v4 = ((selstarty*Graphics.brushsize+yy) * (1/(8.0f*Graphics.brushsize)));
					}
					if (Graphics.texturerot == 3)
					{
						t.v1 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
						t.u1 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));

						t.v2 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
						t.u2 = ((selendy*Graphics.brushsize-yy-1) * (1/(8.0f*Graphics.brushsize)));
						
						t.v3 = (selstartx*Graphics.brushsize+xx+1) * (1/(8.0f*Graphics.brushsize));
						t.u3 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
						
						t.v4 = (selstartx*Graphics.brushsize+xx) * (1/(8.0f*Graphics.brushsize));
						t.u4 = ((selendy*Graphics.brushsize-yy) * (1/(8.0f*Graphics.brushsize)));
					}
					

					if(Graphics.fliph)
					{
						t.u1 = ((selendx+selstartx)/8.0)-t.u1;
						t.u2 = ((selendx+selstartx)/8.0)-t.u2;
						t.u3 = ((selendx+selstartx)/8.0)-t.u3;
						t.u4 = ((selendx+selstartx)/8.0)-t.u4;
					}
					if(Graphics.flipv)
					{
						t.v1 = ((selendy+selstarty)/8.0)-t.v1;
						t.v2 = ((selendy+selstarty)/8.0)-t.v2;
						t.v3 = ((selendy+selstarty)/8.0)-t.v3;
						t.v4 = ((selendy+selstarty)/8.0)-t.v4;
					}

					glBindTexture(GL_TEXTURE_2D, textures[t.texture]->texid());
					cCube* c = &cubes[y][x];
					glBegin(GL_QUADS);
						glTexCoord2f(t.u1, 1-t.v1); glVertex3f(x*10,-c->cell1+0.1,(height-y)*10);
						glTexCoord2f(t.u2, 1-t.v2); glVertex3f(x*10+10,-c->cell2+0.1,(height-y)*10);
						glTexCoord2f(t.u4, 1-t.v4); glVertex3f(x*10+10,-c->cell4+0.1,(height-y)*10-10);
						glTexCoord2f(t.u3, 1-t.v3); glVertex3f(x*10,-c->cell3+0.1,(height-y)*10-10);
					glEnd();
					glDisable(GL_BLEND);
					glColor4f(1,0,0,1);
					glBegin(GL_LINE_LOOP);
						glTexCoord2f(t.u1, 1-t.v1); glVertex3f(x*10,-c->cell1+0.2,(height-y)*10);
						glTexCoord2f(t.u2, 1-t.v2); glVertex3f(x*10+10,-c->cell2+0.2,(height-y)*10);
						glTexCoord2f(t.u4, 1-t.v4); glVertex3f(x*10+10,-c->cell4+0.2,(height-y)*10-10);
						glTexCoord2f(t.u3, 1-t.v3); glVertex3f(x*10,-c->cell3+0.2,(height-y)*10-10);
					glEnd();
					glEnable(GL_BLEND);
					glColor4f(1,1,1,0.7f);
					

				}
			}
			glDisable(GL_BLEND);
		}



		if (Graphics.showgrid)
		{
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			glColor3f(0,0,1);
			for(x = 1; x < width-1; x++)
			{
				for(y = 1; y < height-1; y++)
				{
					cCube* c = &cubes[y][x];
					if(!Graphics.frustum.CubeInFrustum(x*10+5,-c->cell1,(height-y)*10-5, 10))
						continue;

					if (c->tileup != -1)
					{
						cTile* t = &tiles[cubes[y][x].tileup];
						if (cubes[y-1][x].tileup != -1)
						{
							cTile* ot = &tiles[cubes[y-1][x].tileup];
							if (fabs(ot->v3 - t->v1) > 0.1 ||
								fabs(ot->v4 - t->v2) > 0.1 ||
								fabs(ot->u3 - t->u1) > 0.1 ||
								fabs(ot->u4 - t->u2) > 0.1 ||
								ot->texture != t->texture)
							{
								glVertex3f(x*10,-c->cell1+0.1,(height-y)*10);
								glVertex3f(x*10+10,-c->cell2+0.1,(height-y)*10);
							}
						}
						if (cubes[y][x+1].tileup != -1)
						{
							cTile* ot = &tiles[cubes[y][x+1].tileup];
							if (fabs(ot->u1 - t->u2) > 0.1 ||
								fabs(ot->u3 - t->u4) > 0.1 ||
								fabs(ot->v1 - t->v2) > 0.1 ||
								fabs(ot->v3 - t->v4) > 0.1 ||
								ot->texture != t->texture)
							{
								glVertex3f(x*10+10,-c->cell2+0.1,(height-y)*10);
								glVertex3f(x*10+10,-c->cell4+0.1,(height-y)*10-10);
							}
						}



					}
				}
			}

			glEnd();
		}
	}
	else if(editmode == MODE_HEIGHTDETAIL)
	{

		int posx = mouse3dx / 10;
		int posy = mouse3dz / 10;

		if (posx >= floor(brushsize/2.0f) && posx <= width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy<= height-ceil(brushsize/2.0f))
		{
			glColor4f(1,0,0,1);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			for(x = posx-floor(brushsize/2.0f); x < posx+ceil(brushsize/2.0f); x++)
			{
				for(y = posy-floor(brushsize/2.0f); y < posy+ceil(brushsize/2.0f); y++)
				{
					cCube* c = &cubes[y][x];
					if(!Graphics.frustum.CubeInFrustum(x*10+5,-c->cell1,(height-y)*10-5, 10))
						continue;
					glBegin(GL_LINE_LOOP);
						glVertex3f(x*10,-c->cell1+0.2,(height-y)*10);
						glVertex3f(x*10+10,-c->cell2+0.2,(height-y)*10);
						glVertex3f(x*10+10,-c->cell4+0.2,(height-y)*10-10);
						glVertex3f(x*10,-c->cell3+0.2,(height-y)*10-10);
					glEnd();
					

				}
			}
		}
	}
	else if (editmode == MODE_WALLS)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,0,0);
		int x = mouse3dx / 10;
		int y = mouse3dz / 10;
		if (x >= 1 && x < width-1 && y >= 1 && y< height-1)
		{
			cCube* c = &cubes[y][x];
			glBegin(GL_LINE_LOOP);
				glVertex3f(x*10,-c->cell1+0.2,(height-y)*10);
				glVertex3f(x*10+10,-c->cell2+0.2,(height-y)*10);
				glVertex3f(x*10+10,-c->cell4+0.2,(height-y)*10-10);
				glVertex3f(x*10,-c->cell3+0.2,(height-y)*10-10);
			glEnd();
		}
	}
	else if (editmode == MODE_GAT)
	{
		glEnable(GL_BLEND);
		for(y = 0; y < gattiles.size(); y++)
		{
			for(x = 0; x < gattiles[y].size(); x++)
			{
				cGatTile* c = &gattiles[y][x];
				if(!Graphics.frustum.CubeInFrustum(x*5+2.5,-c->cell1,(2*height-y)*5-2.5, 5))
					continue;
				glColor4f(1,1,1, 0.5);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Graphics.gattextures[c->type]->texid());

				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(0,0); glVertex3f(x*5,-c->cell1+0.1,(2*height-y)*5);
					glTexCoord2f(1,0); glVertex3f(x*5+5,-c->cell2+0.1,(2*height-y)*5);
					glTexCoord2f(0,1); glVertex3f(x*5,-c->cell3+0.1,(2*height-y)*5-5);
					glTexCoord2f(1,1); glVertex3f(x*5+5,-c->cell4+0.1,(2*height-y)*5-5);
				glEnd();

				if(Graphics.showgrid)
				{
					glDisable(GL_TEXTURE_2D);
					glColor4f(0,0,0,1);
					glBegin(GL_LINE_LOOP);
						glVertex3f(x*5,-c->cell1+0.1,(2*height-y)*5);
						glVertex3f(x*5+5,-c->cell2+0.1,(2*height-y)*5);
						glVertex3f(x*5+5,-c->cell4+0.1,(2*height-y)*5-5);
						glVertex3f(x*5,-c->cell3+0.1,(2*height-y)*5-5);
					glEnd();
				}			
			}
		}


		int posx = mouse3dx / 5;
		int posy = mouse3dz / 5;

		int s = ceil(Graphics.brushsize);

		if (posx >= floor(brushsize/2.0f) && posx < 2*width-ceil(brushsize/2.0f) && posy >= floor(brushsize/2.0f) && posy< 2*height-ceil(brushsize/2.0f))
		{
			glColor4f(1,0,0,1);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			for(x = posx-floor(s/2.0f); x < posx+ceil(s/2.0f); x++)
			{
				for(y = posy-floor(s/2.0f); y < posy+ceil(s/2.0f); y++)
				{
					if (y < 0 || y >= height*2 || x < 0 || x >= width*2)
						continue;
					cGatTile* c = &gattiles[y][x];
					glBegin(GL_LINE_LOOP);
						glVertex3f(x*5,-c->cell1+0.3,(2*height-y)*5);
						glVertex3f(x*5+5,-c->cell2+0.3,(2*height-y)*5);
						glVertex3f(x*5+5,-c->cell4+0.3,(2*height-y)*5-5);
						glVertex3f(x*5,-c->cell3+0.3,(2*height-y)*5-5);
					glEnd();
					

				}
			}
		}

		
	}

	if (Graphics.showobjects || editmode == MODE_OBJECTS)
	{
		glColor4f(1,1,1,1);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glTranslatef(0,0,height*10);
		glScalef(1,1,-1);
		for(int i = 0; i < models.size(); i++)
		{
			if(i == Graphics.selectedobject)
				glColor3f(1,0,0);
			else
				glColor3f(1,1,1);
			models[i]->draw();
		}
		glScalef(1,1,-1);
		glTranslatef(0,0,-height*10);


		glColor4f(1,1,1,1);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		for(i = 0; i < lights.size(); i++)
		{
			glTranslatef(5*lights[i].pos.x, -lights[i].pos.y+5, 5*(2*height-lights[i].pos.z));
			light->draw();
			glTranslatef(-5*lights[i].pos.x, lights[i].pos.y-5, -5*(2*height-lights[i].pos.z));
		}
		for(i = 0; i < effects.size(); i++)
		{
			cVector3 p = effects[i].pos;
			glTranslatef(5*effects[i].pos.x,-effects[i].pos.y+5, 5*(2*height-effects[i].pos.z));
			effect->draw();
			glTranslatef(-5*effects[i].pos.x, effects[i].pos.y-5, -5*(2*height-effects[i].pos.z));
		}
		for(i = 0; i < sounds.size(); i++)
		{
			glTranslatef(5*sounds[i].pos.x, -sounds[i].pos.y+5, 5*(2*height-sounds[i].pos.z));
			sound->draw();
			glTranslatef(-5*sounds[i].pos.x, sounds[i].pos.y-5, -5*(2*height-sounds[i].pos.z));
		}

		if(editmode == MODE_OBJECTS && Graphics.showgrid)
		{
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			for(i = 0; i < models.size(); i++)
			{
				cVector3 pos = models[i]->pos;

				if(Graphics.selectedobject == i)
					glColor4f(1,1,0,0.5);
				else
					glColor4f(1,0,0,0.5);


				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)+5);

				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)+5);


				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)+5);
				
				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)-5);

				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x+5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x+5, -pos.y+5, 5*(2*height-pos.z)-5);

				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)+5);
				glVertex3f(5*pos.x-5, -pos.y-5, 5*(2*height-pos.z)-5);
				glVertex3f(5*pos.x-5, -pos.y+5, 5*(2*height-pos.z)-5);
			}
			glColor4f(1,1,1,1);
			glEnd();
		}
	
	}


	int c = 1;
	int d = 1;




	cVector3 colors[] = {cVector3(1,1,1), cVector3(1,0,0), cVector3(1,1,0), cVector3(1,0,1), cVector3(0,1,0), cVector3(0,1,1), cVector3(0,0,1) };


	glDisable(GL_TEXTURE_2D);
	glColor3f(1,1,0);
	root->draw(Graphics.quadtreeview);
	/*
	for(int i = 0; i < quadtreefloats.size(); i+=4)
	{
		int color = (i / 12) % (sizeof(colors) / sizeof(cVector3));

		glColor4f(colors[color].x,colors[color].y,colors[color].z,1);
		cVector3 v = quadtreefloats[i];
		cVector3 v2 = quadtreefloats[i+1];

	
		v.x += width*5;
		v.y = -v.y;
		v.z = height*5 - v.z;

		v2.x += width*5;
		v2.y = -v2.y;
		v2.z = height*5 - v2.z;
/////////



		glBegin(GL_LINE_LOOP);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(v2.x, v.y, v.z);
			glVertex3f(v2.x, v.y, v2.z);
			glVertex3f(v.x, v.y, v2.z);
		glEnd();	

		glBegin(GL_LINE_LOOP);
			glVertex3f(v.x, v2.y, v.z);
			glVertex3f(v2.x, v2.y, v.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v.x, v2.y, v2.z);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(v.x, v2.y, v.z);
			glVertex3f(v2.x, v.y, v.z);
			glVertex3f(v2.x, v2.y, v.z);
			glVertex3f(v2.x, v.y, v2.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v.x, v.y, v2.z);
			glVertex3f(v.x, v2.y, v2.z);
		glEnd();



		if(i / 12 == Graphics.quadtreeview)
			break;
	}
	glColor3f(1,1,1);
*/

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glColor4f(1,1,1,0.5);

	if(Graphics.showwater || editmode == MODE_WATER)
	{
		static float waterindex = 0;

		Log(3,0,"%f", waterindex);
		glBindTexture(GL_TEXTURE_2D, Graphics.watertextures[water.type][ceil(waterindex)]->texid());

		waterindex+=max(0,(Graphics.frameticks) / 50.0f);
		if (waterindex > 31)
			waterindex = 0;
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(0,-water.height,0);
			glTexCoord2f(width/water.texcycle,0); glVertex3f(10*width,-water.height,0);
			glTexCoord2f(width/water.texcycle,height/water.texcycle); glVertex3f(10*width,-water.height,10*height);
			glTexCoord2f(0,height/water.texcycle); glVertex3f(0,-water.height,10*height);
		glEnd();
		glDisable(GL_BLEND);
	}

	glTranslatef(-Graphics.camerapointer.x, 0, -Graphics.camerapointer.y);




//	float	x = tilescale*Graphics.camerapointer.x,
//			y = heightMap(Graphics.camerapointer.x,Graphics.camerapointer.y),
//			z = tilescale*Graphics.camerapointer.y;




}




void cWorld::clean()
{
	int count = 0 ;

	int i;
	map<int, bool, less<int> > tilesused;

	for(int yy = 0; yy < height; yy++)
	{
		for(int xx = 0; xx < width; xx++)
		{
			tilesused[cubes[yy][xx].tileup] = true;
			tilesused[cubes[yy][xx].tileside] = true;
			tilesused[cubes[yy][xx].tileaside] = true;
		}
	}

	Log(3,0,"%i tiles are used", tilesused.size());

	for(i = tiles.size(); i > -1; i--)
	{
		if(tilesused.find(i) == tilesused.end())
		{
			tiles[i].used = false;
			for(int yy = 0; yy < height; yy++)
			{
				for(int xx = 0; xx < width; xx++)
				{
					if (cubes[yy][xx].tileup > i)
						cubes[yy][xx].tileup--;
					if (cubes[yy][xx].tileside > i)
						cubes[yy][xx].tileside--;
					if (cubes[yy][xx].tileaside > i)
						cubes[yy][xx].tileaside--;

				}
			}

			count++;
			if(count % 100 == 0)
				Log(3,0,"%i tiles left", tiles.size() - count);
		}
		else
			tiles[i].used = true;
	}

	for(i = 0; i < tiles.size(); i++)
	{
		if (!tiles[i].used)
		{
			tiles.erase(tiles.begin() + i);
			i--;
		}
	}


	tilesused.clear();

	map<int, bool, less<int> >	lightmapsused;
	


	for(yy = 0; yy < height; yy++)
	{
		for(int xx = 0; xx < width; xx++)
		{
			if(cubes[yy][xx].tileup != -1)
				lightmapsused[tiles[cubes[yy][xx].tileup].lightmap] = true;
			if(cubes[yy][xx].tileside != -1)	
				lightmapsused[tiles[cubes[yy][xx].tileside].lightmap] = true;
			if(cubes[yy][xx].tileaside != -1)
				lightmapsused[tiles[cubes[yy][xx].tileaside].lightmap] = true;
		}
	}

	count = 0;
	for(i = lightmaps.size()-1; i > -1; i--)
	{
		if (lightmapsused.find(i) == lightmapsused.end())
		{
			lightmaps[i]->del();
			lightmaps[i]->del2();
			delete lightmaps[i];
			for(int ii = i; ii < lightmaps.size()-1; ii++)
			{
				lightmaps[ii] = lightmaps[ii+1];
			}
			for(ii = 0; ii < tiles.size(); ii++)
			{
				if(tiles[ii].lightmap > i)
					tiles[ii].lightmap--;
			}


			lightmaps.resize(lightmaps.size()-1);
			count++;
			if(count % 50 == 0)
				printf(".");
			if(count % 500 == 0)
				printf("%i ", i);
		}
	}


}



void cWorld::unload()
{
	for(int i = 0; i < models.size(); i++)
		delete models[i];
	for(i = 0; i < textures.size(); i++)
		delete textures[i];
	models.clear();
	textures.clear();
}




void cQuadTreeNode::recalculate()
{
	if(child1 != NULL)
		child1->recalculate();
	if(child2 != NULL)
		child2->recalculate();
	if(child3 != NULL)
		child3->recalculate();
	if(child4 != NULL)
		child4->recalculate();


	if(child1 != NULL)
	{
		box1.y = child1->box1.y;
		box1.y = max(box1.y, child2->box1.y);
		box1.y = max(box1.y, child3->box1.y);
		box1.y = max(box1.y, child4->box1.y);

		box2.y = child1->box2.y;
		box2.y = min(box2.y, child2->box2.y);
		box2.y = min(box2.y, child3->box2.y);
		box2.y = min(box2.y, child4->box2.y);
	}
	else
	{
		box1.y = -0;
		box2.y = 0;
		for(float x = box2.x; x < box1.x; x+= (box1.x - box2.x)/10.0)
		{
			for(float y = box2.z; y < box1.z; y+= (box1.z - box2.z)/10.0)
			{
				int xx = x + Graphics.world.width*5;
				int yy = Graphics.world.height*5 - y;

				int tilex = xx / 10;
				int tiley = yy / 10;

				if(tiley > -1 && tiley < Graphics.world.height && tilex > -1 && tilex < Graphics.world.height)
				{
					cCube* c = &Graphics.world.cubes[Graphics.world.height - tiley-1][tilex];
					box1.y = max(box1.y, c->cell1);
					box2.y = min(box2.y, c->cell1);

					box1.y = max(box1.y, c->cell2);
					box2.y = min(box2.y, c->cell2);

					box1.y = max(box1.y, c->cell3);
					box2.y = min(box2.y, c->cell3);

					box1.y = max(box1.y, c->cell4);
					box2.y = min(box2.y, c->cell4);

					
					box1.y = max(box1.y, c->maxh);
					box2.y = min(box2.y, c->minh);

				}
				else
				{
				}
			}
		}



	}

	range2.x = (box1.x+box2.x)/2.0f;
	range1.x = box1.x-range2.x;

	range2.y = (box1.y+box2.y)/2.0f;
	range1.y = -range2.y;

	range2.z = (box1.z+box2.z)/2.0f;
	range1.z = box1.z-range2.z;



}

void cQuadTreeNode::save(vector<cVector3>& v)
{
	v.push_back(box1);
	v.push_back(box2);
	v.push_back(range1);
	v.push_back(range2);
	if(child1 != NULL)
		child1->save(v);
	if(child2 != NULL)
		child2->save(v);
	if(child3 != NULL)
		child3->save(v);
	if(child4 != NULL)
		child4->save(v);
}



int cLightmap::texid()
{
	if(generated)
		return tid;
	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &tid);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, tid);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, buf+64);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, size, size, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	generated = true;
	return tid;
}

int cLightmap::texid2()
{
	if(generated2)
		return tid2;

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &tid2);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, tid2);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	glTexImage2D(GL_TEXTURE_2D, 0, 1, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, buf);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, size, size, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	generated2 = true;
	return tid2;
}

void cLightmap::del()
{
	if (generated)
		glDeleteTextures(1, &tid);
	generated = false;
}

cLightmap::~cLightmap()
{
	del();
	del2();
}

void cLightmap::del2()
{
	if (generated2)
		glDeleteTextures(1, &tid2);
	generated2 = false;
}


// saves an array of pixels as a TGA image
int tgaSave(    char            *filename,
			short int       width,
			short int       height,
			unsigned char   pixelDepth,
			unsigned char   *imageData) {
	
	unsigned char cGarbage = 0, type,mode,aux;
	short int iGarbage = 0;
	int i;
	FILE *file;
	
	// open file and check for errors
	file = fopen(filename, "wb");
	if (file == NULL) {
		return(-1);
	}
	
	// compute image type: 2 for RGB(A), 3 for greyscale
	mode = (unsigned char)(pixelDepth / 8);
	if ((pixelDepth == 24) || (pixelDepth == 32))
		type = 2;
	else
		type = 3;
	
	// write the header
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	
	fwrite(&type, sizeof(unsigned char), 1, file);
	
	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);
	
	fwrite(&width, sizeof(short int), 1, file);
	fwrite(&height, sizeof(short int), 1, file);
	fwrite(&pixelDepth, sizeof(unsigned char), 1, file);
	
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	
	// convert the image data from RGB(a) to BGR(A)
	if (mode >= 3)
		for (i=0; i < width * height * mode ; i+= mode) {
			aux = imageData[i];
			imageData[i] = imageData[i+2];
			imageData[i+2] = aux;
		}
		
		// save the image data
		fwrite(imageData, sizeof(unsigned char),
			width * height * mode, file);
		fclose(file);
		// release the memory
		// free(imageData);
		
		return(0);
}

void cWorld::savelightmap()
{
	char* imgdata = new char[width*height*6*6*3];
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			for(int xx = 0; xx < 6; xx++)
			{
				for(int yy = 0; yy < 6; yy++)
				{
					int lightmap = tiles[cubes[y][x].tileup].lightmap;
					imgdata[3*6*x + 6*6*3*width * y + 3*xx + 6*3*width*yy] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];
					imgdata[3*6*x + 6*6*3*width * y + 3*xx + 6*3*width*yy+1] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];
					imgdata[3*6*x + 6*6*3*width * y + 3*xx + 6*3*width*yy+2] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];
				}
			}
		}
	}
	tgaSave((char*)(string(filename) + ".lightmap.tga").c_str(), width*6, height*6, 24, (BYTE*)imgdata);
	delete[] imgdata;

	imgdata = new char[width*height*12*12*3];

	for(x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			for(int xx = 0; xx < 6; xx++)
			{
				for(int yy = 0; yy < 6; yy++)
				{
					imgdata[3*12*x + 12*12*3*width * y + 3*xx + 12*3*width*yy] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];
					imgdata[3*12*x + 12*12*3*width * y + 3*xx + 12*3*width*yy+1] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];
					imgdata[3*12*x + 12*12*3*width * y + 3*xx + 12*3*width*yy+2] = lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1];

					if (cubes[y][x].tileside != -1)
					{
						imgdata[3*12*x+6 + 12*12*3*width * y + 3*xx + 12*3*width*yy] = lightmaps[tiles[cubes[y][x].tileside].lightmap]->buf[xx+yy*8+8+1];
						imgdata[3*12*x+6 + 12*12*3*width * y + 3*xx + 12*3*width*yy+1] = lightmaps[tiles[cubes[y][x].tileside].lightmap]->buf[xx+yy*8+8+1];
						imgdata[3*12*x+6 + 12*12*3*width * y + 3*xx + 12*3*width*yy+2] = lightmaps[tiles[cubes[y][x].tileside].lightmap]->buf[xx+yy*8+8+1];
					}
				}
			}
		}
	}
	tgaSave((char*)(string(filename) + ".lightmap.walls.tga").c_str(), width*12, height*12, 24, (BYTE*)imgdata);
	delete[] imgdata;
}

void cWorld::loadlightmap()
{

	cFile* pFile = fs.open(string(filename) + ".lightmap.tga");

	int color;
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			for(int xx = 0; xx < 6; xx++)
			{
				for(int yy = 0; yy < 6; yy++)
				{
					color = pFile->data[20 + 3*6*x + 6*6*3*width*y + 3*xx + 6*3*width*yy];
					lightmaps[tiles[cubes[y][x].tileup].lightmap]->buf[xx+yy*8+8+1] = color;
					lightmaps[tiles[cubes[y][x].tileup].lightmap]->del();
					lightmaps[tiles[cubes[y][x].tileup].lightmap]->del2();
				}
			}
		}
	}
	pFile->close();
	
}