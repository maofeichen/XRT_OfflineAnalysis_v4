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
	std::string fp_;
	std::string od_;

};
#endif