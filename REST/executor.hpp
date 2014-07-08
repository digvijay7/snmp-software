#ifndef __EXECUTOR_FOR_API__
#define __EXECUTOR_FOR_API__

#include <string>
#include <set>
#include <vector>
#include "strutil.hpp"

using std::string;
using std::set;
using std::vector;

namespace ourapi
{

struct args_container;

class Executor
{   
public:
    enum outputType {
        TYPE_JSON, TYPE_XML   
    };
    Executor();
	bool uid(const args_container & args, outputType type,  string& response);
  bool last(const args_container & args, outputType type,  string& response,const string & url);
  bool std(const args_container & args, outputType type,  string & response,const string & url);
  bool count(const args_container &args, outputType type, string & response,const string & url);
  bool live(const args_container &args, outputType type, string & response,const string & url);
  void set_type(unsigned int q) {query_type = q;};
  unsigned int get_type() {return query_type;};
private:
	bool generic_query(const vector<string>& args, outputType type,  string& response,string query,int t);
  bool generic_query(string & response, string query);
  unsigned int query_type;
};
/* TO DO: Right now the url arguments are used to uniquely determine API correctness
   Change to using the url itself.
   Example: <ip>:<port>/client?<url_args>
   Use /client in validating
*/
#define AUID 1
#define AFROM 2
#define ATO 4
#define AFORMAT 8
#define ALAST 16
#define AMAC 32
#define AUSERNAME 64
#define APASSWORD 128
#define VALID_ARGS_STD 15
#define VALID_ARGS_LAST 17
#define VALID_ARGS_UID 32
#define VALID_URL_AUTH 256
#define VALID_URL_CLIENT 512
#define VALID_URL_AP 1024
#define VALID_URL_COUNT 2048
#define VALID_URL_UID 4096
#define VALID_URL_LIVE 8192 
#define INVALID_URL 0
#define NO_ARGS 0
#define VALID_API_UID 4128 // API to get UID
#define VALID_API_AUTH 448
#define VALID_API_AP_STD 1039
#define VALID_API_AP_LAST 1041
#define VALID_API_CLIENT_STD 527
#define VALID_API_CLIENT_LAST 529
#define VALID_API_COUNT 2062
#define VALID_API_LIVE 8192

#define MAX_ENTRIES 1000

struct args_container{
  unsigned int type;
  string uid,from,to,format,last,mac;
  void erase_whitespace(){
    if(type == VALID_ARGS_STD){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(from);
      StrUtil::eraseWhiteSpace(to);
      StrUtil::eraseWhiteSpace(format);
    }
    else if(type == VALID_ARGS_LAST){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(last);
    }
    else if(type == VALID_ARGS_UID){
      StrUtil::eraseWhiteSpace(mac);
    }
  }
};

}  // namespace ourapi

#endif
