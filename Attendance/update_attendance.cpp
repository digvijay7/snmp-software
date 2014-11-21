#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

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
  try{
    pqxx::connection c("dbname=attendance user=postgres password=admin hostaddr=127.0.0.1 port=5432");
    pqxx::work w(c);
    pqxx::result res = w.exec("SELECT rollno,mac FROM ta_macs;");
    std::string output;
    for(int i=0;i<res.size();i++){
      make_http_request(url,today,today,"08:00:00","17:00:00",res[i][1].as<std::string>(),
  "eab8e69002734eda2d14e552d40a8b87112986cf8c93cd3634d23ec128b82c85",output);
      std::cout<<res[i][0]<<"------\n"<<output<<"------\n";
    }
  }
  catch(std::exception &e){
    std::cerr<<e.what()<<std::cerr;
  }
  return 0;
}
