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

bool api::authenticateAPI( const map<string, string>& argvals, string& response)
{
  map<string, string>:: const_iterator it,it2;
  std::string recieved_token;

  it = argvals.find("token");
  if(it !=argvals.end()){
  	if( !tokenchecking(it->second) ){
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

unsigned int fill_args(const map<string,string> & args, struct args_container & params){
  map<string,string>::const_iterator it = args.begin();
  unsigned int result=0;
  while(it!=args.end()){
    if(it->first == "uid"){
      result |= AUID;
      params.uid = it->second;
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
    else if(it->first == "last"){
      result |= ALAST;
      params.last = it->second;
    }
    else if(it->first == "mac"){
      result |= AMAC;
      params.mac = it->second;
    }
    it++;
  }
  params.type = result;
  return result;
}

bool api::executeAPI(const string& url, const map<string, string>& argvals, string& response){
  // Ignore all the args except the "fields" param 
  Executor::outputType type = Executor::TYPE_JSON;
  args_container params;
  
  // **Later check for duplicate url_arguments**

    /*if (it1 != argvals.end()) {
        string prms = it1->second;
        StrUtil::eraseWhiteSpace(prms);
        StrUtil::splitString(prms, ",", params);   
    }*/
	//Old comment -  Unique params will come handy when, in future we allow for multiple MACs to be sent at once.
	string prms;
  unsigned int api_type = fill_args(argvals,params);
  if(api_type!= VALID_API_STD and api_type != VALID_API_LAST and api_type != VALID_API_MAC){
    response = "Invalid API call";
    return false;
  }

  // erase_whitespace()
  params.erase_whitespace();
/*
	isPara.push_back(uid);
	isPara.push_back(fd);
	isPara.push_back(td);
	isPara.push_back(od);
	isPara.push_back(ft);
	isPara.push_back(tt);
	isPara.push_back(mac);
	isPara.push_back(last);
*/
  //create function to validate data later

  /*if ( !_validate(&vdata)) {
        _getInvalidResponse(response);
        return false;
    }*/

    /*it1 = argvals.find("type");
    if (it1 != argvals.end()){
        const string outputtype = it1->second;
        if (strcasecmp(outputtype.c_str(), "xml") == 0 ) {
            type = Executor::TYPE_XML;
        }
    }
*/
    return _executeAPI(url, params, type, response);
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

bool api::_executeAPI(const string& url, const struct args_container & argvals, 
        Executor::outputType type, string& response)
{
    bool ret = false;
    if(argvals.type == VALID_API_MAC){
      ret = _executor.uid(argvals,type,response);
    }
    else if(argvals.type == VALID_API_LAST){
      ret = _executor.last(argvals,type,response,url); // 'url' checks for '/client' or '/device'
    }
    else if(argvals.type == VALID_API_STD){
      ret = _executor.std(argvals,type,response,url); // 'std' is used to refer to the standrd from date to to date api
    }


    /*
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


		if(isPara.at(0)==true && isPara.at(1)==false && isPara.at(2)==false )
		        ret = _executor.client(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==true && isPara.at(2)==false )
			ret = _executor.client_from(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==false && isPara.at(2)==true )
			ret = _executor.client_to(argvals, type, response);
		else if(isPara.at(0)==true && isPara.at(1)==true && isPara.at(2)==true )
			ret = _executor.client_from_to(argvals, type, response);

	}*/
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

