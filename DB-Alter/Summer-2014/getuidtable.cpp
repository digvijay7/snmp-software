#include<iostream>
#include<pqxx/pqxx>

int main(){
//	using namespace std;
	pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
	if(!conn.is_open()){
		std::cout << "Connection colud not be opened!\n";
		return -1;
	}
	std::string query = "Select uid,mac from uid;";
	pqxx::work w(conn);
	try{
		pqxx::result res = w.exec(query);
		w.commit();
		std::cout << "Got "<<res.size()<<" rows\n";
		for(int i=0;i<res.size();i++){
			std::cout << res[i][0]<<" "<<res[i][1]<<std::endl;
		}
	}
	catch(const std::exception &e){
		std::cerr << e.what() << std::endl;
		return 0;
	}
	return 0;
}
