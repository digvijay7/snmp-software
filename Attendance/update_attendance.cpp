#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <string>

#include <pqxx/pqxx>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/asio.hpp>

bool make_http_request(
  std::string addr,
  std::string from_date,
  std::string to_date,
  std::string from_time,
  std::string to_time,
  std::string mac,
  std::string token,
  std::string & result)
  {
  std::string url = addr + "/presence?mac=" + mac + "&from=" + from_date +
  "-" + from_time + "&to=" + to_date + "-" + to_time + "&format=yyyy-mm-dd-hh24:mi:ss" +
  "&token=" + token;
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

std::string get_today_date(){
  using namespace std;
  time_t t = time(0);
  struct tm * now = localtime(& t);
  stringstream ss;
  ss << (now->tm_year)+ 1900 << "-";
  ss << (now->tm_mon) + 1 << "-";
  ss << (now->tm_mday);
  return ss.str();
}

int main(){
  std::string url = "https://192.168.1.40:9136";
  std::string today = get_today_date();
  std::ifstream config_f("config");
  if(!config_f.is_open()){
    std::cerr<<"Error opening config file"<<std::endl;
    return 0;
  }
  std::string conn_string,token,phd_from_time,phd_to_time,mtech_from_time,mtech_to_time;
  getline(config_f,conn_string);
  getline(config_f,token);
  config_f >> phd_from_time >> phd_to_time >> mtech_from_time >> mtech_to_time;
  try{
    pqxx::connection c(conn_string);
    pqxx::work w(c);
    pqxx::result res = w.exec("SELECT rollno,mac,batch FROM ta_macs join ta_info ;");
    std::string output,from_time,to_time;
    for(int i=0;i<res.size();i++){
      if(res[i][2].as<std::string>() == "phd"){
        from_time = phd_from_time;
        to_time = phd_to_time;
      }
      else{
        from_time = mtech_from_time;
        to_time = mtech_to_time;
      }
      make_http_request(url,today,today,from_time,to_time,res[i][1].as<std::string>(),
      token,output);
     // std::cout<<res[i][0]<<"------\n"<<output<<"------\n";
    }
  }
  catch(std::exception &e){
    std::cerr<<e.what()<<std::cerr;
  }
  return 0;
}
