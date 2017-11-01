#ifndef LIVENESS_H_
#define LIVENESS_H_

#include <string>
#include <vector>

class Liveness
 {
 public:
 	Liveness() {};
 	~Liveness() {};
 
 	static void analyze_liveness(std::vector<std::string> &log, 
 								 std::vector<std::string> &rslt);
 private:
 	static void analyze_per_func(std::vector<std::string> &func,
 								 std::vector<std::string> &rslt);
 	static void del_call_stack(std::vector< std::vector<std::string>::iterator > &call_stack, 
 							   int interval);
 }; 
#endif