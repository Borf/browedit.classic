#ifndef __XMLWINDOW_H__
#define __XMLWINDOW_H__

#include "window.h"
#include "windowbutton.h"



class cXmlWindow : public cWindow
{
public:
	class cXmlButton : public cWindowButton
	{
	public:
		cXmlButton(cWindow*, TiXmlDocument&);
		void click();
	};
	cXmlWindow(cTexture*, cFont*, TiXmlDocument&, TiXmlDocument&);

	void event(cWindowObject*, string);

	void (*eventhandler)(cWindow*, string, string);

};





#endif
