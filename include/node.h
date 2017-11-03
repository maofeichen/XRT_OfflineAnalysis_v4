#ifndef NODE_H_
#define NODE_H_

#include <string>

class Node
 {
 public:
 	Node() {};
	Node(uint64_t index, 
		 bool is_mark, 
		 std::string flag, 
		 std::string addr,
       	 std::string val);	
 	~Node() {};

	bool        is_mark();
  	uint64_t    get_index();

  	void        set_flag(std::string flag);
  	std::string get_flag();
  	std::string get_addr();
  	std::string get_val();
  	void        set_int_addr(uint32_t i_addr);
  	uint32_t    get_int_addr();
  	void        set_sz_bit(uint32_t sz_bit);
  	uint32_t    get_sz_bit();
  	uint32_t    get_sz_byte();

  	void print_mem_node();
  	void print_node();

 private:
 	bool 		is_mark_   	= false;
  	uint64_t 	index_ 		= 0;

  	std::string flag_ 		= "";
  	std::string addr_ 		= "";
  	std::string val_  		= "";

  	uint32_t int_addr_		= 0;
  	uint32_t sz_bit_  		= 0;	
 }; 
#endif