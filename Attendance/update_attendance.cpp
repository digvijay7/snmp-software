#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <string>

#include <pqxx/pqxx>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/foreach.hpp>

//#include <boost/asio.hpp>
// To compile:  g++ update_attendance.cpp -lcurlpp -lcurl -lpqxx -lpq -lboost_system -lboost_date_time 2>&1 -o update_attendance.out | more 

bool make_http_request(
  std::string addr,
  std::string from_date,
  std::string to_date,
  std::string from_time,
  std::string to_time,
  std::string mac,
  std::string building,
  std::string token,
  std::string & result)
  {
  std::string url = addr + "/presence?mac=" + mac + "&from=" + from_date +
  "-" + from_time + "&to=" + to_date + "-" + to_time + "&format=yyyy-mm-dd-hh24:mi:ss" +
  "&location=" + building + "&token=" + token;
  std::cerr<<"Making query:"<<url<<std::endl;
  std::stringstream ss;
  try {
    cURLpp::initialize();
    curlpp::Easy request;
    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(curlpp::options::SslVerifyPeer(false));
    request.setOpt(curlpp::options::SslVerifyHost(false));
    request.setOpt(curlpp::options::WriteStream(&ss));
    request.perform();
    result = ss.str();
    cURLpp::terminate();
  }
  catch ( curlpp::LogicError & e ) {
    std::cout << e.what() << std::endl;
  }
  catch ( curlpp::RuntimeError & e ) {
    std::cout << e.what() << std::endl;
  }
}

bool strip_dates(std::string json_string,std::vector<std::string> & output){
  using namespace boost::property_tree;
  std::stringstream ss;
  ss << json_string;
  ptree pt,child,date;
  read_json(ss,pt);
  child = pt.get_child("presence");
  BOOST_FOREACH(ptree::value_type & it, child){
    output.push_back(it.second.get<std::string>("date"));
  }
  return true;
}

bool get_date_range(std::string from, std::string to,std::vector<std::string> & dates){
  using namespace boost::gregorian;
  date start(from_simple_string(from)),end(from_simple_string(to));
  date_duration dd(1);
  date_period range (start,end+dd);
  for(date it = range.begin();it!=range.end();it=it+dd){
    dates.push_back(to_iso_extended_string(it));
  }
  return true;
}

int main(int argc, char * argv[]){
  if(argc!=3){
    std::cerr<<"Usage:"<<argv[0]<<" from-date to-date"<<std::endl;
    return 0;
  }
  std::string url = "https://192.168.1.40:9136";
  std::string from_date(argv[1]);
  std::string to_date(argv[2]);
  std::vector<std::string> dates;
  get_date_range(from_date,to_date,dates);
  std::ifstream config_f("config");
  if(!config_f.is_open()){
    //Take input of location of config file as command line argument
    config_f.open("/home/digvijay/Desktop/working-directory/testing/Attendance/config");
    if( !config_f.is_open()){
      std::cerr<<"Error opening config file"<<std::endl;
      return 0;
    }
  }
  std::string conn_string,token,phd_from_time,phd_to_time,mtech_from_time,mtech_to_time;
  getline(config_f,conn_string);
  getline(config_f,token);
  config_f >> phd_from_time >> phd_to_time >> mtech_from_time >> mtech_to_time;
  try{
    pqxx::connection c(conn_string);
    pqxx::work w(c);
    pqxx::result res = w.exec("SELECT m.rollno,mac,batch FROM ta_macs m join ta_info i on i.rollno = m.rollno;");
    w.commit();
    std::string output,from_time,to_time;
    std::vector<std::string> buildings;
    for(int i=0;i<res.size();i++){
      if(res[i][2].as<std::string>().compare("phd") == 0){
        from_time = phd_from_time;
        to_time = phd_to_time;
        buildings.clear();
        buildings.push_back("academic");
      }
      else{
        from_time = mtech_from_time;
        to_time = mtech_to_time;
        buildings.clear();
        buildings.push_back("academic");
        buildings.push_back("library");
        buildings.push_back("student+centre,2");
      }
      for(int k=0;k<buildings.size();k++){
        make_http_request(url,from_date,to_date,from_time,to_time,
        res[i][1].as<std::string>(),buildings[k],
        token,output);
        std::vector<std::string> present_dates;
        if(strip_dates(output,present_dates)){
          for(int j=0;j<dates.size();j++){
            std::string stmt;
            if(std::find(present_dates.begin(),present_dates.end(),dates[j])!=present_dates.end()){
              stmt = "SELECT * FROM  add_attendance(lower('" +
              res[i][0].as<std::string>() + "'),'"+dates[j]+"','1');";
            }
            else{
             stmt = "SELECT * FROM add_attendance(lower('" +
            res[i][0].as<std::string>() + "'),'"+dates[j]+"','0');";
            }
            try{
              pqxx::work w2(c);
              std::cout<<"Making query:"<<stmt<<std::endl;
              pqxx::result res2 = w2.exec(stmt);
              w2.commit();
            }
            catch(std::exception &e){
              std::cerr<<e.what()<<std::endl;
            }
          }
        }
      }
    }
  }
  catch(std::exception &e){
    std::cerr<<e.what()<<std::cerr;
  }
  return 0;
}
