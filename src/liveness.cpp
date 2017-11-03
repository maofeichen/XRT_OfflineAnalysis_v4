#include <iostream>
#include <list>

#include "cons.h"
#include "flag.h"
#include "liveness.h"
#include "record.h"
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
						// cout << "found a fmark match - call: " << call << "\t" << "- ret: " << ret << endl;
						vector<string> fnct_rcrd(*rit_cs, it_ret+2);
						cout << "analyzing liveness for " << fc << "th function call..." << endl;
						analyze_per_func(fnct_rcrd, rslt);	

						// found the matched call mark, delete it from call_stack until end
						int interval = call_stack.rend() - rit_cs;
						del_call_stack(call_stack, interval);

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
Liveness::merge_buf(std::vector<std::string> &log,
 					std::vector<std::string> &rslt)
{
	cout << "merging continuous buffers..." << endl;
	cout << "function call size: \t" << dec << log.size() << endl;

	vector<string> load;
	vector<string> store;

	int sz 	= log.size();

	if(!Util::is_match_fmark(log[0], log[sz-2]) ) {
		cout << "merge_buf - error: given function call & ret do not match" << endl;
		exit(1);
	}

	for(auto it = log.begin()+2; it != log.end()-2; ++it) {
		string cflag = Util::get_flag(*it);

		if(Util::equal_mark(cflag, flag::TCG_QEMU_LD) ) {
			load.push_back(*it);
		} else if(Util::equal_mark(cflag, flag::TCG_QEMU_ST) ) {
			store.push_back(*it);
		} else {
			cout << "merge_buf - error: un-recognize buf type, exit" << endl;
			exit(1);
		}
	}

	if(!load.empty() ) {
		merge_load_buf(load, rslt);
	}

	if(!store.empty() ) {
		merge_store_buf(store, rslt);
	}
}

void 
Liveness::analyze_per_func(std::vector<std::string> &fnct_rcrd,
 						   std::vector<std::string> &rslt)
{
	// for(auto it = fnct_rcrd.begin(); it != fnct_rcrd.end(); ++it) {
	// 	cout << *it << endl;
	// }

	cout << "function call size: " << fnct_rcrd.size() << endl;

	vector<string> v;
	int sz = fnct_rcrd.size();
	int rc = 0;

	uint32_t esp = get_func_esp(fnct_rcrd[0]);

	// save the call marks
	v.push_back(fnct_rcrd[0]);
	v.push_back(fnct_rcrd[1]);

	for(auto it = fnct_rcrd.begin()+2; it != fnct_rcrd.end() - 2; ++it) {
		string cflag = Util::get_flag(*it);
		uint32_t addr = 0;
		// according to the paper, the alive buffer nust require:
		// 1) alive: its begin addr > current function esp value
		// 2) is updated: is the destination (st)
		// 3) a issue with 2): load needs to be considered due to program can read input at any time
		if(Util::equal_mark(cflag, flag::TCG_QEMU_ST) ) {
			addr = get_store_addr(*it);
			if(is_buf_alive(esp, addr) ) {
				v.push_back(*it);
				rc++;
			}
		} else if(Util::equal_mark(cflag, flag::TCG_QEMU_LD) ) {
			addr = get_load_addr(*it);
			if(is_buf_alive(esp, addr) ) {
				v.push_back(*it);
				rc++;
			} 
		} else {}

	}

	// saves the ret marks
	v.push_back(fnct_rcrd[sz-2]);
	v.push_back(fnct_rcrd[sz-1]);

	if(rc > 0) {
		rslt.insert(rslt.end(), v.begin(), v.end() );
		rslt.push_back(cons::separator);
	}

	cout << "result alive records: \t" << dec << rslt.size() << endl;
}

void 
Liveness::merge_load_buf(std::vector<std::string> &load,
 						 std::vector<std::string> &rslt)
{
	cout << "merging load buf - num: \t" << load.size() << endl;

	
}

void 
Liveness::merge_store_buf(std::vector<std::string> &store,
 						  std::vector<std::string> &rslt)
{
	cout << "merging stroe buf - num: \t" << store.size() << endl;
}


// delete elements given the interval to begin, and until to end
void 
Liveness::del_call_stack(std::vector< std::vector<std::string>::iterator > &call_stack, 
 						 int interval)
{
	auto it_cs = call_stack.begin() + interval -1;
	call_stack.erase(it_cs, call_stack.end() );
}

uint32_t 
Liveness::get_func_esp(const std::string &call)
{
	vector<string> v_call = Util::split(call.c_str(), '\t');
	string s_esp	= v_call[1];
	uint32_t esp 	= stoul(s_esp, nullptr, 16);
	// cout << "function call esp: \t" << hex << esp << endl;
	return esp;
}
uint32_t 
Liveness::get_load_addr(const std::string &r)
{
	vector<string> v_load = Util::split(r.c_str(), '\t');
	string s_addr 	= v_load[1];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}


uint32_t 
Liveness::get_store_addr(const std::string &r)
{
	vector<string> v_st = Util::split(r.c_str(), '\t');
	string s_addr 	= v_st[4];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}

// determines if a buf is alive:
// 1) stack: > current function call esp addr
// 2) heap: true
bool 
Liveness::is_buf_alive(const uint32_t esp, const uint32_t addr)
{
	if(addr > STACK_BEGIN) {
		if(addr > esp) {
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}