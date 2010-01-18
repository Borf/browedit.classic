#ifndef __JSONWINDOW_H__
#define __JSONWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <wm/windowcheckbox.h>



class cJsonWindow : public cWindow
{
public:
	class cJsonButton : public cWindowButton
	{
	public:
		cJsonButton(cWindow*, Json::Value &skin = cWM::skin);
		void onClick();
	};
	class cJsonCheckBox : public cWindowCheckBox
	{
	public:
		cJsonCheckBox(cWindow*, Json::Value &skin = cWM::skin);
		void onClick();
	};

	cJsonWindow(Json::Value&);

	void event(cWindowObject*, std::string);

	void (*eventhandler)(cWindow*, std::string, std::string);

	virtual cWindowObject* getObject(char*);

};





#endif
