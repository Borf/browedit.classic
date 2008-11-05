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
	cWorldContainer*				worldContainer;
};

class cClipboardTexture : public cClipBoardContents
{
public:
	std::vector<std::vector<std::pair<int, cTile> > > data;
	cClipboardTexture(std::vector<std::vector<std::pair<int, cTile> > > pData) : cClipBoardContents(cClipBoard::CLIP_TEXTURE)
	{
		data = pData;
	}

	void apply();

};


#endif
