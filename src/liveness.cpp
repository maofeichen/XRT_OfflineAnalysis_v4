#include <iostream>
#include <list>

#include "flag.h"
#include "liveness.h"
#include "util.h"

using namespace std;

void 
Liveness::analyze_liveness(std::vector<std::string> &log, 
 						   std::vector<std::string> &rslt)
{
	cout << "analyzing liveness..." << endl;

	vector< vector<string>::iterator > call_stack;
	vector<string>::iterator it_call, it_ret;

	int fc = 0;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string cflag	= Util::get_flag(*it);
		bool is_mark	= Util::is_mark(cflag);

		if(is_mark) {
			// if a function call mark
			if(Util::equal_mark(cflag, flag::CALL_INSN) 
			   || Util::equal_mark(cflag, flag::CALL_INSN_FF2) ) {
				call_stack.push_back(it);
			} else if(Util::equal_mark(cflag, flag::RET_INSN_SEC) ) {
				it_ret = it - 1;

				// search the call stack backward to find a match if there is any
				for(auto rit_cs = call_stack.rbegin(); rit_cs != call_stack.rend(); ++rit_cs) {
					string call = **rit_cs;
					string ret  = *it_ret;

					if(Util::is_match_fmark(call, ret) ) {
						cout << "found a match! - call: " << call << " - ret: " << ret << endl;
						// vector<string> func(*rit_cs, it_ret+2);
						// analyze_per_func(func, rslt);	

						// found the matched call mark, delete it from call_stack until end
						// int interval = call_stack.rend() - rit_cs;
						// auto it_cs = call_stack.begin() + interval -1;
						// cout << "call in reverse: " << **rit_cs << "- call in forward: " << **it_cs << endl; 
						// call_stack.erase(it_cs, call_stack.end() );

						// cout << "call stack size: \t" << call_stack.size() << endl;
						// for(auto its = call_stack.begin(); its != call_stack.end(); ++its) {
						// 	cout << **its << endl;
						// }

						int interval = call_stack.rend() - rit_cs;
						del_call_stack(call_stack, interval);

						// cout << "call stack size after delete: \t" << call_stack.size() << endl;
						// for(auto its = call_stack.begin(); its != call_stack.end(); ++its) {
						// 	cout << **its << endl;
						// }

						fc++;
						break;
					} else{}
				}
			} else {}
		} else {}
	}

	cout << "finish analyzing liveness, total function found: " << fc << endl;	
}

void 
Liveness::analyze_per_func(std::vector<std::string> &func,
 						   std::vector<std::string> &rslt)
{
	for(auto it = func.begin(); it != func.end(); ++it) {
		cout << *it << endl;
	}
}

// delete elements given the interval to begin, and until to end
void 
Liveness::del_call_stack(std::vector< std::vector<std::string>::iterator > &call_stack, 
 						 int interval)
{
	auto it_cs = call_stack.begin() + interval -1;
	call_stack.erase(it_cs, call_stack.end() );
}