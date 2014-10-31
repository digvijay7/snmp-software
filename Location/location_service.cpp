#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <stdlib.h>
#include <pthread.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <string>
#include <boost/asio.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::tcp;
using boost::property_tree::ptree;

void send_push_message(std::string mytree)
{
	std::stringstream ss;
	ss<<mytree;
	boost::property_tree::ptree pt;
	boost::property_tree::read_json(ss, pt);
	//boost::property_tree::write_json( std::cout, pt );
	
	std::ostringstream buf; 
	write_json (buf, pt, false);
	std::string json = buf.str();

	const char *url = "https://android.googleapis.com/gcm/send";
  
	  try {
	    curlpp::Cleanup cleaner;
	    curlpp::Easy request;
    
	    request.setOpt(new curlpp::options::Url(url)); 
    
	    std::list<std::string> header; 
	    header.push_back("Content-Type: application/json");
	    header.push_back("Authorization: key=AIzaSyBWIddo9xX1VmJGioj3WvRncLMWA8o6gLk");  
    
	    request.setOpt(new curlpp::options::HttpHeader(header)); 
	    request.setOpt(new curlpp::options::PostFields(json));
	    request.setOpt(new curlpp::options::PostFieldSize(json.length()));

    
	    request.perform(); 
	  }
	  catch ( curlpp::LogicError & e ) {
	    std::cout << e.what() << std::endl;
	  }
	  catch ( curlpp::RuntimeError & e ) {
	    std::cout << e.what() << std::endl;
	  }
}


std::string message;
void* thread_fn(void *x){
	send_push_message(message);
}

class notifier_loc: public pqxx::notification_receiver{
	public:
		pthread_t thread1;
		notifier_loc(pqxx::connection_base &conn, const std::string &ch): pqxx::notification_receiver(conn, ch) {}
		virtual void operator() (const std::string &payload, int backend_pid) 
		{
			message = payload;
			pthread_create( &thread1, NULL, &thread_fn, NULL);
		}
};
int main(int argc, char* argv[])
{
   char *sql;
   
   try{
      pqxx::connection C("dbname=mydb user=postgres password=admin hostaddr=127.0.0.1 port=5432");
      if (!C.is_open()) {
	return 1;      
      }
	std::string chan = "channel_service_insert";
	notifier_loc obj(C,chan);

	while(1)
	{
		C.await_notification();
		C.get_notifs();
	}
 	C.disconnect ();
   }catch (const std::exception &e){
      std::cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}
