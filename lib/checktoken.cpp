#include <iostream>
#include <pqxx/pqxx> 
#include <sstream>
#include "sha.h"

using namespace pqxx;

bool tokenchecking(std::string token,bool & is_sudo)
{
	try{
		connection C("dbname=mydb user=postgres password=admin \
	      	hostaddr=127.0.0.1 port=5432");		
      		if (!C.is_open()) {
        		std::cout << "Can't open database" << std::endl;
      		}
		
		work w(C);
		std::stringstream ss;
		ss<<"SELECT su from tokentable t join users u on u.username = t.username  where token = decode('" <<token<< "','hex');";				
		result R(w.exec(ss.str()));
		w.commit();
    bool ret=false;
    if( R.size()>0 ) ret = true;
    else return false;
    if( R[0][0].as<int>() == 1 ) is_sudo = true;
    C.disconnect();
    return ret;
	}
	catch (const std::exception &e){
      		std::cerr << e.what() << std::endl;
   	}
}
