#pragma once
#include <string>

class Parse
{
private:
	std::string data;	

public:
	Parse(std::string);
	int Execute();
	~Parse(void);

};

