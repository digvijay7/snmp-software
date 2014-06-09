#include<iostream>
#include<pqxx/pqxx>
#include<fstream>
#include<vector>
#include<sstream>
#include "sha256.h"

class entry{
	std::string id,mac;
public:
	entry(std::string id,std::string mac){this->id = id;this->mac=mac;};
	std::string getid(){return id;};
	std::string getmac(){return mac;};
	void show(){ std::cout<< id <<" "<<mac<<std::endl;};
};

class uids{
	std::vector <entry> data;
	std::string filename;
public:
	uids(std::string filename){this->filename = filename;};
	bool populate(){
		try{
			std::fstream in(filename.c_str());
			std::string buf,id,mac;
			getline(in,buf);
			std::stringstream ss;
			while(getline(in,buf)){
				ss.str(buf);
				ss >> id >> mac;
				entry new_entry(id,mac);
				data.push_back(new_entry);
				ss.str("");
				ss.clear();
			}
			return true;
		}
		catch(std::exception &e){
			std::cerr << e.what() <<std::endl;
			return false;
		}
	};
	unsigned int size(){
		return data.size();
	}
	bool display(){
		entry *temp;
		for(int i=0;i<data.size();i++){
			data.at(i).show();
		}
		return true;
	}
	entry at(unsigned int index){
		return data.at(index);
	}
};
std::string generatehash(std::string mac){
	std::string output1,salt="digivjay";
	for(int i=0; i< 1000; i++){
		output1 = sha256(output1 + mac + salt);
	}
	return output1;
}
bool push(entry ent,std::string sha){
	pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
	if(conn.is_open()){
		pqxx::work w(conn);
		std::stringstream ss;
		ss << "update uid set hash = decode('"<<sha<<"','hex')";
		ss << " where uid = "<<ent.getid()<<" ;";
		pqxx::result res = w.exec(ss.str());
		w.commit();
		return true;
	}
	return false;
}

int main(){
	uids uid("uid-table");
	if(uid.populate()){
		std::string hash;
		for(int i=0;i<uid.size();i++)
		{
			hash = generatehash(uid.at(i).getmac());
			if(!push(uid.at(i),hash)){
				std::cout<<"Could not push: "<<hash<<" ";
				uid.at(i).show();
			}
		}
	}
	return 0;
}
