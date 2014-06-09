//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// Compile with: g++ serv.cpp -static -lboost_system -lboost_thread -lpthread -o serv.o


#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <pthread.h>
#include "Parse.h"
#include <sstream>

using boost::asio::ip::tcp;

void * make_thread(void * data){
//	std::string * copy = (std::string *)data;
//	std::cout <<"Making thread with data:"<< *copy <<std::endl;
	Parse p(*((std::string *)data));
	p.Execute();
	int i=0;
	//pthread_exit(NULL);
}


int main(int argc, char * argv[])
{
	if(argc!=2){
		std::cout<<"Usage: ./server <port>"<<std::endl;
		return -1;
	}
  try
  {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), atol(argv[1])));

    for (;;)
    {
		tcp::socket socket(io_service);
		acceptor.accept(socket);
		boost::array<char, 2048> buf;
//      std::string message = make_daytime_string();
		while(1){
			boost::system::error_code error;
			size_t len = socket.read_some(boost::asio::buffer(buf),error);
			if (error == boost::asio::error::eof){
				break; // Connection closed cleanly by peer.
			}
			else if (error)
				throw boost::system::system_error(error); // Some other error.*/
			/*pthread_t thread;
			int rc;*/
			std::string * data = new std::string(buf.begin(),buf.end());
			data = new std::string(*data,0,len);
			//rc = pthread_create(&thread,NULL,make_thread,(void *)data);
			make_thread((void *)data);
			std::stringstream sas;
			sas<<"Server says hi!\n";
			std::string *message = new std::string("Server says hi!\n");
			boost::asio::write(socket,boost::asio::buffer(*message),boost::asio::transfer_all(),error);
//	  std::cout.write(buf.data(),len);
//		std::cout.flush();
	  }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
