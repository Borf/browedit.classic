#ifndef __SOUNDSELECTWINDOW_H__
#define __SOUNDSELECTWINDOW_H__

#include <common.h>
#include <wm/window.h>
#include <wm/windowtree.h>
#include <wm/windowlistbox.h>
#include <wm/windowbutton.h>


class cSound;

class cSoundSelectWindow : public cWindow
{
	class cWindowSoundCatSelect : public cWindowTree
	{
	public:
		cWindowSoundCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, TiXmlDocument &skin);
		void click();
	};
	class cSoundList : public cWindowDataListBox<std::string>
	{
	public:
		cSoundList(cWindow* parent, TiXmlDocument &skin);
		void doubleClick();
	};
	class cWindowPlayButton : public cWindowButton
	{
	public:
		cWindowPlayButton(cWindow* w, TiXmlDocument &skin);
		void click();
	};
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* w, TiXmlDocument &skin);
		void click();
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* w, TiXmlDocument &skin);
		void click();
	};

public:

	std::map<cWindowTree::cTreeNode*, std::vector<std::pair<std::string, std::string> >, std::less<cWindowTree::cTreeNode*> > items;
	cVector3 newPos;
	cSound* selectedSound;

	cSoundSelectWindow(cTexture* t, cFont* f, TiXmlDocument &skin, cVector3 pNewPos);	
	void resizeTo(int ww, int hh);
	void* userfunc(void* param);


};

#endif
