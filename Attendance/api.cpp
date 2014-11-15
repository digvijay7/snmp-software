#include <string.h>
#include <boost/foreach.hpp>
#include "api.hpp"
#include "sha.h"
#include <iostream>

using namespace ourapi;

api::api()
{
    set<string> params;
}

bool api::authenticateAPI( const map<string, string>& argvals, string& response, bool & is_sudo)
{
  map<string, string>:: const_iterator it,it2;
  std::string recieved_token;

  it = argvals.find("token");
  if(it !=argvals.end()){
  	if( !tokenchecking(it->second,is_sudo) ){
		response="Invalid token";
		return false;
	}
	else{
		return true;
	}
  }
  it = argvals.find("username");
  it2 = argvals.find("password");
  if( it !=argvals.end() && it2 != argvals.end() ){
	recieved_token = generatetoken(it->second,it2->second);
	if(recieved_token==""){
		response="Invalid Username/Password";
		return false;
	}
	else{
  		response=recieved_token;
  		return false;
  	}
  }
  else{
  	response="Invalid API Call";
  	return false;
  }
}


unsigned long fill_args(const map<string,string> & args, struct args_container & params){
  map<string,string>::const_iterator it = args.begin();
  unsigned long result=NO_ARGS;
  while(it!=args.end()){
    if(it->first == "name"){
      result |= ANAME;
      params.name = it->second;
    }
    else if(it->first == "from"){
      result |= AFROM;
      params.from = it->second;
    }
    else if(it->first == "to"){
      result |= ATO;
      params.to = it->second;
    }
    else if(it->first == "format"){
      result |= AFORMAT;
      params.format = it->second;
    }
    else if(it->first == "mac"){
      result |= AMAC;
      params.mac = it->second;
    }
    else if(it->first == "type"){
      result |= ATYPE;
      std::string type = it->second;
    }
    else if(it->first == "email"){
      result |= AEMAIL;
      params.email= it->second;
    }
    else if(it->first == "batch"){
      result |= ABATCH;
      params.batch = it->second;
    }
    else if(it->first == "rollno"){
      result |= AROLLNO;
      params.rollno = it->second;
    }
    it++;
  }
  params.type = result;
  return result;
}

unsigned long url_type(const std::string & url){
  if(url == "/auth"){
    return VALID_URL_AUTH;
  }
  else if(url == "/attendance/get"){
    return VALID_URL_ATTENDANCE_GET;
  }
  else if(url == "/attendance/put"){
    return VALID_URL_ATTENDANCE_PUT;
  }
  else if(url == "/ta/get"){
    return VALID_URL_TA_GET;
  }
  else if(url == "/ta/put"){
    return VALID_URL_TA_PUT;
  }
  return INVALID_URL;
}

  
bool api::executeAPI(const string& url, const map<string, string>& argvals, string& response,bool & is_sudo){
  // Ignore all the args except the "fields" param 
  Executor::outputType type = Executor::TYPE_JSON;
  args_container params;
  
  // **Later check for duplicate url_arguments**

	//Old comment -  Unique params will come handy when, in future we allow for multiple MACs to be sent at once.
	string prms;
  unsigned long args_type = fill_args(argvals,params);
  unsigned long _url_type = url_type(url);
  unsigned long query_type = args_type | _url_type;
  _executor.set_type(query_type);
  if(_url_type == INVALID_URL){ // Check URL
    response = "Invalid API call - invalid URL";
    return false;
  }

  // erase_whitespace()
  params.erase_whitespace();
  //create function to validate data later

  /*if ( !_validate(&vdata)) {
        _getInvalidResponse(response);
        return false;
    }*/
  return _executeAPI(url, params, type, response,is_sudo);
}

bool api::_executeAPI(const string& url, const struct args_container & argvals, 
        Executor::outputType type, string& response,bool & is_sudo)
{
  bool ret = false;
  if(_executor.get_type() == VALID_API_ATTENDANCE_GET_EMAIL){
    ret = _executor.attendance_get_email(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_ATTENDANCE_GET_ROLLNO){
    ret = _executor.attendance_get_rollno(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_TA_GET_ALL){
    ret = _executor.ta_get_all(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_TA_GET_EMAIL){
    ret = _executor.ta_get_email(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_TA_GET_ROLLNO){
    ret = _executor.ta_get_rollno(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_TA_PUT){
    ret = _executor.ta_put(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_ATTENDANCE_PUT){
    ret = _executor.attendance_put(argvals,type,response,url);
  }
  else if(_executor.get_type() == VALID_API_ATTENDANCE_GET_ALL){
    ret = _executor.attendance_get_all(argvals,type,response,url);
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

