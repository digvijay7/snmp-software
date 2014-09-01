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

class api
{
public:
    api();
    bool executeAPI(const string& url, const map<string, string>& argval, string& response, bool & is_sudo);
    bool authenticateAPI(const map<string, string>& argvals, string& response,bool & is_sudo);
private:
    Executor _executor;
    bool _validate(const void*  data);
    bool _executeAPI(const string& url, const struct args_container & args, Executor::outputType type, string& response,bool & is_sudo);
    void _getInvalidResponse(string& response);
    map<string, set<string> > _apiparams;

};


} // namespace ourapi ends

#endif
