#ifndef __WINDOWLABEL_H__
#define __WINDOWLABEL_H__

#include "windowobject.h"
#include <string>
#include <vector>
//using namespace std;

class cWindowLabel : public cWindowObject
{
protected:
	std::string text;
	std::string lasttext;
	std::vector<std::string> lines;
public:
	cWindowLabel(cWindow* parent);
	virtual ~cWindowLabel() {}
	virtual void draw(int,int,int,int);
	void setText(int, std::string);
	std::string getText(int);

	int getInt(int);

};




#endif
