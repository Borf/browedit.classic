#include "messagewindow.h"


cMessageWindow::cWindowOkButton::cWindowOkButton( cWindow* parent, TiXmlDocument &skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_BOTTOM;
	moveTo(0, 20);
	resizeTo(100, 20);
	text = "Ok";
}

void cMessageWindow::cWindowOkButton::click()
{
	parent->close();
}

cMessageWindow::cMessageWindow( cTexture* t, cFont* f, TiXmlDocument &skin ) : cWindow(t,f,skin)
{
	windowType = WT_MESSAGE;
	resizable = false;
	visible = true;
	modal = true;
	
	h = 200;
	w = 350;
	title = "Message...";
	center();
	
	defaultObject = "OkButton";
	
	//		objects["rollup"] = new cWindowRollupButton(this);
	//		objects["close"] = new cWindowCloseButton(this);
	
	addLabel("text", 15,40, "UserId")->resizeTo(320, 100);;
	objects["OkButton"] = new cWindowOkButton(this,skin);
}