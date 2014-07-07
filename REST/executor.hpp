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
private:
	bool generic_query(const vector<string>& args, outputType type,  string& response,string query,int t);
  bool generic_query(string & response, string query,unsigned int type);
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
#define VALID_URL_AUTH 64
#define VALID_URL_CLIENT 128
#define VALID_URL_AP 256
#define VALID_URL_COUNT 512
#define INVALID_URL 0
#define INVALID_ARGS 0
#define VALID_ARGS_STD 15  // Valid ARGS for "standard" query - come up with better names
#define VALID_ARGS_LAST 17 // Valid args for "last" n connections query
#define VALID_ARGS_MAC 32 // Valid args to get the uid form the MAC
#define VALID_ARGS_COUNT 14 // Valid args count to get no. of connections made to APs
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
    else if(type == VALID_ARGS_MAC){
      StrUtil::eraseWhiteSpace(mac);
    }
  }
};

}  // namespace ourapi

#endif
