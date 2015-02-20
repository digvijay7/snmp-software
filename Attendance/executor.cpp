#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <utility>


typedef std::map<std::string,std::vector<std::pair<std::string,std::string> > > attendance_map;

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
bool Executor::ta_del(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT * FROM del_ta('"+args.rollno+"');";
  pqxx::result res;
  ptree root;
  if(generic_query_helper(stmt,res)){
    root.put("status","Entry deleted");
    root.put("status code","0");
  }
  else{
    root.put("status","Error in deleting");
    root.put("status code","1");
  }
  std::ostringstream oss;
  write_json(oss,root);
  response = oss.str();
  return true;
}
bool Executor::ta_mac_del(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT * FROM del_ta_mac('"+args.rollno+"','"+args.mac+"');";
  pqxx::result res;
  ptree root;
  if(generic_query_helper(stmt,res)){
    root.put("status","Entry deleted");
    root.put("status code","0");
  }
  else{
    root.put("status","Error in deleting");
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
  std::stringstream ss;
  ss << "SELECT * FROM get_attendance_email('";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"','"<<args.email<<"');";
  pqxx::result res1;
  if(generic_query_helper(ss.str(),res1)){
    ptree root_t,children;
    root_t.put("status","Okay");
    root_t.put("status code","0");
    root_t.put("email",args.email);
    for(unsigned int row_num=0;row_num<res1.size();row_num++){
      ptree child;
      if(res1[row_num][1].as<std::string>() == "1"){
        child.put("",res1[row_num][0]);
        children.push_back(make_pair("",child));
      }
    }
    root_t.add_child("present_dates",children);
    std::ostringstream oss;
    write_json(oss,root_t);
    response = oss.str();
  }
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
  std::string stmt = "SELECT  * FROM update_attendance('"+args.rollno+
  "','"+args.at+"','"+args.format+"','"+args.present+"');";
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
bool Executor::attendance_put_time(const args_container &args, outputType type, string & response,const string & url){
  std::string stmt = "SELECT  * FROM update_attendance_timing('"+args.batch+
  "','"+args.from+"','"+args.to+"');";
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

bool Executor::attendance_get_all(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss;
  ss << "SELECT * FROM all_attendance('";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"');";
  pqxx::result res,res2;
  ptree root,children;
  std::string stmt2 = "SELECT * FROM attendance_timings";
  if(generic_query_helper(ss.str(),res) and generic_query_helper(stmt2,res2)){
    root.put("status","OK");
    root.put("status code","0");
    root.put("from",args.from);
    root.put("to",args.to);
    root.put("format",args.format);
    ptree timings;
    for(unsigned int i=0;i<res2.size();i++){
      ptree child1;
      child1.put("batch",res2[i]["batch"]); // Not a good way to reference columns directly, incase someone messes with the attendance_timings table
      child1.put("from",res2[i]["from_time"]);
      child1.put("to",res2[i]["to_time"]);
      timings.push_back(make_pair("",child1));
    }
    root.add_child("timings",timings);
    attendance_map attendance;
    std::string rollno,date,status;
    for(unsigned int i=0;i<res.size();i++){
      rollno = res[i][0].as<std::string>(); date = res[i][1].as<std::string>();
      status = res[i][2].as<std::string>();
      attendance[rollno].push_back(std::make_pair(date,status));
    }
    for(attendance_map::iterator it = attendance.begin();
        it != attendance.end();
        it++){
      ptree child1,child2,children2;
      child1.put("rollno",it->first);
      for(unsigned int i=0 ; i< it->second.size() ; i++){
        if(it->second[i].second.compare("1") == 0){
          child2.put("",it->second[i].first);
          children2.push_back(make_pair("",child2));
        }
      }
      child1.add_child("present_dates",children2);
      children.push_back(make_pair("",child1));
    }
    root.add_child("attendance",children);
  }
  else{
    root.put("status","Error in getting attendance");
    root.put("status code","1");
  }
  std::stringstream oss;
  write_json(oss,root);
  response = oss.str();
  return true;
}
/*
******************************
Exceptions for days
******************************
*/
bool Executor::exception_get(const args_container &args, outputType type, string & response,const string & url){
  std::stringstream ss,ss2;
  ss << "SELECT * FROM get_positive_exceptions('";
  ss << args.from <<"','"<<args.to<<"','";
  ss << args.format <<"');";
  ss2 << "SELECT * FROM get_negative_exceptions('";
  ss2 << args.from <<"','"<<args.to<<"','";
  ss2 << args.format <<"');";
  pqxx::result res,res2;
  if( !(generic_query_helper(ss.str(),res) and generic_query_helper(ss2.str(),res2))){
    return false;
  }
  ptree root,positive_exceptions_t,negative_exceptions_t,child;
  root.put("status","OK");
  root.put("status code","0");
  root.put("from",args.from);
  root.put("to",args.to);
  root.put("format",args.format);
  for(unsigned int i=0;i<res.size();i++){
    child.put("",res[i][0].as<std::string>());
    positive_exceptions_t.push_back(make_pair("",child));
  }
  for(unsigned int i=0;i<res2.size();i++){
    child.put("",res[i][0].as<std::string>());
    negative_exceptions_t.push_back(make_pair("",child));
  }
  root.add_child("positive exceptions",positive_exceptions_t);
  root.add_child("negative_exceptions",negative_exceptions_t);
  std::ostringstream oss;
  write_json(oss,root);
  response = oss.str();
  return true;
}
bool Executor::exception_put(const args_container &args, outputType type, string & response,const string & url){
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
