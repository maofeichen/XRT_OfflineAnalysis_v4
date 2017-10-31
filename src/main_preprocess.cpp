#include "boost/program_options.hpp"

// #include "xt_detector.h"
#include "file.h"
#include "util.h"

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

void 
analyze(string &fp, string &od, bool &is_dump, string &ct){
  xt::File file(fp, od, is_dump, ct);
  file.preproc_read();
}


int 
main(int argc, char *argv[]) {
  po::options_description desc("Allowed options");

  desc.add_options()
      ("help,h", "produce help message")
      ("input-file,i", po::value< string >(), "input file: file path")
      ("output-drec,o", po::value< string >(), "output file: drec path")
      ("dump-result,d", po::value< bool >(), "dump result: yes/no, 1/0")
      ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // if (vm.count("help")) {
  //   cout << desc << endl;
  //   return 1;
  // }

  if(vm.count("input-file") 
     && vm.count("dump-result") 
     && vm.count("output-drec") ){

    string fp = vm["input-file"].as< string >();
    cout << "Input Log: \t" << fp << endl;

    string od = vm["output-drec"].as< string >();
    cout << "Output Drec: \t" << od << endl;

    bool is_dump = vm["dump-result"].as< bool >();
    cout << "Is Dump: \t" << is_dump << endl;

    string ct = get_time();

    analyze(fp, od, is_dump, ct);

    // Detector detector(log_name, is_dump);
    // detector.detect();
  } else{
    cout << desc << endl;
    return 1;
  }

  return 0;
}