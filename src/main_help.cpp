#include "boost/program_options.hpp"

#include "help.h"

using namespace boost;
namespace po = boost::program_options;

using namespace std;

string 
get_time() {
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );

  string ct = to_string( (now->tm_year + 1900) ) + '-' +
      to_string( (now->tm_mon + 1) ) + '-' +
      to_string(  now->tm_mday) + '-' +
      to_string(  now->tm_hour) + '-' +
      to_string(  now->tm_min);

  ct = "-" + ct;
  return ct;
}

int 
main(int argc, char *argv[]) {
  po::options_description desc("Allowed options");

  desc.add_options()
      ("help,h", "produce help message")
      ("input-file,i", po::value< string >(), "input file: file path")
      ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("input-file") ){

    string fp = vm["input-file"].as< string >();
    // cout << "Input Log: \t" << fp << endl;

    Help::cmpt_num_addr(fp);

  } else{
    cout << desc << endl;
    return 1;
  }

  return 0;
}