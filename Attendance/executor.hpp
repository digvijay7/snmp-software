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
  bool ta_del(const args_container &args, outputType type, string & response,const string & url);
  bool ta_mac_del(const args_container &args, outputType type, string & response,const string & url);
  bool ta_get_rollno(const args_container &args, outputType type, string & response,const string & url);
  bool ta_get_all(const args_container &args, outputType type, string & response,const string & url);
  bool ta_put_info(const args_container &args, outputType type, string & response,const string & url);
  bool ta_put_mac(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_get_rollno(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_put(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_put_time(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_get_all(const args_container &args, outputType type, string & response,const string & url);
  bool attendance_update(const args_container &args, outputType type, string & response,const string & url);
  bool exception_put(const args_container &args, outputType type, string & response,const string & url);
  bool exception_del(const args_container &args, outputType type, string & response,const string & url);
  bool exception_get(const args_container &args, outputType type, string & response,const string & url);
  bool log_get(const args_container &args, outputType type, string & response,const string & url);
  bool log_put(const args_container &args, outputType type, string & response,const string & url);
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
//#define AFTIME 512
#define APRESENT 1024
#define VALID_URL_AUTH 256
#define VALID_URL_ATTENDANCE_GET 1048576
#define VALID_URL_ATTENDANCE_PUT 2048
#define VALID_URL_ATTENDANCE_UPDATE 33554432
#define VALID_URL_TA_GET 4096
#define VALID_URL_TA_PUT 8192
#define VALID_URL_TA_DEL 512
#define VALID_URL_EXCEPTION_PUT 65536
#define VALID_URL_EXCEPTION_GET 2097152
#define VALID_URL_EXCEPTION_DEL 4194304
#define VALID_URL_LOG_GET 8388608
#define VALID_URL_LOG_PUT 16777216
#define INVALID_URL 0
#define NO_ARGS 0
#define VALID_API_UID 4128 // API to get UID
#define VALID_API_AUTH 448
#define VALID_API_ATTENDANCE_GET_EMAIL (VALID_URL_ATTENDANCE_GET + AFROM + ATO + AFORMAT + AEMAIL)
#define VALID_API_ATTENDANCE_GET_ROLLNO (VALID_URL_ATTENDANCE_GET + AFROM + ATO + AFORMAT + AROLLNO)
#define VALID_API_ATTENDANCE_PUT_TIME (VALID_URL_ATTENDANCE_PUT + ABATCH + AFROM + ATO )
#define VALID_API_ATTENDANCE_PUT (VALID_URL_ATTENDANCE_PUT + AROLLNO + AAT + AFORMAT + APRESENT +AUSERNAME)
#define VALID_API_ATTENDANCE_GET_ALL 1048590 //(VALID_URL_ATTENDANCE_GET + AFROM + ATO + AFORMAT)
#define VALID_API_ATTENDANCE_UPDATE (VALID_URL_ATTENDANCE_UPDATE + AFROM + ATO + AFORMAT + AUSERNAME)
#define VALID_API_TA_GET_EMAIL (VALID_URL_TA_GET + AEMAIL)
#define VALID_API_TA_GET_ROLLNO (VALID_URL_TA_GET + AROLLNO)
#define VALID_API_TA_GET_ALL (VALID_URL_TA_GET)
#define VALID_API_TA_DEL (VALID_URL_TA_DEL + AROLLNO + AUSERNAME)
#define VALID_API_TA_MAC_DEL (VALID_URL_TA_DEL + AROLLNO + AMAC + AUSERNAME)
#define VALID_API_TA_PUT_INFO (VALID_URL_TA_PUT + AEMAIL + AROLLNO + ANAME +ABATCH + AUSERNAME)
#define VALID_API_TA_PUT_MAC (VALID_URL_TA_PUT + AROLLNO + AMAC + AFROM + AFORMAT + AUSERNAME)
#define VALID_API_EXCEPTION_PUT (VALID_URL_EXCEPTION_PUT +  AAT + AFORMAT + ATYPE +AUSERNAME)
#define VALID_API_EXCEPTION_GET (VALID_URL_EXCEPTION_GET +  AFROM + ATO + AFORMAT )
#define VALID_API_EXCEPTION_DEL (VALID_URL_EXCEPTION_DEL +  AAT + AFORMAT +AUSERNAME)
#define VALID_API_LOG_GET (VALID_URL_LOG_GET +  AFROM + ATO + AFORMAT)
#define VALID_API_LOG_PUT (VALID_URL_LOG_PUT +  AEMAIL + ATYPE)
#define MAX_ENTRIES 1000

struct args_container{
  string type,username;
  string name,from,to,format,last,mac,at,rollno,email,batch,present,atype;
  args_container(){
    present = "0";
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
