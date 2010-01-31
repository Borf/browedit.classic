#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <vector>
#include <world.h>
class cWorldContainer;
class cClipBoardContents;

class cClipBoard
{
public:
	enum eClipBoardType
	{
		CLIP_TEXTURE,
		CLIP_HEIGHT,
		CLIP_OBJECT,
		CLIP_GAT,
		CLIP_OBJECTS,
		CLIP_AREA,
	};

	cClipBoard(){};
	static void					setClipBoard(cClipBoardContents*);
	static cClipBoardContents*	currentClipBoard;
	static bool					pasting;
};


class cClipBoardContents
{
public:
	cClipBoardContents(cClipBoard::eClipBoardType);
	cClipBoard::eClipBoardType		type;
	virtual void					apply() = 0;
	virtual void					render() {};
	cWorldContainer*				worldContainer;
};

class cClipboardTexture : public cClipBoardContents
{
public:
	std::vector<std::vector<std::pair<int, cTile> > > data;
	cClipboardTexture() : cClipBoardContents(cClipBoard::CLIP_TEXTURE) {};
	void apply();
	void render();
};


class cClipboardHeight : public cClipBoardContents
{
public:
	std::vector<std::vector<std::vector<float> > > data;
	cClipboardHeight() : cClipBoardContents(cClipBoard::CLIP_HEIGHT) {};
	void apply();
	void render();
};

class cClipboardGat : public cClipBoardContents
{
public:
	std::vector<std::vector<std::pair<int, std::vector<float> > > > data;
	cClipboardGat() : cClipBoardContents(cClipBoard::CLIP_GAT) {};
	void apply();
	void render();
};

class cClipboardObject : public cClipBoardContents
{
public:
	cVector3 clipboardRot;
	cVector3 clipboardScale;
	cVector3 pos;
	cVector3 pos2;
	std::string clipboardFile;
	std::string clipboardName;
	float clipboardY;
	float clipboardFloat;
	cRsmModel* rsmmodel;
	bool usePos;

	cClipboardObject() : cClipBoardContents(cClipBoard::CLIP_OBJECT)
	{
		rsmmodel = NULL;
		usePos = false;
	}
	~cClipboardObject();
	void apply();
	void render();
};

class cClipBoardObjects : public cClipBoardContents
{
public:
	cClipBoardObjects() : cClipBoardContents(cClipBoard::CLIP_OBJECTS) {};
	std::vector<cClipboardObject*> objects;
	void rotate();
	void apply();
	void render();
};

class cClipBoardArea : public cClipBoardContents
{
public:
	class cContainer
	{
	public:
		cCube		cube;
		cGatTile	gat;
		int x;
		int y;
	};
	std::vector<cContainer>			tiles;
	std::vector<cClipboardObject*>	objects;
	std::vector<cLight>				lights;
	std::vector<cEffect>			effects;
	std::vector<cSound>				sounds;


	int startX,startZ;

	bool doTextures;
	bool doHeight;
	bool doObjects;
	bool doGat;
	bool doWalls;
	bool doLights;
	bool doEffects;
	bool doSounds;

	cClipBoardArea(bool,bool,bool,bool,bool,bool,bool,bool);
	~cClipBoardArea();
	void apply();
	void render();
};


#endif
