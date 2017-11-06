#ifndef FILE_H_
#define FILE_H_ 

#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "alivebuf.h"
#include "alivefunc.h"
#include "ns.h"

class xt::File 
{
public:
	File(std::string fp, std::string od, bool is_dump, std::string ct) { 
		fp_ = fp; od_ = od; is_dump_ = is_dump; ct_ = ct; 
	};
	~File() {};

	void preproc_read();
	void liveness_read();
	void mergebuf_read();
	void cleanmerge_read();
private:
	const unsigned int MAX_LINE_	= 5000000;	// log_ size

	std::string fp_;
	std::string od_;
	bool is_dump_;
	std::string ct_;
	std::vector<std::string> log_;

	void preproc_flow(std::ofstream &fout, uint64_t &idx);
	void liveness_flow(std::vector<std::string> &rslt);
	void mergebuf_flow(std::vector<std::string> &rslt,
					   std::list<Alivebuf>& lst_rslt);
	void cleanmerge_flow(std::list<Alivefunc>& lst_alvfunc);

	void dump(const std::string s, std::vector<std::string> &out);
	void dump(std::ofstream &fout, std::vector<std::string> &out);
	std::string get_op(const std::string fns);
};
#endif