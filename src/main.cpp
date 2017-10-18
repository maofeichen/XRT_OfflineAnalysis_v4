#include "boost/program_options.hpp"

// #include "xt_detector.h"
#include "file.h"
#include "util.h"

using namespace boost;
namespace po = boost::program_options;

using namespace std;

void analyze(string &fp, string &od, bool &is_dump){
  xt::File file(fp, od);
  file.read();
}


int main(int argc, char *argv[]) {
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

    analyze(fp, od, is_dump);

    // vector<string> v_fp = Util::split(fp.c_str(), '/');
    // string log_name = v_fp.back();
    // log_name = log_name.substr(0, log_name.size() - 4);
    // cout << "log name: " << log_name << endl;

    // Detector detector(log_name, is_dump);
    // detector.detect();
  } else{
    cout << desc << endl;
    return 1;
  }

  return 0;
}