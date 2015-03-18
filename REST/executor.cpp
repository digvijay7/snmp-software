
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdint.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include <pqxx/pqxx>

#include "executor.hpp"
#include "strutil.hpp"
#include <sha.h>

#include "processor.hpp"

using namespace ourapi;

using boost::property_tree::ptree;
using std::make_pair;
using std::vector;


std::vector<std::string> location_type={"b","f","w","r"};

Executor::Executor()
{
}

// Function to execute SQL statements which returns pqxx::result

bool generic_query_helper(const std::string & stmt,pqxx::result & res){
  try{
    pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    if(!conn.is_open()){
      return false;
    }
    pqxx::work w(conn);
    res = w.exec(stmt);
    w.commit();
    return true;
  }
  catch(std::exception &e){
    std::cerr<<e.what()<<std::endl;
  }
  return false;
}


/*
*******************************************************************
Sudo functions - Only functions which allow writing to the database
and/or accessing sensitive data
*******************************************************************
*/
bool Executor::su_get(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT uid,rollno,type,access FROM uid where lower(email) = lower('" + args.rollno + "') or lower(rollno) = lower('" + args.rollno +"') ;";
  std::cout<<"Making query: "<<stmt<<std::endl;
  return Executor::generic_query(response,stmt);
}

bool write_su_get(pqxx::result & res, string & response){
  ptree root_t,children;
  root_t.put("size",res.size());
  if(res.size()>0) root_t.put("rollno",res[0][1]);
  for(unsigned int i=0;i<res.size();i++){
    ptree child;
    child.put("uid",res[i][0]);
    child.put("device type",res[i][2]);
    child.put("access",res[i][3]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("devices",children);
  std::stringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}
bool Executor::su_put(const args_container &args, outputType type, string & response,const string & url){
    std::string stmt = "SELECT * FROM update_access( " + args.uid + "," + std::to_string(args.access) + ");";
    return Executor::generic_query(response,stmt);
}
bool write_su_put(pqxx::result & res, string & response){
  ptree root_t,children;
  root_t.put("affected rows",res[0][0].as<int>());
  std::stringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}
/*
******************************
Getting UID from MAC functions
******************************
*/
bool Executor::uid(const args_container &args, outputType type, string & response){
	std::stringstream ss;
	ss << "SELECT uid from uid where hash=decode('"<< generatehash(args.mac)<<"','hex');";
	return Executor::generic_query(response,ss.str());
}

bool write_uid(pqxx::result & res,string & response){
  ptree root_t;
  root_t.put("uid",res[0][0]);
  std::ostringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}
/*
******************************
Attendance
******************************
*/
bool Executor::presence(const args_container &args, outputType type, string & response,const string & url){
	std::stringstream ss;
	ss << "SELECT * FROM presence('" << generatehash(args.mac)<<"','";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"');";
  std::cout<<"Making query: "<<ss.str()<<std::endl;
	return Executor::generic_query(response,ss.str());
}
bool Executor::presence_location(const args_container &args, outputType type, string & response,const string & url){
	std::stringstream ss;
  std::string building,floor;
  if(args.building) building = args.building_str;
  if(args.floor) floor = args.floor_str;
  else floor = "any";
  ss << "SELECT * FROM presence_location('" << generatehash(args.mac)<<"','";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"','"<<building<<"','";
  ss << floor<<"');";
  std::cout<<"Making query: "<<ss.str()<<std::endl;
	return Executor::generic_query(response,ss.str());
}

bool write_presence(pqxx::result & res,string & response){
  ptree root_t,children;
  for(unsigned int row_num =0;row_num<res.size();row_num++){
    ptree child;
    child.put("date",res[row_num][0]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("presence",children);
  std::ostringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}

/*
*******************************
Getting last N entries function
*******************************
*/
bool Executor::last(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  if( url == "/client"){
    ss << "select * from client_last_building(" << args.uid <<")";
  }
  else if( url == "/ap"){
    ss << "select * from device_last(" << args.uid << ")";
  }
  else { // Not yet implemented API or invalid API
    return false;
  }
  if(atoi(args.last.c_str()) > MAX_ENTRIES){
    ss << " order by tro DESC limit " << MAX_ENTRIES + 1<<" ;";
  }
  else {
    ss << " order by tro DESC limit " << args.last <<" ;";
  }
  //std::cout<<"Query:"<<ss.str()<<std::endl;
  return Executor::generic_query(response,ss.str());
}

bool write_ap_std(pqxx::result & res, string & response,const args_container &args){
  std::vector<std::vector<std::string> >  fro_tro;
  Processor::make_fro_tro(res,fro_tro);
	ptree root_t;
  ptree children;
  unsigned int size=0;
  std::string limit_reached="no"; // default to no for now -- 18/02/2015
  root_t.put("limit reached",limit_reached);
  for(unsigned int rownum = 0 ;rownum < fro_tro.size(); rownum++){
    if(fro_tro[rownum][0] == args.uid){
      size++;
      ptree child;
      child.put("device_id",fro_tro[rownum][0]);
      child.put("client_id",fro_tro[rownum][1]);
      child.put("from",fro_tro[rownum][2]);
      child.put("to",fro_tro[rownum][3]);
      children.push_back(make_pair("",child));
    }
  }
  root_t.put("size",size);
  root_t.add_child("log entries",children);
  std::stringstream ss;
  write_json(ss,root_t);
  response = ss.str();
  return true;
}

bool write_last_std(pqxx::result & res, string & response){
	ptree root_t;
  ptree children;
  unsigned int size;
  std::string limit_reached; // yes or no
  if(res.size() > MAX_ENTRIES){
    size = MAX_ENTRIES;
    limit_reached = "yes";
  }
  else{
    size = res.size();
    limit_reached = "no";
  }
  root_t.put("size",size);
  root_t.put("limit reached",limit_reached);
  for(unsigned int rownum = 0 ;rownum < res.size(); rownum++){
    ptree child;
    child.put("device_id",res[rownum][0]);
    child.put("client_id",res[rownum][1]);
    child.put("from",res[rownum][2]);
    child.put("to",res[rownum][3]);
    if(res.columns() > 4){
      std::stringstream ss;
      unsigned int i=0;
      for(unsigned int col = 4;col<res.columns();col++){
        if(i>4){break;} //This for loop has to be improved
        if(res[rownum][col].as<std::string>() != ""){
          ss <<location_type[i]<<":"<<res[rownum][col];
          if(col<res.columns()-1 and res[rownum][col+1].as<std::string>()!="")
          { ss<<","; }
          i++;
        }
      }
      child.put("location",ss.str());
    }
    children.push_back(make_pair("",child));
  }
  root_t.add_child("log entries",children);
  std::stringstream ss;
  write_json(ss,root_t);
  response = ss.str();
  return true;
}
/*
*****************************************************************
COUNT API - returns count of distinct connections made to all APs
*****************************************************************
*/
bool Executor::count(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  ss << "SELECT * FROM all_count('" << args.from <<"','";
  ss << args.to << "','" << args.format<<"');";
  std::cout <<"Making query:" << ss.str() << std::endl;
  return Executor::generic_query(response,ss.str());
}
bool Executor::count_at(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss,ss2;
  std::vector<std::string> tmp;
  if(args.building) tmp.push_back("building");
  if(args.floor) tmp.push_back("floor");
  if(args.wing) tmp.push_back("wing");
  if(args.room) tmp.push_back("room");
  for(unsigned int i = 0;i<tmp.size();i++){
    if(i == tmp.size()-1){
      ss2 << tmp.at(i);
      continue;
    }
    ss2 << tmp.at(i) <<",";
  }
  ss << "SELECT " << ss2.str() << ",sum(count) as count from at_all_count('" << args.at;
  ss << "','"<<args.format<<"') join label on device_id = uid GROUP BY ";
  ss << ss2.str() << " ORDER BY "<<ss2.str()<< ";";
  std::cout << "Making Query:"<<ss.str()<<std::endl;
  std::stringstream ss3;
  ss3 << "SELECT "<<ss2.str()<<",client_id FROM at_all_uids('"<<args.at<<"','"<<args.format<<"') ";
  ss3 << "join label on device_id = uid GROUP BY ";
  ss3 << ss2.str() << ",client_id ORDER BY "<< ss2.str()<<",client_id;";

  pqxx::result res1,res2;
  if(!generic_query_helper(ss.str(),res1) or !generic_query_helper(ss3.str(),res2)){
    return false;
  }
  ptree root_t, children;
  root_t.put("size",res1.size());
  std::vector<std::string> col_names;
  if(res1.size() > 0){
    for(pqxx::result::tuple::size_type j=0;j<res1.size();j++){
      std::string col(res1[0][j].name(),strlen((res1[0][j]).name()));
      col_names.push_back(col);
    }
  }
  for(size_t i=0;i<res1.size();i++){
    ptree child,uids;
    for(pqxx::result::tuple::size_type j=0;j<res1[i].size();j++){
      child.put(col_names[j],res1[i][j]);
    }
    for(size_t j=0;j<res2.size();j++){
      for(pqxx::result::tuple::size_type k=0;k<res2[j].size();k++){
        bool flag=true;
        for(unsigned int a=0;a<col_names.size()-1;a++){ // makes assumption that the building,floor,etc. names
                                               // appear in the beginning!
          std::string col = col_names[a];
          if(res2[j][col].as<std::string>() != res1[i][col].as<std::string>()){
            flag=false;
            break;
          }
          if(flag){
            ptree uid;
            uid.put("",res2[j][k]);
            uids.push_back(make_pair("",uid));
          }
        }
      }
    }
    if(args.uids){
      child.add_child("uids",uids);
    }
    children.push_back(make_pair("",child));
  }
  root_t.add_child("occupancy_information",children);
  std::stringstream ss4;
  write_json(ss,root_t);
  response = ss.str();
  return true;

}

/*
************************************************
Getting entries between two dates+times function
************************************************
*/

bool Executor::std(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  if( url == "/client"){
    ss << "select * from client_std('" << args.from <<"','" << args.to << "','";
    ss << args.format << "',"<<args.uid<<")";
    ss << " order by fro limit "<< MAX_ENTRIES + 1 << ";"; // +1 to check if limit exceeded
  }
  else if( url == "/ap"){
    ss << "select * from logs_std('" << args.from <<"','" << args.to << "','";
    ss << args.format <<"');";
  }
  else { // Not yet implemented API or invalid API
    return false;
  }
  std::cout<<"Making query:"<<ss.str()<<std::endl;
  return Executor::generic_query(response,ss.str(),args);
}
/*
************************************************
Function to generate json for count type queries
************************************************
*/
bool write_count(pqxx::result & res, std::string & response){
  ptree root_t,children;
  for(size_t i = 0; i<res.size(); i++){
    ptree child;
    child.put("device_id",res[i][0]);
    child.put("batch",res[i][1]);
    child.put("count",res[i][2]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("counts",children);
  std::stringstream ss;
  write_json(ss,root_t);
  response = ss.str();
  return true;
}
bool write_count_at(pqxx::result & res, std::string & response){
  ptree root_t, children;
  root_t.put("size",res.size());
  for(size_t i=0;i<res.size();i++){
    ptree child;
    for(pqxx::result::tuple::size_type j=0;j<res[i].size();j++){
      std::string col(res[i][j].name(),strlen((res[i][j]).name()));
      child.put(col,res[i][j]);
    }
    children.push_back(make_pair("",child));
  }
  root_t.add_child("occupancy_information",children);
  std::stringstream ss;
  write_json(ss,root_t);
  response = ss.str();
  return true;
}
/*
********************************
Function to get live connections
********************************
*/
bool Executor::live(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  if(url == "/live"){
    ss << "SELECT * from get_live_table()";
    return Executor::generic_query(response,ss.str());
  }
  else{
    return false;
  }
}

bool write_live(pqxx::result & res, std::string & response){
  ptree root_t,children;
  root_t.put("size",res.size());
  for(unsigned int row = 0 ;row < res.size(); row++){
    ptree child;
    child.put("client_id",res[row][0]);
    child.put("device_id",res[row][1]);
    child.put("timestamp",res[row][2]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("live connections",children);
  std::stringstream ss;
  write_json(ss,root_t);
  response = ss.str();
  return true;
}
/*
************************************************************
Function to Register and Deregister for the Location Service
************************************************************
*/

bool Executor::loc_register(const args_container &args, outputType type, string & response,const string & url, char* ipAddress){

try{
    pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    if(!conn.is_open()){
      return false;
    }
    pqxx::work w(conn);
    std::stringstream query1;
    query1 <<  "SELECT uid FROM uid WHERE ip ='" <<ipAddress<<"';";
    pqxx::result res = w.exec(query1.str());
    w.commit();
    
    const char* uid = res[0][0].c_str();
    std::stringstream ss;
    ss << "INSERT INTO location_service VALUES (" << uid << ", now(),'" << args.appid <<"'); ";
    return Executor::generic_query(response,ss.str());
  }
  catch(const std::exception & e){
    return false;
  }

}
bool Executor::loc_deregister(const args_container &args, outputType type, string & response,const string & url, char* ipAddress){

  try{
    pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    if(!conn.is_open()){
      return false;
    }
    pqxx::work w(conn);
    std::stringstream query1;
    query1 <<  "SELECT uid FROM uid WHERE ip ='" <<ipAddress<<"';";
    pqxx::result res = w.exec(query1.str());
    w.commit();
    
    const char* uid = res[0][0].c_str();
  std::stringstream ss;
  ss<< "DELETE FROM location_service WHERE uid = " << uid << ";";
  return Executor::generic_query(response,ss.str());  
  }
  catch(const std::exception & e){
    return false;
  }
}
/*
*****************************
Function to execute SQL query
*****************************
*/
bool Executor::generic_query(string & response, const string query){
  args_container args;
  return Executor::generic_query(response,query,args);
}

bool Executor::generic_query(string & response, const string query,const args_container & args){
  try{
    pqxx::connection conn("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    if(!conn.is_open()){
      return false;
    }
    pqxx::work w(conn);
    pqxx::result res = w.exec(query);
    w.commit();
    //TBD: Maybe some checking on pqxx::result itself
    if(query_type == VALID_API_UID){
      return write_uid(res,response);
    }
    else if(query_type == VALID_API_COUNT){
      return write_count(res,response); 
    }
    else if(query_type == VALID_API_LIVE){
      return write_live(res,response); 
    }
    else if(query_type == VALID_API_COUNT_AT){
      return write_count_at(res,response);
    }
    else if(query_type == VALID_API_SU_GET){
      return write_su_get(res,response);
    }
    else if(query_type == VALID_API_SU_PUT){
      return write_su_put(res,response);
    }
    else if(query_type == VALID_API_PRESENCE or query_type == VALID_API_PRESENCE_LOCATION){
      return write_presence(res,response);
    }
    else if(query_type == VALID_API_AP_STD){
      return write_ap_std(res,response,args);
    }
    else { // Write standard or last
      return write_last_std(res,response);
    }
  }
  catch(const std::exception & e){
    std::cerr<<e.what()<<std::endl;
    return false;
  }
  response = "API not ready yet"; // Remove before issuing a pull request
  return true;
}
