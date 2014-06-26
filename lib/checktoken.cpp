#include <iostream>
#include <pqxx/pqxx> 
#include <sstream>
#include "sha.h"

using namespace pqxx;

bool tokenchecking(std::string token)
{
	try{
		connection C("dbname=mydb user=postgres password=admin \
	      	hostaddr=127.0.0.1 port=5432");		
      		if (!C.is_open()) {
        		std::cout << "Can't open database" << std::endl;
      		}
		
		work w(C);
		std::stringstream ss;
		ss<<"SELECT count(*) from tokentable where token = decode('" <<token<< "','hex');";				
		result R(w.exec(ss.str()));
		w.commit();

		for(result::const_iterator v=R.begin(); v!=R.end(); ++v){
			if(v[0].as<int>() == 1)
			{
				C.disconnect();
				return true;
			}
			else
			{
				C.disconnect();
				return false;
			}
		}
	}
	catch (const std::exception &e){
      		std::cerr << e.what() << std::endl;
   	}
}
