#include <iostream>
#include "alivefunc.h"
#include "cons.h"

using namespace std;

void 
Alivefunc::insert_buf(const Alivebuf& buf)
{
 	lst_alvbuf_.push_back(buf);
 	num_alvbuf_++;
}

 void 
 Alivefunc::print()
 {
 	cout << "total alive buffers: " << dec << num_alvbuf_ << endl;
 	cout << call_fir_ << endl;
 	cout << call_sec_ << endl;
 	cout << cons::dash_sprtr << endl;
 	for(auto it = lst_alvbuf_.begin(); it != lst_alvbuf_.end(); ++it) {
 		it->print();
 	}
 	cout << ret_fir_ << endl;
 	cout << ret_sec_ << endl;
 }