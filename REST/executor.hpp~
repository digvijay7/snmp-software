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
  bool count_at(const args_container &args, outputType type, string & response,const string & url);
  bool live(const args_container &args, outputType type, string & response,const string & url);
  bool su_get(const args_container &args, outputType type, string & response,const string & url);
  bool su_put(const args_container &args, outputType type, string & response,const string & url);
  bool attendance(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_all(const args_container &args, outputType type, string & response,const string & url);	
  bool loc_register(const args_container &args, outputType type, string & response,const string & url);
  bool loc_deregister(const args_container &args, outputType type, string & response,const string & url);
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
#define AAT 16384
#define ATYPE 32768
#define AROLLNO 131072
#define AACCESS 262144
#define AEMAIL 524288
#define AAPPID 2097152
#define VALID_ARGS_AT 49160
#define VALID_ARGS_STD 15
#define VALID_ARGS_LAST 17
#define VALID_ARGS_UID 32
#define VALID_URL_AUTH 256
#define VALID_URL_CLIENT 512
#define VALID_URL_AP 1024
#define VALID_URL_COUNT 2048
#define VALID_URL_UID 4096
#define VALID_URL_LIVE 8192 
#define VALID_URL_SU 65536
#define VALID_URL_ATTENDANCE 1048576
#define VALID_URL_REGISTER 4194304
#define VALID_URL_DEREGISTER 8388608 
#define INVALID_URL 0
#define NO_ARGS 0
#define VALID_API_UID 4128 // API to get UID
#define VALID_API_AUTH 448
#define VALID_API_AP_STD 1039
#define VALID_API_AP_LAST 1041
#define VALID_API_CLIENT_STD 527
#define VALID_API_CLIENT_LAST 529
#define VALID_API_COUNT 2062
#define VALID_API_COUNT_AT 51208
#define VALID_API_LIVE 8192
#define VALID_API_SU_GET 196608
#define VALID_API_SU_PUT 327681
#define VALID_API_ATTENDANCE 1572878
#define VALID_API_ATTENDANCE_ALL 1048590
#define VALID_API_REGISTER 6291457
#define VALID_API_DEREGISTER 8388609
#define MAX_ENTRIES 1000

struct args_container{
  unsigned long type;
  int access;
  string uid,from,to,format,last,mac,at,rollno,email,appid;
  bool building,floor,wing,room;
  args_container(){
    building = floor = wing = room = false;
  }
  bool set(char c){
    if(c == 'b'){
      building = true;
    }
    else if(c == 'f'){
      floor = true;
    }
    else if(c == 'w'){
      wing = true;
    }
    else if( c == 'r'){
      room = true;
    }
    else {
      return false;
    }
    return true;
  }

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
