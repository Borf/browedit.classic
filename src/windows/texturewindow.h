#ifndef __TEXTUREWINDOW_H__
#define __TEXTUREWINDOW_H__

#include <wm/wm.h>
#include <wm/window.h>
#include <wm/windowtree.h>
#include <wm/windowpicturebox.h>





class cTextureWindow : public cWindow
{
	class cWindowTexture : public cWindowPictureBox
	{
		std::string data;
	public:
		cWindowTexture(cWindow* parent);
		void onClick();
		void setText(int i, std::string s);
	};


	class cWindowTextureCatSelect : public cWindowTree
	{
	public:
		cWindowTextureCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};



public:

	std::map<cWindowTree::cTreeNode*, std::vector<std::pair<std::string, std::string> >, std::less<cWindowTree::cTreeNode*> > items;
	int iconSize;
	cTextureWindow();	
	void resizeTo(int ww, int hh);
	void* userfunc(void* param);
	bool onKeyDown(int keyid, bool shift);

};

#endif
