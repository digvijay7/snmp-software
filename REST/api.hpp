#ifndef __API_FOR_REST_OR_WHATEVER__
#define __API_FOR_REST_OR_WHATEVER__

#include <map>
#include <string>
#include <set>
#include "executor.hpp"
#include <vector>

using std::map;
using std::string;
using std::set;
using std::vector;

namespace  ourapi
{

struct args_container;

class api
{
public:
    api();
    bool executeAPI(const string& url, const map<string, string>& argval, 
            string& response);
private:
    Executor _executor;
    bool _validate(const void*  data);
    bool _executeAPI(const string& url, const struct args_container & args, Executor::outputType type, string& response);
    void _getInvalidResponse(string& response);
    map<string, set<string> > _apiparams;

};
#define AUID 1
#define AFROM 2
#define ATO 4
#define AFORMAT 8
#define ALAST 16
#define MAC 32
#define VALID_API_STD 15  // Valid API "standard" - come up with better names
#define VALID_API_LAST 17 // Valid "last" n connections api
#define VALID_API_MAC 32 // Valid API to get the uid form the MAC

struct args_container{
  unsigned int type;
  string uid,from,to,format,last,mac;
  void erase_whitespace(){
    if(type == VALID_API_STD){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(from);
      StrUtil::eraseWhiteSpace(to);
      StrUtil::eraseWhiteSpace(format);
    }
    else if(type == VALID_API_LAST){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(last);
    }
    else if(type == VALID_API_MAC){
      StrUtil::eraseWhiteSpace(mac);
    }
  }
};
} // namespace ourapi ends

#endif
