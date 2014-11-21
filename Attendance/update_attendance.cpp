#include <iostream>
#include <sstream>
#include <cstdlib>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <string>

//#include <pqxx/pqxx>
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
  std::string token)
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
    std::string buf;
    buf = ss.str();
    std::cout<<buf;
    cURLpp::terminate();
  }
  catch ( curlpp::LogicError & e ) {
    std::cout << e.what() << std::endl;
  }
  catch ( curlpp::RuntimeError & e ) {
    std::cout << e.what() << std::endl;
  }
}
int main(){
  std::string url = "https://192.168.1.40:9119";
  std::string date = 
  make_http_request("https://127.0.0.1:9119","2014-06-01","2014-08-02",
  "01:01:00","01:01:01","f0:6b:ca:e2:fd:ee",
  "3d4e0b1a115953507e908b318a83cdccfe52fe48579d89813114459dbad98a6b");
  return 0;
}
