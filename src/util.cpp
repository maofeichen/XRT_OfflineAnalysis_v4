#include "util.h"

using namespace std;

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