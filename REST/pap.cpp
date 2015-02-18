#include"pap.hpp"
#include<map>

namespace sn{
  // This is a helper class to maintain which device an Access Point or Client is
  // connected to and at what time.
  // TBD: "file" has to be made a separate class
  class connected{ 
    int device_id; // Device Connected to
    long int timestamp; // Time of connection
    long int time_file_transferred;
    bool is_it_connected,file;
    public:
    connected(){device_id=-1;timestamp=-1;is_it_connected=false;file=false;};
    int connected_to(){return device_id;};
    bool has_file(){return file;};
    void receive_file(){file=true;};
    long int connected_at(){return timestamp;};
    bool is_connected(){ return is_it_connected;};
    void set_is_connected(bool value){ is_it_connected = value;};
    void got_connected_to(int d_id){device_id = d_id;};
    void got_connected_at(long int ts){timestamp = ts;};
    void reset(){is_it_connected = false;};
    bool operator==(const connected &o)const {
        return device_id == o.device_id;
    }
    bool operator<(const connected &o) const{
        return device_id < o.device_id;
    }
  };

  struct connected_clients{ // this is used in the form of an array. 
                            // Here the array is idnexed by the Access Point ID
                            // And a map of "connections" is kept, which is keyed by 
                            // the client_id and the second number represents the time connected for
    int ap_id;
    std::map<int,long int> connection;
  };
  struct waypoint_connected_clients{
    std::map<int,std::pair<long int, int> > connection;
  };

  int get_change_in_status(connected (& users)[20000],std::vector<log>::iterator l){
    int client_id = l->cli_id;
    int access_point = l->ap_id;
    long int timestamp = l->ts;
    int trap_type = l->type;
    int status = 0;
    if( ! (users[client_id].is_connected()) and trap_type != 1){
    }
    else if( ! (users[client_id].is_connected()) and trap_type == 1){
      status = 1;
    }
    else if( (users[client_id].connected_to() == access_point) and (trap_type %2 != 0) ){
      status = 2;
    }
    else if( (users[client_id].connected_to() == access_point) and (trap_type%2 ==0) ){
      status = 3;
    }
    // Cases where access_point ids are different
    else if( trap_type == 1){
      // Connected to new access point w/o disconnecting from previous AP
      status = 4;
    }
    else if(trap_type == 3){
      std::cerr<<"Trap type 3 encounterred. Ignoring.\n";
    }
    else{
      status = -1;
      std::cerr<<"Should never reach here, in pap.cpp::update_users()\n";
      std::cerr<<"Log:"<<access_point<<" "<<client_id<<" "<<timestamp;
      std::cerr<<" "<<trap_type<<std::endl;
    }
    return status;
  }

  std::string get_time(long int ts){
    tm * tm = std::localtime(&ts);
    char buf[50];
    strftime(buf,50,"%Y-%m-%d %H:%M:%S",tm);
    return std::string(buf);
  }
  void update_users(connected (& users)[20000],std::vector<log>::iterator l,int status,
  std::vector<std::vector<std::string> >  & fro_tro){
    int client_id = l->cli_id;
    int access_point = l->ap_id;
    long int timestamp = l->ts;
    int trap_type = l->type;
    if(status <= 0 or status == 2) return;
    else if( status == 1){
      users[client_id].got_connected_to(access_point);
      users[client_id].got_connected_at(timestamp);
      users[client_id].set_is_connected(true);
    }
    else if( status == 3){
      // Disconnected
      long int time_connected; // Can be used in the future
      time_connected = timestamp - users[client_id].connected_at();
      std::vector<std::string> entry;
      entry.push_back(std::to_string(access_point));
      entry.push_back(std::to_string(client_id));
      entry.push_back(get_time(users[client_id].connected_at()));
      entry.push_back(get_time(timestamp));
      fro_tro.push_back(entry);
      users[client_id].reset();
    }
    // Cases where access_point ids are different
    else if( status == 4){
      // Connected to new access point
      long int time_connected; // Can be used in the future
      time_connected = timestamp - users[client_id].connected_at();
      std::vector<std::string> entry;
      entry.push_back(std::to_string(users[client_id].connected_to()));
      entry.push_back(std::to_string(client_id));
      entry.push_back(get_time(users[client_id].connected_at()));
      entry.push_back(get_time(timestamp));
      std::cout<<entry[0]<<entry[1]<<entry[2]<<entry[3]<<std::endl;
      fro_tro.push_back(entry);
      users[client_id].got_connected_to(access_point);
      users[client_id].got_connected_at(timestamp);
    }
    else{
      std::cerr<<"Unknown status received(>5):"<<status<<std::endl;
    }
    return;
  }
  void update_access_points(connected_clients (& access_points)[20000],
  std::vector<log>::iterator l, int status,int previous_access_point,
  std::vector<std::vector<std::string> >  & fro_tro){
    using namespace std;
    int client_id = l->cli_id;
    int access_point_id = l->ap_id;
    long int timestamp = l->ts;
    int trap_type = l->type;
    //vector<edge> edges;
    connected_clients & access_point = access_points[access_point_id];
    if(status <= 0 or status == 2) {}
    else if( status == 1){
      map<int,long int>:: iterator it = access_point.connection.find(client_id);
      if(it!=access_point.connection.end()){
        //Error, 1st time the client has connected, so it should not be found
        cerr<<client_id<< " got connected to "<<access_point_id;
        cerr<< " for the first time. Should not be found\n";
        // throw error
        throw 1; //Throwing 1 until I know best practice :P
      }
      access_point.connection[client_id] = timestamp;
    }
    else if( status == 3){
      // Disconnected
      map<int,long int> :: iterator it;
      // Generate edges
      for(it = access_point.connection.begin();it!=access_point.connection.end();
      it++){
        vector<string> entry;
        if(it->first != client_id){

          // edge e(client_id,it->first,timestamp - it->second);
/*          e.u = client_id;
          e.v = it->first;
          e.weight = timestamp - it->second;*/
          //edges.push_back(e);
        }
      }
      // Remove entry
      access_point.connection.erase(client_id);
    }
    // Cases where access_point ids are different
    else if( status == 4){
      // Connected to new access point
      map<int,long int> :: iterator it;
      // Generate edges
      for( it = access_points[previous_access_point].connection.begin();
      it!=access_points[previous_access_point].connection.end(); 
      it++ ){
        if(it->first != client_id){
          //edge e(client_id,it->first,timestamp - it->second);
/*          e.u = client_id;
          e.v = it->first;
          e.weight = timestamp - it->second;*/
          //edges.push_back(e);
        }
      }
      // Remove entry
      access_points[previous_access_point].connection.erase(client_id);
      // Add to new access_point
      access_point.connection[client_id] = timestamp;
    }
    else{
      std::cerr<<"Unknown status received(>5):"<<status<<std::endl;
    }
//    return edges;
  }

  // Function source. Function defined in pap.hpp, as static
  bool process_n_populate(std::vector<log> & data, std::vector<std::vector<std::string> >  &fro_tro){
    using namespace std;

    connected users[20000]; // To maintain who clients are connected to
    //connected_clients access_points[20000]; // To maintain who access points are
                                            // connected to
    for(vector<log>::iterator it = data.begin(); it!=data.end();it++){
      int status = get_change_in_status(users,it);
      cout<<">"<<status<<endl;
      // Following line has to appear before any "update" functions
      int previous_access_point_id = users[it->cli_id].connected_to();
      // End - last comment
      update_users(users,it,status,fro_tro);
      // update_access_points not required as of now 18/02/2015
      //update_access_points(access_points,it,status,
      //previous_access_point_id, fro_tro);
    }
    // Finally all the users who are still connected need to be
    // need to be added to fro and tro

  }
}
