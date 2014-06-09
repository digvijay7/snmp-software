#include "Log.h"


Log::Log(std::string date,std::string type,std::string device,std::string user,std::string label)
{
	Log::date = date;
	Log::type = type;
	Log::device = device;
	Log::user = user;
	Log::label = label;
}


Log::~Log(void)
{
}
