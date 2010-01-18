#include "jsonwindow.h"
#include <common.h>
#include <wm/windowlabel.h>

cJsonWindow::cJsonWindow(Json::Value &layout) : cWindow()	
{
	windowType = WT_JSON;
	resizable = false;
	visible = true;
	modal = true;
	
	std::string translation = layout["translation"].asString();

	title = GetMsg("wm/" + translation + "/TITLE");

	resizeTo(layout["width"].asInt(), layout["height"].asInt());
	if(layout["closebutton"].asBool())
		objects["closebutton"] = new cWindowCloseButton(this);

	Json::Value el = layout["components"];

	for(Json::Value::iterator i = el.begin(); i != el.end(); i++)
	{
		cWindowObject* o = NULL;
		if((*i)["type"].asString() == "checkbox")
			o = new cJsonCheckBox(this);
		if((*i)["type"].asString() == "button")
			o = new cJsonButton(this);
		if((*i)["type"].asString() == "label")
			o = new cJsonButton(this);
		o->alignment = ALIGN_TOPLEFT;

		if((*i).isMember("width"))
			o->resizeTo((*i)["width"].asInt(), (*i)["height"].asInt());
		if((*i).isMember("x"))
			o->moveTo((*i)["x"].asInt(), (*i)["y"].asInt());
		if((*i).isMember("alignment"))
			o->moveTo((*i)["x"].asInt(), (*i)["y"].asInt());

		if((*i)["type"].asString() == "checkbox")
			o->setInt(0, (*i)["value"].asBool() ? 1 : 0);
		else if((*i)["value"].asString() != "")
			o->setText(0, GetMsg("wm/" + translation + "/" + (*i)["value"].asString()));

		std::string name = i.memberName();
		if(name.length() < 2 || name.length() > 30)
			Log(3,0,"ARGH");
		objects[name] = o;

	}

	center();
}	


void cJsonWindow::event(cWindowObject* o, std::string type)
{
	for(objectlist::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if(i->second == o)
			eventhandler(this, i->first, type);
	}
}

cWindowObject* cJsonWindow::getObject( char* name)
{
	return objects[name];
}

cJsonWindow::cJsonButton::cJsonButton(cWindow* p, Json::Value &skin) : cWindowButton(p, skin)
{
	text = "";
	resizeTo(getWidth(), 15);
}

void cJsonWindow::cJsonButton::onClick()
{
	((cJsonWindow*)parent)->event(this, "click");

}

cJsonWindow::cJsonCheckBox::cJsonCheckBox( cWindow* p, Json::Value &skin) : cWindowCheckBox(p,skin)
{
}

void cJsonWindow::cJsonCheckBox::onClick()
{
	cWindowCheckBox::onClick();
	((cJsonWindow*)parent)->event(this, "click");		
}

