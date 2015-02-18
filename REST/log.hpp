#include<iostream>
#include<ctime>

namespace sn{
  struct log{
    int ap_id,cli_id,type;
    long int ts;
    log(int ap,int cli, std::string timestamp,int trap_type){
      ap_id = ap;
      cli_id = cli;
      type = trap_type;
      tm tm;
      if(timestamp.length() > 0){
        strptime(timestamp.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
        ts = mktime(&tm);
      }
      else {
        std::cerr<<"Log: "<<ap<<","<<cli<<" timestamp.length less than 0"<<std::endl;
      }
    }
    log(int a,int c,long int f, int t){ap_id = a;cli_id = c;ts = f;type = t;}
    std::string get_time(){
      tm * tm = std::localtime(&ts);
      char buf[50];
      strftime(buf,50,"%Y-%m-%d %H:%M:%S",tm);
      return std::string(buf);
    }
    
    void print(){
      std::cout<<ap_id<<" "<<cli_id<<" "<<ts<<" "<<type<<std::endl;
    }
  };
}

