#include <string.h>
#include <boost/foreach.hpp>
#include "api.hpp"
#include "strutil.hpp"
#include <iostream>

using namespace ourapi;

struct validate_data
{
    string api;
    set <string>* params; 
};

api::api()
{
    set<string> params;
}

bool api::executeAPI(const string& url, const map<string, string>& argvals, string& response)
{
    // Ignore all the args except the "fields" param 
    validate_data vdata ;
    vdata.api = url;
    Executor::outputType type = Executor::TYPE_JSON;
    set<string> uniqueparams;
	vector<bool> isPara;
	vector<std::string> args;
	bool mac=false,uid=false,fd=false,td=false,ft=false,tt=false,od=false,last=false; //f_=from _d=date _t=time t_=to od = on date
    map<string,string>::const_iterator it1 = argvals.find("uid");
	map<string,string>::const_iterator it2 = argvals.find("fd");
	map<string,string>::const_iterator it3 = argvals.find("td");
	map<string,string>::const_iterator it4 = argvals.find("od");
	map<string,string>::const_iterator it5 = argvals.find("ft");
	map<string,string>::const_iterator it6 = argvals.find("tt");
	map<string,string>::const_iterator it7 = argvals.find("mac");
	map<string,string>::const_iterator it8 = argvals.find("last");
    /*if (it1 != argvals.end()) {
        string prms = it1->second;
        StrUtil::eraseWhiteSpace(prms);
        StrUtil::splitString(prms, ",", params);   
    }*/
	// Unique params will come handy when, in future we allow for multiple MACs to be sent at once.
	string prms;	
	if(it1!=argvals.end()){
	prms = it1->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	uid=true;
	}
	if(it2!=argvals.end()){
	prms = it2->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	fd=true;
	}
	if(it3!=argvals.end()){
	prms = it3->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	td=true;
	}
	if(it4!=argvals.end()){
	prms = it4->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	od=true;
	}
	if(it5!=argvals.end()){
	prms = it5->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	ft=true;
	}
	if(it6!=argvals.end()){
	prms = it6->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	tt=true;
	}
	if(it7!=argvals.end()){
	prms = it7->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	mac=true;
	}
	if(it8!=argvals.end()){
	prms = it8->second;
	StrUtil::eraseWhiteSpace(prms);
	uniqueparams.insert(prms);
	args.push_back(prms);
	last=true;
	}

	isPara.push_back(uid);
	isPara.push_back(fd);
	isPara.push_back(td);
	isPara.push_back(od);
	isPara.push_back(ft);
	isPara.push_back(tt);
	isPara.push_back(mac);
	isPara.push_back(last);
	vdata.params = &uniqueparams;
/*    if ( !_validate(&vdata)) {
        _getInvalidResponse(response);
        return false;
    }*/

    it1 = argvals.find("type");
    if (it1 != argvals.end()){
        const string outputtype = it1->second;
        if (strcasecmp(outputtype.c_str(), "xml") == 0 ) {
            type = Executor::TYPE_XML;
        }
    }

    return _executeAPI(url, args, type, response,isPara);
}

int queryType(vector<bool> para){
	if(para.at(0)==false){
		return 0;
	}
	if(para.at(1)==true && para.at(2)==true && para.at(3)==false && para.at(4)==true && para.at(5)==true){
		return 1;
	}
	else if(para.at(1)==false && para.at(2)==false && para.at(3)==true && para.at(4)==true && para.at(5)==true){
		return 2;
	}
	else if(para.at(1)==false && para.at(2)==false && para.at(3)==false && para.at(4)==false && para.at(5)==false && para.at(7)==true){
		return 3;
	}
	else
		return 0;
}

bool api::_executeAPI(const string& url, const vector<string>& argvals, 
        Executor::outputType type, string& response,vector<bool> isPara)
{
    bool ret = false;
	if(isPara.at(6)==true)
		ret = _executor.uid(argvals, type, response);
	else if( url == "/connections"){
		ret = _executor.ap_connections(argvals, type, response);
	}
	else if ( url == "/ap/near/"){
		if(isPara.at(0)){
			ret = _executor.ap_near(argvals, type, response);
		}
	}
	else if ( url == "/client/near/"){
		if(isPara.at(0)){
			ret = _executor.client_near(argvals, type, response);
		}
	}
	else if (url == "/ap"){
		switch(queryType(isPara)){
			case 0: break;
			case 1: ret = _executor.ap(argvals, type, response);break;
			case 2: ret = _executor.ap_ondate(argvals, type, response);break;
			case 3: ret = _executor.ap_last(argvals, type, response);break;
		}
	}
	else if (url == "/client"){
		switch(queryType(isPara)){
			case 0:break;
			case 1: ret = _executor.client(argvals, type, response);break;
			case 2: ret = _executor.client_ondate(argvals, type, response);break;
			case 3: ret = _executor.client_last(argvals, type, response);break;
		}

/*
		if(isPara.at(0)==true && isPara.at(1)==false && isPara.at(2)==false )
		        ret = _executor.client(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==true && isPara.at(2)==false )
			ret = _executor.client_from(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==false && isPara.at(2)==true )
			ret = _executor.client_to(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==true && isPara.at(2)==true )
			ret = _executor.client_from_to(argvals, type, response);
*/
	}
    return ret;
}

bool api::_validate(const void *data)
{
/*    const validate_data *vdata = static_cast<const validate_data *>(data );
    map<string, set<string> > ::iterator it =  _apiparams.find(vdata->api);

    it = _apiparams.find(vdata->api);

    if ( it == _apiparams.end()){
        return false;
    }
    set<string>::iterator it2 = vdata->params->begin();
    while (it2 != vdata->params->end()) {
        if (it->second.find(*it2) == it->second.end()) 
            return false;
        ++it2;
    }
    return true;*/
    return true;
}

void api::_getInvalidResponse(string& response)
{
    response = "Some error in your data ";
}

