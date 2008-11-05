#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <vector>

class cWorldContainer;
#include <world.h>

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
	};

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
	std::string clipboardFile;
	std::string clipboardName;
	float clipboardY;
	float clipboardFloat;
	cRSMModel* rsmmodel;

	cClipboardObject() : cClipBoardContents(cClipBoard::CLIP_OBJECT)
	{
		rsmmodel = NULL;
	}
	~cClipboardObject();
	void apply();
	void render();
};


#endif
