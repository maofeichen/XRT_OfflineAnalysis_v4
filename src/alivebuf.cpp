#include <iostream>
#include <sstream>

#include "alivebuf.h"
#include "cons.h"

using namespace std;

Alivebuf::Alivebuf(uint32_t baddr, uint32_t sz, std::vector<std::string> &rec)
{
	baddr_ 		= baddr;
	byte_sz_	= sz;
	recs_.insert(recs_.begin(), rec.begin(), rec.end() );
}

Alivebuf::Alivebuf(const Alivebuf &r)
{
	baddr_ = r.get_begin_addr();
	byte_sz_ = r.get_byte_sz();
	recs_.insert(recs_.begin(), r.get_recs().begin(), r.get_recs().end() );
}

string 
Alivebuf::get_buf_info()
{
	stringstream ss_baddr;
	ss_baddr << "0x" << hex << baddr_;
	string s_baddr( ss_baddr.str() );

	string s_size 	= to_string(byte_sz_);
	return s_baddr + '\t' + s_size;
}

void 
Alivebuf::concatenate_buf(const Alivebuf& r)
{
	uint32_t r_sz 	= r.get_byte_sz();
	byte_sz_ 		+= r_sz;
	recs_.insert(recs_.end(), r.get_recs().begin(), r.get_recs().end() );
}

bool 
Alivebuf::compare(const Alivebuf& r)
{
	if(baddr_ == r.get_begin_addr() && byte_sz_ == r.get_byte_sz() ) {
		vector<string> r_recs = r.get_recs();

		if(recs_.size() == r_recs.size() ) {
			int sz = recs_.size();
			for(int i = 0; i < sz; i++) {
				if(recs_[i].compare(r_recs[i]) != 0) {
					return false;
				}
			}

			return true;
		}
	}

	return false;
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