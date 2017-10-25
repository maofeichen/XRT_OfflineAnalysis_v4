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