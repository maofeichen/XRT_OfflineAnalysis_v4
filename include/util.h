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
	static bool is_match_fmark(const std::string &call, const std::string &ret);

	static std::string get_flag(const std::string &r);
	static void print_log(const std::vector<std::string> &log);
	static bool is_buf_range(const int &flag);
	static std::vector<std::string> split(const char *s, char c);	
};
#endif