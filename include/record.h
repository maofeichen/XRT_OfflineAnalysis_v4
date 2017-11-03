#ifndef RECORD_H_
#define RECORD_H_

#include "node.h"

class Record
{
public:
	Record() {};
	~Record() {};

  	bool init_record(std::string rec);
  	bool is_mark();

  	void     set_index(uint64_t index);
  	uint64_t get_index();
  	Node 	 get_src();
  	Node 	 get_dst();

  	void 	 print_record();

private:
  bool 		is_mark_   	= false;
  uint64_t 	index_ 		= 0;

  Node src_;
  Node dst_;	
};

#endif