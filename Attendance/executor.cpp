#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <stdint.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include <pqxx/pqxx>

#include "executor.hpp"
#include "strutil.hpp"
#include <sha.h>


using namespace ourapi;

using boost::property_tree::ptree;
using std::make_pair;
using std::vector;

enum get_mac_by {EMAIL, ROLLNO};

Executor::Executor()
{
}
bool generic_query_helper(const std::string & stmt,pqxx::result & res){
  try{
    pqxx::connection conn("dbname=attendance user=postgres password=admin hostaddr=127.0.0.1 port=5432");
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
bool get_mac(std::string txt,enum get_mac_by type,std::string & mac){
  std::stringstream ss;
  if(type == EMAIL){
  	ss << "SELECT * FROM get_mac_email('" << txt<<"');";
  }
  else if(type == ROLLNO){
    ss << "SELECT * FROM get_mac_rollno('"<<txt<<"');";
  }
  std::cout<<"Making query: "<<ss.str()<<std::endl;
  pqxx::result res;
  if(generic_query_helper(ss.str(),res)){
    mac = res[0][0].as<std::string>();
    return true;
  }
  return false;
}
/*
******************************
TA info funcitons
******************************
*/
bool Executor::ta_get_all(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt1 = "SELECT * FROM get_info_all()";
  std::string stmt2 = "SELECT * FROM get_macs_all()";
  pqxx::result res1,res2;
  if(generic_query_helper(stmt1,res1)){ // Optimize by making one query.
    ptree root_t,children;
    if(res1.size()==0){
      root_t.put("status","No TAs found");
      std::ostringstream oss;
      write_json(oss,root_t);
      response = oss.str();
      return true;
    }
    // else 
    std::map<std::string,std::vector<std::string> > rollno_macs;
    if(generic_query_helper(stmt2,res2)){
      for(unsigned int row_num=0;row_num<res2.size();row_num++){
        std::string mac = res2[row_num]["mac"].as<std::string>();
        rollno_macs[res2[row_num]["rollno"].as<std::string>()].push_back(mac);
      }
    }
    for(unsigned int row_num=0;row_num<res1.size();row_num++){
      ptree child;
      child.put("rollno",res1[row_num]["rollno"]);
      child.put("email",res1[row_num]["email"]);
      child.put("name",res1[row_num]["name"]);
      child.put("batch",res1[row_num]["batch"]);
      std::string temp = (res1[row_num]["rollno"].as<std::string>()); 
      std::vector<std::string> macs = rollno_macs[temp];
      ptree children2;
      for(unsigned int j=0;j<macs.size();j++){
        ptree child2;
        child2.put("",macs[j]);
        children2.push_back(make_pair("",child2));
      }
      child.add_child("macs",children2);
      children.push_back(make_pair("",child));
    }
    root_t.add_child("TAs",children);
    std::ostringstream oss;
    write_json(oss,root_t);
    response = oss.str();
  }
  return true;
}
bool Executor::ta_get_email(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt1 = "SELECT * FROM get_info_email('"+args.email+"');";
  std::string stmt2 = "SELECT * FROM get_mac_email('"+args.email+"');";
  pqxx::result res1,res2;
  if(generic_query_helper(stmt1,res1)){ // Optimize by making one query.
    ptree root_t,children;
    if(res1.size()==0){
      root_t.put("status","No TA with "+args.email +" found");
      std::ostringstream oss;
      write_json(oss,root_t);
      response = oss.str();
      return true;
    }
    // else 
    root_t.put("rollno",res1[0]["rollno"]);
    root_t.put("email",res1[0]["email"]);
    root_t.put("name",res1[0]["name"]);
    root_t.put("batch",res1[0]["batch"]);
    if(generic_query_helper(stmt2,res2)){
      for(unsigned int row_num=0;row_num<res2.size();row_num++){
        ptree child;
        child.put("",res2[row_num][0]);
        children.push_back(make_pair("",child));
      }
    }
    root_t.add_child("macs",children);
    std::ostringstream oss;
    write_json(oss,root_t);
    response = oss.str();
  }
  return true;
}
bool Executor::ta_get_rollno(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt1 = "SELECT * FROM get_info_rollno('"+args.rollno+"');";
  std::string stmt2 = "SELECT * FROM get_mac_rollno('"+args.rollno+"');";
  pqxx::result res1,res2;
  if(generic_query_helper(stmt1,res1)){ // Optimize by making one query.
    ptree root_t,children;
    if(res1.size()==0){
      root_t.put("status","No TA with roll number "+args.rollno +" found");
      std::ostringstream oss;
      write_json(oss,root_t);
      response = oss.str();
      return true;
    }
    // else 
    root_t.put("rollno",res1[0]["rollno"]);
    root_t.put("email",res1[0]["email"]);
    root_t.put("name",res1[0]["name"]);
    root_t.put("batch",res1[0]["batch"]);
    if(generic_query_helper(stmt2,res2)){
      for(unsigned int row_num=0;row_num<res2.size();row_num++){
        ptree child;
        child.put("",res2[row_num][0]);
        children.push_back(make_pair("",child));
      }
    }
    root_t.add_child("macs",children);
    std::ostringstream oss;
    write_json(oss,root_t);
    response = oss.str();
  }
  return true;
}
bool Executor::ta_put_info(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT * FROM put_ta('"+args.rollno+"','"+
  args.email+"','"+args.name+"','"+args.batch+"');";
  pqxx::result res;
  ptree root;
  if(generic_query_helper(stmt,res)){
    root.put("status","Entry added");
    root.put("status code","0");
  }
  else{
    root.put("status","Error in adding");
    root.put("status code","1");
  }
  std::ostringstream oss;
  write_json(oss,root);
  response = oss.str();
  return true;
}
bool Executor::ta_put_mac(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT * FROM put_mac('"+args.rollno+"','"+args.mac+"');";
  pqxx::result res;
  ptree root;
  if(generic_query_helper(stmt,res)){
    root.put("status","Entry added");
    root.put("status code","0");
  }
  else{
    root.put("status","Error in adding");
    root.put("status code","1");
  }
  std::ostringstream oss;
  write_json(oss,root);
  response = oss.str();
  return true;
}

/*
******************************
Attendance
******************************
*/
bool Executor::attendance_get_email(const args_container &args, outputType type, string & response,const string & url){
  std::string mac="Reached here";
  if(!get_mac(args.email,EMAIL,mac)){
    response = "No such email found"; //TBD: create a (JSON) error generating function
    return false;
  }
  //Get UID from MAC

  //And get logs
  response = mac;
  return true;
}
bool Executor::attendance_get_rollno(const args_container &args, outputType type, string & response,const string & url){
  std::string mac;
  if(!get_mac(args.rollno,ROLLNO,mac)){
    response = "No such email found"; //TBD: create a (JSON) error generating function
    return false;
  }
  //Get UID from MAC

  //And get logs
  response = mac;
  return true;
}

bool Executor::attendance_put(const args_container &args, outputType type, string & response,const string & url){
  std::string mac;
  //Get UID from MAC

  //And get logs
  response = mac;
  return true;
}

bool write_attendance(pqxx::result & res,string & response){
  ptree root_t,children;
  for(unsigned int row_num =0;row_num<res.size();row_num++){
    ptree child;
    child.put("date",res[row_num][0]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("attendance",children);
  std::ostringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}

bool Executor::attendance_get_all(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  ss << "SELECT * FROM all_attendance('";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"');";
  return Executor::generic_query(response,ss.str());
}


bool write_attendance_all(pqxx::result & res,string & response){
  ptree root_t,children;
  for(unsigned int row_num =0;row_num<res.size();row_num++){
    ptree child;
    child.put("rollno",res[row_num][0]);
    child.put("date",res[row_num][1]);
    children.push_back(make_pair("",child));
  }
  root_t.add_child("attendance",children);
  std::ostringstream oss;
  write_json(oss,root_t);
  response = oss.str();
  return true;
}
/*
*****************************
Function to execute SQL query
*****************************
*/
bool Executor::generic_query(string & response, const string query){
  try{
    pqxx::connection conn("dbname=attendace user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    if(!conn.is_open()){
      return false;
    }
    pqxx::work w(conn);
    pqxx::result res = w.exec(query);
    w.commit();
    //TBD: Maybe some checking on pqxx::result itself
    /*if(query_type == VALID_API_UID){
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
    else if(query_type == VALID_API_ATTENDANCE){
      return write_attendance(res,response);
    }
    else if(query_type == VALID_API_ATTENDANCE_ALL){
      return write_attendance_all(res,response);
    }
    else { // Write standard or last
      return write_last_std(res,response);
    }*/
  }
  catch(const std::exception & e){
    std::cerr<<e.what()<<std::endl;
    return false;
  }
  response = "API not ready yet"; // Remove before issuing a pull request
  return true;
}
