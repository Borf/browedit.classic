#ifndef __RSMMODEL_H__
#define __RSMMODEL_H__

#include <plugins/base/interface.h>
#include <common.h>
#include <string>
#include <map>

class cTexture;
class cFile;


class cRsmModelBase
{
public:
	class cMesh
	{
	public:
		cMesh(cFile*, cRsmModelBase*, int ver1, int ver2);
		void fetchChildren(std::map<std::string, cMesh*, std::less<std::string> >&);
		void draw();
		cMatrix4x4 getMatrix1();
		cMatrix4x4 matrix1Cache;
		bool		cache1;
		cMatrix4x4 getMatrix2();
		cMatrix4x4 matrix2Cache;
		bool		cache2;
		void setBoundingBox(cVector3&, cVector3&);
		void setBoundingBox2(cMatrix4x4 &mat, cVector3 &bbmin, cVector3 &bbmax);
		virtual bool collides(cMatrix4x4 &mat, cVector3 from, cVector3 to, cVector3* = NULL);

		std::string name;
		std::string parentName;
		int unknown1;
		float unknown2;

		cMatrix4x4	offset;
		cVector3	pos_;
		cVector3	pos;
		float		rotangle;
		cVector3	rotaxis;
		cVector3	scale;

		std::vector<cTexture*> textures;
		int			nVertices;
		cVector3*	vertices;
		int			nTexVertices;
		cVector2*	texVertices;
		int			nFaces;
		class cFace
		{
		public:
			int			vertices[3];
			int			texvertices[3];
			cVector3	normal;
			int			texIndex;
			int			twoSide;
			int			smoothGroup;
		};
		cFace*		faces;
		int			nAnimationFrames;
		long		lastTick;
		class cFrame
		{
		public:
			int			time;
			cQuaternion	quat;
		};
		cFrame*		animationFrames;
		
		
		cRsmModelBase*		base;
		cMesh* parent;
		std::vector<cMesh*> children;
		cVector3 bbmin;
		cVector3 bbmax;
		cVector3 bbrange;
	};

	std::string filename;
	std::string rofilename;
	std::vector<cTexture*>	textures;
	cVector3 realbbmin;
	cVector3 realbbmax;
	cVector3 realbbrange;
	float		maxrange;
	
	cVector3 bbmin;
	cVector3 bbmax;
	cVector3 bbrange;
	cMesh* root;

	enum eShadeType
	{
		SHADE_NO,
		SHADE_FLAT,
		SHADE_SMOOTH,
		SHADE_BLACK,
	} shadeType;


	cRsmModelBase(std::string);
	virtual ~cRsmModelBase();
	void draw();
	virtual bool collides(cMatrix4x4 &mat, cVector3 from, cVector3 to, cVector3* = NULL);
};

class cRsmModel : public cRsmModelBase, public cBrowInterface::cPluginRSMModel
{
public:
	std::string name;
	float lightopacity;
	bool selected;

	cRsmModel(std::string pFilename) : cRsmModelBase(pFilename)
	{
		filename = pFilename;
		rofilename = filename.substr(filename.find("model\\") + 6);
	}


	void draw();
	bool collides(cVector3 from, cVector3 to, cVector3* = NULL);
	void setHeight();
};




#endif
