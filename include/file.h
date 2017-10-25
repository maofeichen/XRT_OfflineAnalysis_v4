#ifndef FILE_H_
#define FILE_H_ 

#include <fstream>
#include <string>
#include <vector>

#include "ns.h"

class xt::File 
{
public:
	File(std::string fp, std::string od, bool is_dump, std::string ct) { 
		fp_ = fp; od_ = od; is_dump_ = is_dump; ct_ = ct; 
	};
	~File() {};

	void read();
private:
	const unsigned int MAX_LINE_	= 1000000;	// log_ size

	std::string fp_;
	std::string od_;
	bool is_dump_;
	std::string ct_;
	std::vector<std::string> log_;

	void preprocess();
	void filter_insn_mark();
	void filter_empty_fmark();
	void filter_invalid_fmark();

	void dump(std::ofstream &fout);

	bool is_match_fmark(const std::string &call, const std::string &ret);
	std::string get_flag(const std::string &r);
	std::string get_op();
};
#endif