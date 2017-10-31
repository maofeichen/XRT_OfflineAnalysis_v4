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

	void preproc_read();
private:
	const unsigned int MAX_LINE_	= 5000000;	// log_ size

	std::string fp_;
	std::string od_;
	bool is_dump_;
	std::string ct_;
	std::vector<std::string> log_;

	void preprocess_flow(std::ofstream &fout);
	void dump(std::ofstream &fout);

	std::string get_op();
};
#endif