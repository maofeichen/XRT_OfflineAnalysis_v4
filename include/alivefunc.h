#ifndef ALIVEFUNC_H_
#define ALIVEFUNC_H_

#include <list>
#include <string>
#include "alivebuf.h"

class Alivefunc
 {
 public:
 	Alivefunc() {};
 	~Alivefunc() {};

 	std::string get_call_fir() { return call_fir_; }
 	std::string get_call_sec() { return call_sec_; }
 	std::string get_ret_fir() { return ret_fir_; }
	std::string get_ret_sec() { return ret_sec_; }

	void set_call_fir(const std::string& s) { call_fir_ = s; }
	void set_call_sec(const std::string& s) { call_sec_ = s; }
	void set_ret_fir(const std::string& s) { ret_fir_ = s; }
	void set_ret_sec(const std::string& s) { ret_sec_ = s; }

	void insert_buf(const Alivebuf& buf);
	void print(); 
 private:
 	std::string call_fir_ = "";
 	std::string call_sec_ = "";
 	std::string ret_fir_  = "";
 	std::string ret_sec_  = "";
 
 	uint32_t num_alvbuf_  = 0;
 	std::list<Alivebuf> lst_alvbuf_;

 }; 
#endif