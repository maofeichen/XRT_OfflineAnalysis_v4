#ifndef ALIVEBUF_H_
#define ALIVEBUF_H_

#include <string>
#include <vector>

class Alivebuf
 {
 public:
 	Alivebuf() {};
 	Alivebuf(uint32_t baddr, uint32_t sz, std::vector<std::string> &rec);
 	~Alivebuf() {};

 	uint32_t get_begin_addr() { return baddr_; }
 	uint32_t get_byte_sz() { return byte_sz_; }

 	void set_begin_addr(uint32_t baddr) { baddr_ = baddr; }
 	void set_byte_sz(uint32_t sz) { byte_sz_ = sz; }
 	void insert_rec(std::string &rec) { recs_.push_back(rec); }

 	void clear();
 	bool empty();
 	void print();
 private:
 	uint32_t baddr_ = 0;
 	uint32_t byte_sz_ 	= 0;
 	std::vector<std::string> recs_;
 	
 }; 
#endif