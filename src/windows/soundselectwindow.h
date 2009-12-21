#ifndef __SOUNDSELECTWINDOW_H__
#define __SOUNDSELECTWINDOW_H__

#include <common.h>
#include <wm/window.h>
#include <wm/windowtree.h>
#include <wm/windowlistbox.h>
#include <wm/windowbutton.h>
#include <bengine/math/vector3.h>


class cSound;

class cSoundSelectWindow : public cWindow
{
	class cWindowSoundCatSelect : public cWindowTree
	{
	public:
		cWindowSoundCatSelect(cWindow* parent, std::vector<cWindowTree::cTreeNode*> n, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cSoundList : public cWindowDataListBox<std::string>
	{
	public:
		cSoundList(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onDoubleClick();
	};
	class cWindowPlayButton : public cWindowButton
	{
	public:
		cWindowPlayButton(cWindow* w, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* w, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	class cWindowCancelButton : public cWindowButton
	{
	public:
		cWindowCancelButton(cWindow* w, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};

public:

	std::map<cWindowTree::cTreeNode*, std::vector<std::pair<std::string, std::string> >, std::less<cWindowTree::cTreeNode*> > items;
	bEngine::math::cVector3 newPos;
	cSound* selectedSound;

	cSoundSelectWindow(bEngine::math::cVector3 pNewPos);	
	void resizeTo(int ww, int hh);
	void* userfunc(void* param);


};

#endif
