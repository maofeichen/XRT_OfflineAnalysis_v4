#ifndef LIVENESS_H_
#define LIVENESS_H_

#include <string>
#include <vector>
#include "alivebuf.h"

class Liveness
 {
 public:
 	Liveness() {};
 	~Liveness() {};
 
 	static void analyze_liveness(std::vector<std::string> &log, 
 								 std::vector<std::string> &rslt);
 	static void merge_buf(std::vector<std::string> &log,
 						  std::vector<std::string> &rslt);
 private:
 	static const uint32_t STACK_BEGIN = 0xb0000000;
 	static const uint32_t MIN_BUF_SZ  = 8;

 	static void analyze_per_func(std::vector<std::string> &func,
 								 std::vector<std::string> &rslt);

 	static void merge_ldst_buf(std::vector<std::string> &buf,
 							   std::vector<std::string> &rslt,
							   std::list<Alivebuf> &lst_alvbuf,
 							   bool is_ld);
 	static void merge_load_buf(std::vector<std::string> &load,
 							   std::vector<std::string> &rslt);
 	static void merge_store_buf(std::vector<std::string> &store,
 							   std::vector<std::string> &rslt);

 	static void init_ldst_begin_buf(uint32_t &b_addr, 
 							    	uint64_t &b_idx, 
 							    	uint32_t &b_byte_sz, 
 							    	uint32_t &accm_byte_sz,
 							    	const std::string &rec,
 							    	std::vector<std::string> &buf_rcrd,
 							    	bool is_ld);
 	static void init_begin_lbuf(uint32_t &b_addr, 
 							    uint64_t &b_idx, 
 							    uint32_t &b_byte_sz, 
 							    uint32_t &accm_byte_sz,
 							    const std::string &rec,
 							    std::vector<std::string> &buf_rcrd);

 	static void init_ldst_alive_buf(uint32_t &b_addr, 
 							    	uint64_t &b_idx, 
 							    	uint32_t &b_byte_sz, 
 							    	uint32_t &accm_byte_sz,
 							    	std::string &rec,
 							    	Alivebuf &alive_buf,
 							    	bool is_ld);
 	static void store_merge_alvbuf(std::list<Alivebuf> &lst_alive_buf,
 								   Alivebuf &alive_buf);

 	static void print_merge_buf(uint32_t baddr, uint32_t size, std::vector<std::string> &buf_rcrd);
 	static void store_merge_buf(uint32_t baddr, 
 							    uint32_t size, 
 							    std::vector<std::string> &buf_rcrd, 
 							    std::vector<std::string> &rslt);

 	static void del_call_stack(std::vector< std::vector<std::string>::iterator > &call_stack, 
 							   int interval);

 	static uint32_t get_func_esp(const std::string &call);
 	static uint32_t get_ldst_addr(const std::string &r, bool is_ld);
 	static uint32_t get_load_addr(const std::string &r);
 	static uint32_t get_store_addr(const std::string &r);
 	static uint64_t get_idx(const std::string &r);
 	static uint32_t get_byte_sz(const std::string &r);
 	static bool is_buf_alive(const uint32_t esp, const uint32_t addr);
 }; 
#endif