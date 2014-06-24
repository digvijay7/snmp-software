#include <iostream>
#include <pqxx/pqxx>
#include<sstream>
#include "sha.h"

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
   char *sql;
   
   try{
      connection C("dbname=mydb user=postgres password=admin \
      hostaddr=127.0.0.1 port=5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }
      string user,pass;
	cout<<"Enter username"<<endl;
	cin>>user;
	cout<<"Enter password"<<endl;
	cin>>pass;
	string hashedpass = generatehash(pass);
	work w(C);

	stringstream ss;
	ss << "INSERT INTO users VALUES ('" << user << "','" << hashedpass <<"');";
	w.exec(ss.str());
	w.commit();

      cout << "Records created successfully" << endl;
      C.disconnect ();
   }catch (const std::exception &e){
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}


