#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>

#include <stdint.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include <pqxx/pqxx>

#include "executor.hpp"
#include "strutil.hpp"

using namespace ourapi;

using boost::property_tree::ptree;
using std::make_pair;
using std::vector;


Executor::Executor()
{
}

bool Executor::ap(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where device_id="<< args.at(0)<<" and ts>= to_timestamp('"<<args.at(1) << " " << args.at(3)<< "','dd-mm-yyyy hh24-mi-ss') ";
	ss << "and ts<= to_timestamp('"<< args.at(2) << " " << args.at(4) << "','dd-mm-yyyy hh24-mi-ss');";
	//std::cout << ss.str()<<std::endl;
	return Executor::generic_query(args,type,response,ss.str(),0);	
}
bool Executor::ap_ondate(const vector<string>&args, outputType type, string & response){
	vector<string> copy;
	copy.push_back(args.at(0));
	copy.push_back(args.at(1));
	copy.push_back(args.at(1));
	copy.push_back(args.at(2));
	copy.push_back(args.at(3));
	return Executor::ap(copy,type,response);	
}
bool Executor::ap_near(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "select * from logs where device_id = " << args.at(0) << "order by ts desc;";
	return Executor::generic_query(args,type,response,ss.str(),0);
}

bool Executor::ap_last(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where device_id="<< args.at(0)<<" order by ts desc LIMIT "<< args.at(1)<<";";
	return Executor::generic_query(args,type,response,ss.str(),0);
}
bool Executor::ap_connections(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << " select t2.device_id,t1.mac,t2.connections ";
	ss << " from ";
	ss << " ( select device_id,count(distinct client_id) as connections from logs where ts >= now() - interval '30 minutes' and type = 1 group by device_id) as t2 ";
	ss << " left join uid as t1 on t1.uid = t2.device_id; ";
	return Executor::generic_query(args,type,response,ss.str(),2);
}

bool Executor::uid(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT uid from uid where mac='"<< args.at(0)<<"';";
	return Executor::generic_query(args,type,response,ss.str(),1);
}

bool Executor::generic_query(const vector<string>&args, outputType type, string & response,string query,int t){
	try{    
		pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
		if (!conn.is_open()) 
				return false;
		pqxx::work w(conn);
		pqxx::result res = w.exec(query);//where user='b0:ec:71:4e:f1:b3'
		w.commit();
		const int num_rows = res.size();
		std::ostringstream oss,oss2;
		ptree root_t;	
		if(t==0){
			ptree children;
			root_t.put("client",args.at(0));
			root_t.put("size",num_rows);
			for (int rownum=0; rownum < num_rows; ++rownum)
			{
			//	oss<<res[rownum][0]<<' '<<res[rownum][1]<<' '<<res[rownum][2]<<' '<<res[rownum][3]<<'\n';
				ptree child;
				child.put("device_id",res[rownum][0]);
				child.put("client_id",res[rownum][1]);
				child.put("ts",res[rownum][2]);
				child.put("label",res[rownum][3]);
				child.put("type",res[rownum][4]);
				children.push_back(make_pair("",child));
			}
			root_t.add_child("log entry",children);
			write_json(oss,root_t);
			response = oss.str();
		}
		else if (t ==1){
			root_t.put("mac",args.at(0));
			root_t.put("size",num_rows);
			root_t.put("uid",res[0][0]);
			write_json(oss,root_t);
			response = oss.str();
		}
		else if(t==2){
			ptree children;
			root_t.put("size",num_rows);
			for (int rownum=0; rownum < num_rows; ++rownum)
			{
			//	oss<<res[rownum][0]<<' '<<res[rownum][1]<<' '<<res[rownum][2]<<' '<<res[rownum][3]<<'\n';
				ptree child;
				child.put("device_id",res[rownum][0]);
				child.put("mac",res[rownum][1]);
				child.put("connections",res[rownum][2]);
				children.push_back(make_pair("",child));
			}
			root_t.add_child("status",children);
			write_json(oss,root_t);
			response = oss.str();
		}
	}catch(const std::exception &e){
		std::cerr << e.what() << std::endl;
		return false;
	}
//	std::cout<<response<<std::endl;
	return true;
}
bool Executor::client(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where client_id="<< args.at(0)<<" and ts>= to_timestamp('"<<args.at(1) << " " << args.at(3)<< "','dd-mm-yyyy hh24-mi-ss') ";
	ss << "and ts<= to_timestamp('"<< args.at(2) << " " << args.at(4) << "','dd-mm-yyyy hh24-mi-ss') order by ts desc;";
	//std::cout << ss.str()<<std::endl;
	return Executor::generic_query(args,type,response,ss.str(),0);	
}

bool Executor::client_ondate(const vector<string>&args, outputType type, string & response){
	vector<string> copy;
	copy.push_back(args.at(0));
	copy.push_back(args.at(1));
	copy.push_back(args.at(1));
	copy.push_back(args.at(2));
	copy.push_back(args.at(3));
	return Executor::client(copy,type,response);	
}
bool Executor::client_last(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where client_id="<< args.at(0)<<" order by ts desc LIMIT "<< args.at(1)<<";";
	return Executor::generic_query(args,type,response,ss.str(),0);
}

bool Executor::client_from(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where client_id="<< args.at(0)<<" and ts >='"<<args.at(1)<<"';";
	return Executor::generic_query(args,type,response,ss.str(),0);
}

bool Executor::client_near(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "select * from logs where device_id = (select device_id from logs where client_id = ";
	ss << args.at(0);
	ss << " order by ts desc limit 1) and ts >= now() - interval '15 minutes'and type = 1 order by ts desc;";
	return Executor::generic_query(args,type,response,ss.str(),0);
}

bool Executor::client_to(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where client_id="<< args.at(0)<<" and ts <='"<<args.at(1)<<"';";
	return Executor::generic_query(args,type,response,ss.str(),0);
}
bool Executor::client_from_to(const vector<string>&args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT * from logs where client_id="<< args.at(0)<<" and ts >='"<<args.at(1)<<"' and ts <= '"<<args.at(2)<<"';";
	return Executor::generic_query(args,type,response,ss.str(),0);
}
