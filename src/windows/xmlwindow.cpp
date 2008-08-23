#include "xmlwindow.h"
#include <wm/windowbutton.h>
#include <wm/windowcheckbox.h>
#include <wm/windowlabel.h>

cXmlWindow::cXmlWindow(cTexture* t, cFont* f, TiXmlDocument& skin, TiXmlDocument &layout) : cWindow(t,f,skin)	
{
	windowType = WT_XML;
	resizable = false;
	visible = true;
	modal = true;
	std::string translation = layout.RootElement()->Attribute("translation");

	title = GetMsg("wm/" + translation + "/TITLE");

	TiXmlElement* el = layout.RootElement()->FirstChildElement();
	while(el != NULL)
	{
		if(strcmp(el->Value(), "closebutton") == 0)
			objects["closebutton"] = new cWindowCloseButton(this, skin);
		else if(strcmp(el->Value(), "width") == 0)
			w = skinOffLeft + skinOffRight + atoi(el->FirstChild()->Value());
		else if(strcmp(el->Value(), "height") == 0)
			h = skinOffTop + skinOffBottom + atoi(el->FirstChild()->Value());
		else if(strcmp(el->Value(), "checkbox") == 0)
		{
			cWindowObject* o = new cWindowCheckBox(this,skin);
			o->moveTo(atoi(el->FirstChildElement("x")->FirstChild()->Value()), atoi(el->FirstChildElement("y")->FirstChild()->Value()));
			o->alignment = ALIGN_TOPLEFT;
			o->setInt(0, atoi(el->Attribute("value")));
			objects[el->Attribute("name")] = o;
		}
		else if(strcmp(el->Value(), "label") == 0)
		{
			addLabel(el->Attribute("name"), atoi(el->FirstChildElement("x")->FirstChild()->Value()), atoi(el->FirstChildElement("y")->FirstChild()->Value()), GetMsg("wm/" + translation + "/" + el->Attribute("value")));
		}
		else if(strcmp(el->Value(), "button") == 0)
		{
			cWindowObject* o = new cXmlButton(this,skin);
			o->moveTo(atoi(el->FirstChildElement("x")->FirstChild()->Value()), atoi(el->FirstChildElement("y")->FirstChild()->Value()));
			o->alignment = ALIGN_TOPLEFT;
			o->setText(0, el->Attribute("value"));
			objects[el->Attribute("name")] = o;
		}
		else
		{
			Log(3,0,"Unknown XML Option: %s", el->Value());
		}

		el = el->NextSiblingElement();
	}

	center();
}	


void cXmlWindow::event(cWindowObject* o, std::string type)
{
	for(std::map<std::string, cWindowObject*, std::less<std::string> >::iterator i = objects.begin(); i != objects.end(); i++)
	{
		if(i->second == o)
			eventhandler(this, i->first, type);
	}
}

cXmlWindow::cXmlButton::cXmlButton(cWindow* p, TiXmlDocument &skin) : cWindowButton(p, skin)
{
	
}

void cXmlWindow::cXmlButton::click()
{
	((cXmlWindow*)parent)->event(this, "click");
}