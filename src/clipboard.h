#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

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
	eClipBoardType type;


	static cClipBoard* currentClipBoard;
};


#endif
