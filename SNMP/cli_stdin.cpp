//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Compile with : g++ cli.cpp -static -lboost_system -lboost_thread -lpthread -o cli_stdin.o


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <vector>
#include <ctime>

using boost::asio::ip::tcp;

class info{
	std::string client,device,label,date;
	bool ok;
	int type,line_count;
	public:
	info(){line_count=0;ok=false;};
	~info(){};
	void setClient(std::string in){ client = in;};
	void setDate(std::string in){ date = in;};
	void setDevice(std::string in){ device = in;};
	void setLabel(std::string in){ label = in;};
	void setTrap(int in){ type = in; };
	void setLineCount(int in){ line_count = in; };
	std::string getClient(){return client;};
	std::string getDevice(){return device;};
	std::string getLabel(){return label;};
	std::string getDate(){return date;};
	int getTrap(){return type;};
	int getLineCount(){return line_count;};
	void setOk(bool val){ok = val;};
	bool getOk(){return ok;};
	std::string getData(){ 
		std::stringstream ss; 
		ss << getDate() << "\n" << getTrap()<< "\n" << getDevice()<<"\n" << getClient();
		ss << "\n" << getLabel()<<"\n";
		return ss.str();
	};
};

void put_log(std::ofstream & out, std::string text){
	using namespace std;
	time_t t = time(0);
	struct tm * now = localtime( & t );
	std::cout << (now->tm_year + 1900) << '-' 
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << ' '
		<< now->tm_hour << ':'
		<< now->tm_min << ':'
		<< now->tm_sec
		<< endl;
	std::cout <<text<<std::endl;
}

void put_log(std::ofstream & out, std::string text,long int number){
	using namespace std;
	time_t t = time(0);
	struct tm * now = localtime( & t );
	std::cout << (now->tm_year + 1900) << '-' 
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << ' '
		<< now->tm_hour << ':'
		<< now->tm_min << ':'
		<< now->tm_sec
		<< endl;
	std::cout <<text<<" "<<number<<std::endl;
}
int addTrapType(std::string type){
	int ra =0;
	if(type=="AIRESPACE-WIRELESS-MIB::bsnDot11StationAssociate"){
//		(*log).push_back("1");
		ra=1;
	}
	else if(type=="AIRESPACE-WIRELESS-MIB::bsnDot11StationDeauthenticate"){
//		(*log).push_back("2");
		ra=2;
	}
	else if(type=="CISCO-LWAPP-RRM-MIB::ciscoLwappDot11ClientCoverageHolePreAlarm"){
//		(*log).push_back("3");
		ra=3;
	}
	else{
//		(*log).push_back("0");
		ra=0;
	}
	return ra;
}

int information(int trap, std::string info){
	std::stringstream ss(info);
	int ra;
	ss>>info;
	if(trap!=3){
		if(info.compare("AIRESPACE-WIRELESS-MIB::bsnAPName.0")==0){
			ra = 3;
		}
		else if(info.compare("AIRESPACE-WIRELESS-MIB::bsnStationMacAddress.0")==0){
			ra = 2;
		}
		else if(info.compare("AIRESPACE-WIRELESS-MIB::bsnStationAPMacAddr.0")==0){
			ra = 1;
		}
		else if(info.compare("--------------")==0){
			ra = 0;
		}
		else
			ra = -1;
	}
	else{
		if(info.compare("CISCO-LWAPP-AP-MIB::cLApName.0")==0){
			ra = 3;
		}
		else if(info.compare("CISCO-LWAPP-DOT11-CLIENT-MIB::cldcClientMacAddress.0")==0){
			ra = 2;
		}
		else if(info.compare("CISCO-LWAPP-DOT11-CLIENT-MIB::cldcApMacAddress.0")==0){
			ra = 1;
		}
		else if(info.compare("--------------")==0){
			ra = 0;
		}
		else
			ra = -1;
	}

	return ra;
}

info parse(std::vector<std::string> * log, bool getseperator,std::ofstream & out){
	std::string buffer;
	info inf;
  int i,trap,seen;
	try{
    while(1){
      if(getseperator)
        std::getline(std::cin,buffer);
      std::istringstream iss(buffer);
  		std::string first;
  		iss>>first;
  		int i=0,trap=0,seen=0;
  		if(first.compare("--------------")==0){
  		  i=0;trap=0;seen=0;
    		for(;i<3;i++){getline(std::cin,buffer);}
    			//(*log).push_back(buffer); // DATE TIME
    		inf.setDate(buffer);
    		for(i=0;i<9;i++){getline(std::cin,buffer);}
    			std::istringstream iss2(buffer);
    			for(i=0;i<4;i++)iss2>>buffer;
    			trap = addTrapType(buffer);
    			inf.setTrap(trap);
    			if(trap==0){
            getseperator = true;
    				continue;
    			}
    			while(seen!=3){
    				getline(std::cin,buffer);
    				int type = information(trap,buffer);
    				if(type==0){
    					put_log(out,"Encountered a --------------. Breaking and setting Ok to false.");
    					break;
    				}
    				else if(type==-1) continue;
    				else{
    					std::stringstream tempss(buffer);
    					tempss >> buffer;tempss>>buffer;tempss>>buffer;tempss>>buffer;
    					if(type == 1) {inf.setDevice(buffer);std::cout<<"Setting Device = "<<buffer<<std::endl;}
    					else if(type == 2) inf.setClient(buffer);
    					else if(type == 3) inf.setLabel(buffer);
    					seen++;
    					if(seen==3) inf.setOk(true);
    				}
    			}
    			break;
    		}
        else{
  		  	//do nothing;
    		}
    }
   	
  }
 	catch (std::exception& e)
 	{
 		std::cerr << e.what() << std::endl;
 		inf.setLineCount(0);
 		return inf;
 	}
 	inf.setLineCount(0);
 	return inf;
}

int main(int argc, char* argv[])
{
	//For Logging
	// --
	if (argc != 3)
    {
		std::cerr << "Usage: client <host> <port>" << std::endl;
		return 1;
    }
	std::stringstream tss;
	tss << "/home/iiitd/cli_stdin" <<argv[2]<<".log";
	std::cout<<"Trying to open \""<<tss.str()<<"\" log file"<<std::endl;
	std::ofstream out (tss.str().c_str(),std::ofstream::out);
	//
	try
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], argv[2]);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error){
			throw boost::system::system_error(error);
		}
//		std::ifstream in ("/home/digvijay/temp.txt",std::ifstream::in);
//		std::ifstream in ("/media/digvijay/44E6C1E0E6C1D27A/snmptraps.log",std::ifstream::in);
//		std::fstream file("/home/digvijay/Desktop/count.txt");
    bool setseperator=false;
		while(1)
		{
	//      boost::array<char, 256> buf;
			boost::system::error_code error;
			std::vector<std::string> log;
			put_log(out,"Going to parse.");
      info inf = parse(&log,setseperator,out); //atol(line.c_str())
			put_log(out,"Back from parsing.");
			if(inf.getOk()){
        setseperator = false;
			}
			else{
				if(inf.getLineCount()==0){
          setseperator = false;
        }
				else{
          setseperator = true;
				}
			}
			std::string buf(inf.getData());
			put_log(out,"Sending to server.");
			put_log(out,inf.getData());
			boost::asio::write(socket,boost::asio::buffer(buf),boost::asio::transfer_all(),error);
			boost::array<char, 2048> stemp;
			size_t len = socket.read_some(boost::asio::buffer(stemp),error);
    }
  }
  catch (std::exception& e){
  	put_log(out,"Could not connect...");
  	put_log(out,e.what());
  	std::cerr << e.what() << std::endl;
  }
	out.close();
	return 0;
}

