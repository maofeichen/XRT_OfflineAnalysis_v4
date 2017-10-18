#ifndef FILE_H_
#define FILE_H_ 

#include <string>
#include <vector>

#include "ns.h"

class xt::File 
{
public:
	File(std::string fp, std::string od) { fp_ = fp; od_ = od; };
	~File() {};

	void preprocess();

private:
	const unsigned int MAX_LINE_	= 500000;

	std::string fp_;
	std::string od_;
	std::vector<std::string> log_;

	void read();

};
#endif