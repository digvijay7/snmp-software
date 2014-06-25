#include <iostream>
#include <string.h>
#include <pqxx/pqxx> 
#include <sstream>
#include "sha.h"
#include <time.h>

using namespace pqxx;

std::string generatetoken(std::string inpUsername,std::string inpPassword){
	char *sql,*sql2;
	std::string token;
	bool alreadyExistsFlag=false, credentialsMatchFlag=false;
	const char* inpU = inpUsername.c_str(); 
	try{
		connection C("dbname=mydb user=postgres password=admin \
	      	hostaddr=127.0.0.1 port=5432");
      		if (!C.is_open()) {
        		std::cout << "Can't open database" << std::endl;
      		}
		work W1(C);
		std::stringstream ss;
		ss << "SELECT username, encode(password,'hex') from users where username ='"<<inpUsername<<"';";
		result R(  W1.exec(ss.str())  );
		W1.commit();
	
		
		result::const_iterator v=R.begin();
		if(R.empty())
		{
			C.disconnect();
			return "";
		}
		if(!R.empty())
		{
			std::string resultPass = v[1].as<std::string>();
			std::string hashedPassword = generatehash(inpPassword);
      std::cout<<hashedPassword<<std::endl;
      std::cout<<resultPass<<std::endl;
			if(hashedPassword==resultPass)
			{
					credentialsMatchFlag=true;

			}
				
		}
		
		if(credentialsMatchFlag==false)
		{
			C.disconnect();
			return "";
		}
		else if(credentialsMatchFlag)
		{
			time_t curr_sec;
			time_t expiration_time;

			//if token already exists and request for auth comes again then generate token again and replace it.
			work W2(C);
			std::stringstream ss;
			ss << "SELECT username from tokenTable where username ='"<<inpUsername<<"';";
			result Rest2(  W2.exec(ss.str())  );
			W2.commit();

			if( !Rest2.empty() ) //user exists in tokentable
			{
					alreadyExistsFlag =true;
					curr_sec = time (NULL);
					expiration_time =  curr_sec +1200;
					token = hashtoken(inpUsername,curr_sec);
					work W3(C);
					std::stringstream s1;
					s1<< "UPDATE tokenTable set token = '"<< token <<"', expiration ='"<< expiration_time <<"' where USERNAME='" <<inpUsername <<"'";
					W3.exec(s1.str());
					W3.commit();

			}
			
			if (!alreadyExistsFlag)
			{
				curr_sec = time (NULL);
				expiration_time =  curr_sec +1200; //expiration time is set 20mins i.e 1200secs
				token = hashtoken(inpUsername,curr_sec);

				work W4(C);
				std::stringstream ss;
				ss << "INSERT INTO tokenTable (username, token, expiration) VALUES ('" << inpUsername << "','" << token << "','" <<expiration_time << "');";
				W4.exec(ss.str());
				W4.commit();
			}//if stat for alreadyExistsFlag ends
		}
		C.disconnect();
		return token;
	}
	catch (const std::exception &e){
      	std::cerr << e.what() << std::endl;
   	}

}
