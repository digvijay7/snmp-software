//This file contains only function definitions required for making small programs for the snmp-software

#include<vector>
#include<string>

namespace snmp{

  class util{
    static std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
    public:
    static std::string mac_formatter(std::string mac);
    static std::string mac_formatter2(std::string mac);
    static void vector_to_lower(std::vector<std::string> &);
    static std::vector<std::string> split (const std::string &, char delim);
  };
};
