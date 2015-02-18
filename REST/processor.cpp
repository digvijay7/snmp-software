#include"pap.hpp"
#include<pqxx/pqxx>

namespace Processor{
  
  void make_fro_tro(pqxx::result & res, std::vector<std::vector<std::string> >  & fro_tro){
    using namespace std;
    vector<sn::log> logs;
    int ap_id,cli_id,type;
    for(unsigned int i=0;i<res.size();i++){
      ap_id = res[i][0].as<int>();
      cli_id = res[i][1].as<int>();
      type = res[i][3].as<int>();
      sn::log l(ap_id,cli_id,res[i][2].as<string>(),type);
      logs.push_back(l);
    }
    sn::process_n_populate(logs,fro_tro);
  }
}

