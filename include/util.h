#ifndef UTIL_H
#define UTIL_H 

#include <string>
#include <vector>

class Util
{
public:
	Util() {};
	~Util() {};

	static bool equal_mark(std::string &flag, std::string mark);
	static std::vector<std::string> split(const char *s, char c);	
};
#endif