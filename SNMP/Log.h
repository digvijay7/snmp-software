#pragma once
#include <string>
class Log
{
	std::string date,device,user,label,type;
public:
	Log(std::string,std::string,std::string,std::string,std::string);
	std::string getDate(){return date;};
	std::string getDevice(){return device;};
	std::string getUser(){return user;};
	std::string getLabel(){return label;};
	std::string getType(){return type;};
	~Log(void);
};



