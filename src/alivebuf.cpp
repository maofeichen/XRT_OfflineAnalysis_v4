#include <iostream>
#include "alivebuf.h"
#include "cons.h"

using namespace std;

Alivebuf::Alivebuf(uint32_t baddr, uint32_t sz, std::vector<std::string> &rec)
{
	baddr_ 		= baddr;
	byte_sz_	= sz;
	recs_.insert(recs_.begin(), rec.begin(), rec.end() );
}

void 
Alivebuf::clear()
{
	baddr_ 	 = 0;
	byte_sz_ = 0;
	recs_.clear();
}

bool 
Alivebuf::empty()
{
	if(baddr_ != 0 
		&& byte_sz_ != 0 
		&& !recs_.empty() ) {
		return false;
	} else {
		return true;
	}
}

void 
Alivebuf::print()
{
	cout << "begin: 0x" << hex << baddr_ 
		 << " - byte: " << dec << byte_sz_ << endl;
	for(auto it = recs_.begin(); it != recs_.end(); ++it) {
		cout << *it << endl;
	} 
	cout << cons::dash_sprtr << endl;
}