#ifndef __XMLWINDOW_H__
#define __XMLWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <wm/windowcheckbox.h>



class cXmlWindow : public cWindow
{
public:
	class cXmlButton : public cWindowButton
	{
	public:
		cXmlButton(cWindow*, Json::Value &skin = cWM::skin);
		void onClick();
	};
	class cXmlCheckBox : public cWindowCheckBox
	{
	public:
		cXmlCheckBox(cWindow*, Json::Value &skin = cWM::skin);
		void onClick();
	};

	cXmlWindow(TiXmlDocument&);

	void event(cWindowObject*, std::string);

	void (*eventhandler)(cWindow*, std::string, std::string);

};





#endif
