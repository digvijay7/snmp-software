#include "Parse.h"
#include "Log.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <pqxx/pqxx>
#include <cstring>
#include <sha.h>

Log makeLog(std::vector<std::string>);
std::string makeDate(std::string);
void pushLog(Log,pqxx::connection *);
int isMac(std::string mac);
int isHex(char a);


Parse::Parse(std::string data)
{
	Parse::data = data;	
}

// * Improve Parse function by accessing key value pairs
// * If database connection not open, put data in temp file and try to insert it into database later
int Parse::Execute(){
	pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
	if (conn.is_open()) {
	//	std::cout << "We are connected to " << conn.dbname() << std::endl;
	} 
	else{
		std::cout<<"Connection not established. Quitting Parsing. Try again."<<std::endl;
		return -1;
	}
	std::stringstream ss(Parse::data);
	std::string buffer;
	std::vector<std::string> log;
	int i=0,count=1;
	while(getline(ss,buffer)){
		//std::cout<<count<<":"<<buffer<<std::endl;
		count++;
		i++;
		log.push_back(buffer);
		if(i==5){
			i=0;
			Log to_push_log = makeLog(log);
			pushLog(to_push_log,&conn);
			log.clear();
		}
	}
	conn.disconnect();

/*	std::ifstream in (Parse::filename.c_str(),std::ifstream::in);
	std::ofstream out("output.txt",std::ofstream::out);
	if(!in || !out){
		std::cout<<"in or out file could not be opened!\n";
		throw 1;
	}
	std::string buffer;
//	long count = 0;
	while(std::getline(in,buffer)){
		std::istringstream iss(buffer);
		std::string first;
		iss>>first;
		if(first.compare("--------------")==0){
			std::vector<std::string> log;
			int i=0;
			for(;i<3;i++)getline(in,buffer);
			log.push_back(buffer); // DATE TIME
			//std::cout<<buffer<<std::endl;
			for(i=0;i<10;i++)getline(in,buffer);
			std::istringstream iss2(buffer);
			for(i=0;i<4;i++)iss2>>buffer;
			//std::cout<<buffer<<std::endl;
			log.push_back(buffer); // Device Mac Address
			for(i=0;i<2;i++)getline(in,buffer);
			std::istringstream iss3(buffer);
			for(i=0;i<4;i++)iss3>>buffer;
			//std::cout<<buffer<<std::endl;
			log.push_back(buffer); // Client Mac Address
			for(i=0;i<2;i++)getline(in,buffer);
			std::istringstream iss4(buffer);
			for(i=0;i<4;i++)iss4>>buffer;
			//std::cout<<buffer<<std::endl;
			log.push_back(buffer); // Location Label
//			std::cout<<log.size()<<std::endl;
			Log to_push_log = makeLog(log);
			pushLog(to_push_log,&conn);
		}
		else{
			//std::cout<<"";//do nothing;
		}
	}
	in.close();
	out.close();*/
	return 0;
}

void pushLog(Log log,pqxx::connection *conn){
	using namespace std;
	std::cout<<"Pushing: "<<log.getDevice()<<","<<log.getUser()<<","<<log.getDate()<<","<<log.getLabel()<<","<<log.getType()<<std::endl;
	pqxx::work w(*conn);
	std::stringstream stmt;
	stmt<<"SELECT log_insert('"<<generatehash(log.getDevice())<<"','"<<generatehash(log.getUser())<<"','"<<log.getDate()<<"','"<<log.getLabel()<<"',"<< log.getType() <<");";
	try{
		pqxx::result res = w.exec(stmt.str());
		w.commit();
	}
	catch(const std::exception &e){
		std::cerr << e.what() << std::endl;
		std::cout<<"Exception on statement:["<<stmt.str()<<"]\n";
		return;
	}
}

Log makeLog(std::vector<std::string> list){
	std::string date,device,user,label,type;
	date  = makeDate(list.at(0));
	type = list.at(1);
	if(!isMac(list.at(2))) device = "null";
	else device = list.at(2);
	if(!isMac(list.at(3))) user = "null";
	else user = list.at(3);
	//Put in function later - extract quoted text
	std::string temp;
	temp = list.at(4);
	if(temp.compare(0,1,"\"")==0) // Making blind assumption, if first character is a \" then the last will be too.
		label = temp.substr(1,temp.size()-2);
	else
		label = temp;
	// End Label function
	return Log(date,type,device,user,label);
}
std::string makeDate(std::string date){
	std::string year,month,day,hour,minute,second;
	std::string temp;
	std::istringstream iss(date);
	std::stringstream iss2;
	std::vector<std::string> tokens;
	try{
		while(iss >> temp)
			tokens.push_back(temp);
		year = tokens.at(10);
		month = tokens.at(8);
		day = tokens.at(6);
		hour = tokens.at(1);
		minute = tokens.at(3);
		second = tokens.at(5);
		iss2<<year<<"-"<<month<<"-"<<day<<" "<<hour<<":"<<minute<<":"<<second;
		return iss2.str();
	}
	catch(std::exception& e){
		return "null";
	}
	
}

int isHex(char a){
	if(a=='a' || a=='b' || a=='c'|| a=='d'|| a=='e'|| a=='f'|| ( (a - '0' >= 0) && (a - '0' <= 9)))
		return 1;
	else
		return -1;
}

int isMac(std::string mac){
	if(mac.size()>17) return -1;
	char temp[18];
	strncpy(temp,mac.c_str(),mac.size());
	for(unsigned int i = 0 ; i < mac.size() ; i++){
		if(!isHex(temp[i]) && temp[i]!=':') return -1;
	}
	return 1;
}

Parse::~Parse(void)
{
}
