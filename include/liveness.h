#ifndef LIVENESS_H_
#define LIVENESS_H_

#include <string>
#include <vector>

class Liveness
 {
 public:
 	Liveness() {};
 	~Liveness() {};
 
 	static void analyze_liveness(const std::vector<std::string> &log, 
 								 std::vector<std::string> &rslt);
 }; 
#endif