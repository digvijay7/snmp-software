#include "util.hpp"
#include<algorithm>
#include<sstream>


std::vector<std::string> & snmp::util::split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::vector<std::string> snmp::util::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

void snmp::util::vector_to_lower(std::vector<std::string> &elems){
  for(unsigned int i=0;i<elems.size();i++){
    std::string *temp = &elems.at(i);
    std::transform (temp->begin(),temp->end(),temp->begin(),::tolower);
  }
}

std::string snmp::util::mac_formatter(std::string mac){
  std::string new_mac;
  int flag=0;
  for(unsigned int i=0;i<17;i++){
    if(mac[i] == '0' && !flag){
      flag=1;
      continue;
    }
    new_mac += mac[i];
    flag = 1;
    if(mac[i] == ':')
      flag=0;
  }
  return new_mac;
}
std::string snmp::util::mac_formatter2(std::string mac){
  std::transform(mac.begin(),mac.end(),mac.begin(),::tolower);
  std::string new_mac;
  for(unsigned int i=0;i<12;i++){
    if(mac[i] == '0' && i%2 == 0){
      continue;
    }
    new_mac += mac[i];
    if(i%2==1 && i!=11)
      new_mac += ":";
  }
  return new_mac;
}

