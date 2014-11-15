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
  bool attendance_get_email(const args_container &args, outputType type, string & response,const string & url);
  bool ta_get_email(const args_container &args, outputType type, string & response,const string & url);
  bool ta_get_rollno(const args_container &args, outputType type, string & response,const string & url);
  bool ta_get_all(const args_container &args, outputType type, string & response,const string & url);
  bool ta_put(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_get_rollno(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_put(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_get_all(const args_container &args, outputType type, string & response,const string & url);
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
#define ANAME 1
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
#define ABATCH 262144
#define AEMAIL 524288
#define AFTIME 512
#define ATTIME 1024
#define VALID_URL_AUTH 256
#define VALID_URL_ATTENDANCE_GET 1048576
#define VALID_URL_ATTENDANCE_PUT 2048
#define VALID_URL_TA_GET 4096
#define VALID_URL_TA_PUT 8192
#define INVALID_URL 0
#define NO_ARGS 0
#define VALID_API_UID 4128 // API to get UID
#define VALID_API_AUTH 448
#define VALID_API_ATTENDANCE_GET_EMAIL 1572878
#define VALID_API_ATTENDANCE_GET_ROLLNO 1179662
#define VALID_API_ATTENDANCE_PUT 690208 // Has to be changed
#define VALID_API_ATTENDANCE_GET_ALL 1048590
#define VALID_API_TA_GET_EMAIL (VALID_URL_TA_GET + AEMAIL)
#define VALID_API_TA_GET_ROLLNO (VALID_URL_TA_GET + AROLLNO)
#define VALID_API_TA_GET_ALL (VALID_URL_TA_GET)
#define VALID_API_TA_PUT (VALID_URL_TA_PUT + AEMAIL + AROLLNO + AMAC + ANAME +ABATCH)
#define MAX_ENTRIES 1000

struct args_container{
  string type;
  string name,from,to,format,last,mac,at,rollno,email,batch;
  args_container(){
  }

  void erase_whitespace(){
/*    if(type == VALID_ARGS_STD){
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
    }*/
  }
};

}  // namespace ourapi

#endif
