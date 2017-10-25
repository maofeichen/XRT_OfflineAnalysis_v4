#ifndef UTIL_H
#define UTIL_H 

#include <string>
#include <vector>

class Util
{
public:
	Util() {};
	~Util() {};

	static bool equal_mark(const std::string &flag, const std::string mark);
	static bool is_mark(const std::string &flag);
	static std::vector<std::string> split(const char *s, char c);	
};
#endif