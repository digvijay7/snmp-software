#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <time.h>

using namespace std;
using namespace pqxx;

int main()
{
	char *sql;
   
	try{
      		connection C("dbname=mydb user=postgres password=admin \
	      	hostaddr=127.0.0.1 port=5432");
	      	if (C.is_open()) {
        		cout << "Opened database successfully: " << C.dbname() << endl;
      		}
		else {
			cout << "Can't open database" << endl;
	        	return 1;
      		}
		time_t curr_sec;
		int curr_sec_int;
		
		for(;;)
		{work W1(C);
		
			sql = (char*)"SELECT username,expiration from tokenTable";

	        	result R( W1.exec( sql ));
	      		W1.commit();
			
			for (result::const_iterator c = R.begin(); c != R.end(); ++c)
			{work W2(C);
				curr_sec = time (NULL);
				curr_sec_int = (int)curr_sec;
				if (curr_sec_int > c[1].as<int>())
				{	
					
					std::stringstream ss;
					ss << (char*)"DELETE from tokenTable where username = '"<<c[0].as<string>()<<"';";
					W2.exec(ss.str());
					W2.commit();
				}
			}
			sleep(2);	
		}
	}
	catch (const std::exception &e){
      		std::cerr << e.what() << std::endl;
   	}
	return 0;
}

