#include "mui.h"
#include "system/muiconfig.h"
#include <fstream> 
int main() {
  mui::mpi_split_by_app();
  // Option 1: Declare MUI interface and samplers using specialisms (i.e. 1 = 1 dimensional, d = double)
  printf( "Ping started ....");

  //declare interface
  std::vector<std::unique_ptr<mui::uniface<mui::mui_config>>> mui_ifs;
  // for this case we do not need a spatial sampler since we are 
  // accessing all the individual cells we want
  mui::temporal_sampler_exact<mui::mui_config> temporal_sampler;
  mui::point3d push_point;

  std::vector<std::string> ifsName;
  ifsName.emplace_back("ifs");
  mui_ifs=mui::create_uniface<mui::mui_config>( "ping", ifsName );
	
  // setup for writing temp values to file.
  double temp;
  std::ofstream outputFile;
  outputFile.open("TempOut.txt");
  //loop through same number of iterations as the solver
  // would need to change this if you alter timestep or endtime
  for ( int t = 0; t < 1000; t++) {
    // fetch exact values and locations of the points commited this time step
    std::vector<mui::point<mui::mui_config::REAL, 3>> fetch_locs = mui_ifs[0]->fetch_points<mui::mui_config::REAL>( "dataFromOF", t, temporal_sampler ); // Extract the locations stored in the interface at time=0
    std::vector<double> fetch_vals = mui_ifs[0]->fetch_values<mui::mui_config::REAL>( "dataFromOF", t, temporal_sampler );

    // Write values to file
    for(int i = 0; i < fetch_locs.size(); i++){
      outputFile << "Ping fetched temperature value of " << fetch_vals[i] << " at location " << fetch_locs[i][0] << "," << fetch_locs[i][1] << " at time " << t << std::endl;
    }
  }
  outputFile.close();
  return 0;
}
 