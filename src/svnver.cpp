#include <common.h>
#include <string>

int getversion()
{
	return atoi(std::string("$Revision: 486$").substr(11).c_str());
}
