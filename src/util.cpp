#include "util.h"

using namespace std;

bool 
Util::equal_mark(string &flag, string mark) {
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