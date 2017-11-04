#include <cassert>
#include <iostream>

#include "flag.h"
#include "util.h"

using namespace std;

bool 
Util::equal_mark(const string &flag, const string mark) {
	if(!flag.empty() && !mark.empty() ) {
		if(flag.compare(mark) == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool 
Util::is_mark(const std::string &flag)
{
	if(equal_mark(flag, flag::INSN_MARK) 
		|| equal_mark(flag, flag::CALL_INSN) 
		|| equal_mark(flag, flag::CALL_INSN_SEC) 
		|| equal_mark(flag, flag::CALL_INSN_FF2) 
		|| equal_mark(flag, flag::CALL_INSN_FF2_SEC) 
		|| equal_mark(flag, flag::RET_INSN) 
		|| equal_mark(flag, flag::RET_INSN_SEC) ) {
		return true;
	} else {
		return false;
	}
}

bool 
Util::is_match_fmark(const std::string &call, const std::string &ret)
{
	vector<string> v_call = split(call.c_str(), '\t');
	vector<string> v_ret  = split(ret.c_str(), '\t');
	assert(v_call.size() == v_ret.size() );

	// if esp vals are eaqual, matched marks
	string c_esp = v_call[1];
	string r_esp = v_ret[1];
	if(c_esp.compare(r_esp) == 0) {
		return true;
	} else {
		return false;
	}
}

// returns the flag
//		first 2 character of the record
string 
Util::get_flag(const std::string &r)
{
	return r.substr(0,2);
}

void 
Util::print_log(const std::vector<std::string> &log)
{
	for(auto it = log.begin(); it != log.end(); ++it) {
		cout << *it << endl;
	}
}

bool 
Util::is_buf_range(const int &flag)
{
	if(flag >= flag::NUM_TCG_LD_MIN 
		&& flag <= flag::NUM_TCG_ST_MAX) {
		return true;
	} else {
		return false;
	}
}

vector<std::string> 
Util::split(const char *s, char c)
{
	vector<string> v;

	do {
		const char *b = s;
		while(*s != c && *s)
		  s++;

		v.push_back(string(b, s) );
	} while (*s++ != 0);

	return v;
}	