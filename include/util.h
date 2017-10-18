#ifndef UTIL_H
#define UTIL_H 

#include <string>
#include <vector>

class Util
{
public:
	Util() {};
	~Util() {};

	static std::vector<std::string> split(const char *s, char c);	
};
#endif