#ifndef __EXECUTOR_FOR_API__
#define __EXECUTOR_FOR_API__

#include <string>
#include <set>
#include <vector>

using std::string;
using std::set;
using std::vector;

namespace ourapi
{

class Executor
{   
public:
    enum outputType {
        TYPE_JSON, TYPE_XML   
    };
    Executor();
	bool client(const vector<string>& args, outputType type,  string& response);
	bool client_last(const vector<string>& args, outputType type,  string& response);
	bool client_ondate(const vector<string>& args, outputType type,  string& response);
	bool client_from(const vector<string>& args, outputType type,  string& response);
	bool client_to(const vector<string>& args, outputType type,  string& response);
	bool client_from_to(const vector<string>& args, outputType type,  string& response);
	bool client_near(const vector<string>& args, outputType type,  string& response);
	bool ap(const vector<string>& args, outputType type,  string& response);
	bool ap_last(const vector<string>& args, outputType type,  string& response);
	bool ap_ondate(const vector<string>& args, outputType type,  string& response);
	bool ap_connections(const vector<string>& args, outputType type,  string& response);
	bool ap_near(const vector<string>& args, outputType type,  string& response);
	bool uid(const vector<string>& args, outputType type,  string& response);
private:
	bool generic_query(const vector<string>& args, outputType type,  string& response,string query,int t);
};



}  // namespace ourapi

#endif
