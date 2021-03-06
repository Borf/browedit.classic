#ifndef __RSMMODEL_H__
#define __RSMMODEL_H__

#include <plugins/base/interface.h>
#include <common.h>
#include <string>
#include <map>
#include <bengine/util/filesystem.h>
#include <bengine/math/matrix4x4.h>
#include <bengine/math/quaternion.h>
#include <bengine/util/serializabledata.h>

class cRsmModelBase : public bEngine::util::cSerializable
{
public:
	class cMesh : public bEngine::util::cSerializable
	{
	public:
		cMesh(cRsmModelBase*, int ver1, int ver2);
		bEngine::util::cInStream& readData(bEngine::util::cInStream &instream);
		bEngine::util::cOutStream& writeData(bEngine::util::cOutStream &outstream);
		void save();
		void fetchChildren(std::map<std::string, cMesh*, std::less<std::string> >&);
		void draw();
		bEngine::math::cMatrix4x4 getMatrix1(bool animate = true);
		bEngine::math::cMatrix4x4 matrix1Cache;
		bool		cache1;
		bEngine::math::cMatrix4x4 getMatrix2();
		bEngine::math::cMatrix4x4 matrix2Cache;
		bool		cache2;
		void setBoundingBox(bEngine::math::cVector3&, bEngine::math::cVector3&);
		void setBoundingBox2(bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 &bbmin, bEngine::math::cVector3 &bbmax);
		virtual bool collides(bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* = NULL);
		void setTexture(bEngine::cTexture* oldTexture, bEngine::cTexture* newTexture);
		void setSelection(cMesh* mesh);

		cRsmModelBase* model;

		std::string						name;
		std::string						parentName;
		int								unknown1;
		float							unknown2;

		bEngine::math::cMatrix4x4		offset;
		bEngine::math::cVector3			pos_;
		bEngine::math::cVector3			pos;
		float							rotangle;
		bEngine::math::cVector3			rotaxis;
		bEngine::math::cVector3			scale;

		std::vector<bEngine::cTexture*> textures;
		int								nVertices;
		bEngine::math::cVector3*		vertices;
		int								nTexVertices;
		bEngine::math::cVector2*		texVertices;
		int								nFaces;

		class cFace : public bEngine::util::cSerializable
		{
		public:
			bEngine::util::cInStream& readData(bEngine::util::cInStream &instream);
			bEngine::util::cOutStream& writeData(bEngine::util::cOutStream &outstream);
			int							vertices[3];
			int							texvertices[3];
			bEngine::math::cVector3		normal;
			int							texIndex;
			int							twoSide;
			int							smoothGroup;
		};
		cFace*							faces;
		int								nAnimationFrames;
		long							lastTick;
		class cFrame : public bEngine::util::cSerializable
		{
		public:
			bEngine::util::cInStream& readData(bEngine::util::cInStream &instream);
			bEngine::util::cOutStream& writeData(bEngine::util::cOutStream &outstream);
			cFrame();
			int							time;
			bEngine::math::cQuaternion	quat;
		};
		cFrame*							animationFrames;
		
		
		bool selected;
		cRsmModelBase*		base;
		cMesh* parent;
		std::vector<cMesh*> children;
		bEngine::math::cVector3 bbmin;
		bEngine::math::cVector3 bbmax;
		bEngine::math::cVector3 bbrange;
	};

	cBrowInterface::cRoString<80> filename;
	std::vector<bEngine::cTexture*>	textures;
	bEngine::math::cVector3 realbbmin;
	bEngine::math::cVector3 realbbmax;
	bEngine::math::cVector3 realbbrange;
	float		maxrange;
	
	bEngine::math::cVector3 bbmin;
	bEngine::math::cVector3 bbmax;
	bEngine::math::cVector3 bbrange;
	cMesh* root;

	enum eShadeType
	{
		SHADE_NO,
		SHADE_FLAT,
		SHADE_SMOOTH,
		SHADE_BLACK,
	}								shadeType;

	char							unknown[16];
	char							alpha;
	char							ver1;
	char							ver2;


	cRsmModelBase(std::string);
	cRsmModelBase() { root = NULL; }
	void save(std::string);
	virtual ~cRsmModelBase();
	void draw();
	virtual bool collides(bEngine::math::cMatrix4x4 &mat, bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* = NULL);
	void setTexture(bEngine::cTexture* oldTexture, bEngine::cTexture* newTexture);
	void setSelection(cMesh* mesh);

	virtual bEngine::util::cInStream& readData(bEngine::util::cInStream &instream);
	virtual bEngine::util::cOutStream& writeData(bEngine::util::cOutStream &outstream);
};

class cRsmModel : public cRsmModelBase, public cBrowInterface::cPluginRSMModel
{
public:
	cBrowInterface::cRoString<40> name;
	float lightopacity;
	bool selected;

	cRsmModel(std::string pFilename) : cRsmModelBase(pFilename)
	{
		filename = pFilename;
	}
	cRsmModel(bEngine::util::cInStream &instream);


	bEngine::util::cInStream& readData(bEngine::util::cInStream &instream);
	bEngine::util::cOutStream& writeData(bEngine::util::cOutStream &outstream);

	void draw();
	bool collides(bEngine::math::cVector3 from, bEngine::math::cVector3 to, bEngine::math::cVector3* = NULL);
	void setHeight();
};




#endif
