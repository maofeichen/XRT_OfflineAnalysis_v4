#include <iostream>
#include <sstream>

#include "alivebuf.h"
#include "cons.h"
#include "flag.h"
#include "liveness.h"
#include "record.h"
#include "util.h"

using namespace std;

void 
Liveness::analyze_liveness(std::vector<std::string> &log, 
 						   std::vector<std::string> &rslt)
{
	cout << "analyzing liveness..." << endl;

	vector< vector<string>::iterator > call_stack;
	vector<string>::iterator it_call, it_ret;

	int fc = 0;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string cflag	= Util::get_flag(*it);
		bool is_mark	= Util::is_mark(cflag);

		if(is_mark) {
			// if a function call mark
			if(Util::equal_mark(cflag, flag::CALL_INSN) 
			   || Util::equal_mark(cflag, flag::CALL_INSN_FF2) ) {
				call_stack.push_back(it);
			} else if(Util::equal_mark(cflag, flag::RET_INSN_SEC) ) {
				it_ret = it - 1;

				// search the call stack backward to find a match if there is any
				for(auto rit_cs = call_stack.rbegin(); rit_cs != call_stack.rend(); ++rit_cs) {
					string call = **rit_cs;
					string ret  = *it_ret;

					if(Util::is_match_fmark(call, ret) ) {
						// cout << "found a fmark match - call: " << call << "\t" << "- ret: " << ret << endl;
						vector<string> fnct_rcrd(*rit_cs, it_ret+2);
						cout << "analyzing liveness for " << fc << "th function call..." << endl;
						analyze_per_func(fnct_rcrd, rslt);	

						// found the matched call mark, delete it from call_stack until end
						int interval = call_stack.rend() - rit_cs;
						del_call_stack(call_stack, interval);

						fc++;
						break;
					} else{}
				}
			} else {}
		} else {}
	}

	cout << "finish analyzing liveness, total function found: " << fc << endl;	
}

void 
Liveness::merge_buf(std::vector<std::string> &log,
 					std::vector<std::string> &rslt,
 					std::list<Alivebuf>& lst_rslt)
{
	cout << cons::star_sprtr << endl;
	cout << "merging continuous buffers..." << endl;
	cout << "function call size: \t" << dec << log.size() << endl;
	cout << cons::star_sprtr << endl;

	vector<string> load;
	vector<string> store;
	vector<string> ld_rslt;
	vector<string> st_rslt;

	list<Alivebuf> lst_ldrslt;
	list<Alivebuf> lst_strslt;

	int sz 	= log.size();

	if(!Util::is_match_fmark(log[0], log[sz-2]) ) {
		cout << "merge_buf - error: given function call & ret do not match" << endl;
		exit(1);
	}

	for(auto it = log.begin()+2; it != log.end()-2; ++it) {
		string cflag = Util::get_flag(*it);

		if(Util::equal_mark(cflag, flag::TCG_QEMU_LD) ) {
			load.push_back(*it);
		} else if(Util::equal_mark(cflag, flag::TCG_QEMU_ST) ) {
			store.push_back(*it);
		} else {
			cout << "merge_buf - error: un-recognize buf type, exit" << endl;
			exit(1);
		}
	}

	if(!load.empty() ) {
		// merge_load_buf(load, rslt);
		merge_ldst_buf(load, ld_rslt, lst_ldrslt, true);
	}

	if(!store.empty() ) {
		// merge_store_buf(store, rslt);
		merge_ldst_buf(store, st_rslt, lst_strslt, false);
	}

	// if(!ld_rslt.empty() || !st_rslt.empty() ) {
	// 	rslt.push_back(log[0]);
	// 	rslt.push_back(log[1]);

	// 	if(!ld_rslt.empty() ) {
	// 		rslt.insert(rslt.end(), ld_rslt.begin(), ld_rslt.end() );
	// 	} 

	// 	if(!st_rslt.empty() ) {
	// 		rslt.insert(rslt.end(), st_rslt.begin(), st_rslt.end() );
	// 	}

	// 	rslt.push_back(log[sz-2]);
	// 	rslt.push_back(log[sz-1]);
	// 	rslt.push_back(cons::star_sprtr);
	// }

	if(!lst_ldrslt.empty() || !lst_strslt.empty() ) {
		rslt.push_back(log[0]);
		rslt.push_back(log[1]);
		rslt.push_back(cons::dash_sprtr);

		if(!lst_ldrslt.empty() ) {
			group_cntns_buf(lst_ldrslt);
			// print_lst_alvbuf(lst_ldrslt);
			// lst_rslt.insert(lst_rslt.end(), lst_ldrslt.begin(), lst_ldrslt.end() );
			store_group_list(rslt, lst_ldrslt);	
		}

		if(!lst_strslt.empty() ) {
			group_cntns_buf(lst_strslt);
			// print_lst_alvbuf(lst_strslt);
			// lst_rslt.insert(lst_rslt.end(), lst_strslt.begin(), lst_strslt.end() );
			store_group_list(rslt, lst_strslt);
		}

		rslt.push_back(log[sz-2]);
		rslt.push_back(log[sz-1]);
		rslt.push_back(cons::star_sprtr);
	}
}

void 
Liveness::print_lst_alvbuf(std::list<Alivebuf>& lst_alvbuf)
{
	cout << "list alive buffers " 
		 << " - total alive buffers : " << dec << lst_alvbuf.size() << endl;
	for(auto it = lst_alvbuf.begin(); it != lst_alvbuf.end(); ++it) {
		it->print();
	}
	// cout << cons::dash_sprtr << endl;
}

void 
Liveness::analyze_per_func(std::vector<std::string> &fnct_rcrd,
 						   std::vector<std::string> &rslt)
{
	// for(auto it = fnct_rcrd.begin(); it != fnct_rcrd.end(); ++it) {
	// 	cout << *it << endl;
	// }

	cout << "function call size: " << fnct_rcrd.size() << endl;

	vector<string> v;
	int sz = fnct_rcrd.size();
	int rc = 0;

	uint32_t esp = get_func_esp(fnct_rcrd[0]);

	// save the call marks
	v.push_back(fnct_rcrd[0]);
	v.push_back(fnct_rcrd[1]);

	for(auto it = fnct_rcrd.begin()+2; it != fnct_rcrd.end() - 2; ++it) {
		string cflag = Util::get_flag(*it);
		uint32_t addr = 0;
		// according to the paper, the alive buffer nust require:
		// 1) alive: its begin addr > current function esp value
		// 2) is updated: is the destination (st)
		// 3) a issue with 2): load needs to be considered due to program can read input at any time
		if(Util::equal_mark(cflag, flag::TCG_QEMU_ST) ) {
			addr = get_store_addr(*it);
			if(is_buf_alive(esp, addr) ) {
				v.push_back(*it);
				rc++;
			}
		} else if(Util::equal_mark(cflag, flag::TCG_QEMU_LD) ) {
			addr = get_load_addr(*it);
			if(is_buf_alive(esp, addr) ) {
				v.push_back(*it);
				rc++;
			} 
		} else {}

	}

	// saves the ret marks
	v.push_back(fnct_rcrd[sz-2]);
	v.push_back(fnct_rcrd[sz-1]);

	if(rc > 0) {
		rslt.insert(rslt.end(), v.begin(), v.end() );
		rslt.push_back(cons::dash_sprtr);
	}

	cout << "result alive records: \t" << dec << rslt.size() << endl;
}

void 
Liveness::merge_ldst_buf(std::vector<std::string> &buf,
 						 std::vector<std::string> &rslt,
 						 list<Alivebuf> &lst_alvbuf,
 					     bool is_ld)
{
	if(is_ld) {
		cout << "merging load buf - num: \t" << buf.size() << endl;
	} else {
		cout << "merging store buf - num: \t" << buf.size() << endl;
	}
	// Util::print_log(buf);

	vector<string> buf_rcrd;

	// list<Alivebuf> lst_alvbuf;
	Alivebuf alvbuf;

	uint32_t b_addr;
	uint64_t b_idx;	
	uint32_t b_byte_sz;
	uint32_t accm_byte_sz;
	// init_ldst_begin_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, buf[0], buf_rcrd, is_ld);
	init_ldst_alive_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, buf[0], alvbuf, is_ld);


	uint32_t interval 	 = 0;	
	uint32_t cntns_ct	 = 0;
	uint64_t p_idx		 = b_idx;
	uint64_t c_idx 		 = 0;

	for(auto it = buf.begin()+1; it != buf.end(); ++it) {
		uint32_t addr  	 = get_ldst_addr(*it, is_ld);
		uint64_t idx  	 = get_idx(*it);
		uint32_t byte_sz = get_byte_sz(*it);

		uint32_t c_addr  = b_addr + accm_byte_sz;
		c_idx = idx;

		if(c_addr > addr) {
			// print_merge_buf(b_addr, accm_byte_sz, buf_rcrd);
			// store_merge_buf(b_addr, accm_byte_sz, buf_rcrd, rslt);
			// re-init
			// init_ldst_begin_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, buf_rcrd, is_ld);

			// lst_alvbuf.push_back(alvbuf);
			store_merge_alvbuf(lst_alvbuf, alvbuf);
			init_ldst_alive_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, alvbuf, is_ld);

			interval = 0;
			cntns_ct = 0;
			p_idx	 = idx;
		} else if( c_addr == addr) {
			// continuous case
			if(cntns_ct == 0) {
				// if first continuous buffers, init interval 
				interval = c_idx - p_idx;
				cntns_ct++;

				// buf_rcrd.push_back(*it);
				accm_byte_sz += byte_sz;

				alvbuf.set_byte_sz(accm_byte_sz);
				alvbuf.insert_rec(*it);
				// success continue
			} else if(cntns_ct > 0) {
				// for rest continuous buffers
				if(interval == (c_idx - p_idx) ) {
					// buf_rcrd.push_back(*it);
					accm_byte_sz += byte_sz;

					alvbuf.set_byte_sz(accm_byte_sz);
					alvbuf.insert_rec(*it);

					cntns_ct++;
				} else {
					// print_merge_buf(b_addr, accm_byte_sz, buf_rcrd);
					// store_merge_buf(b_addr, accm_byte_sz, buf_rcrd, rslt);
					// re-init
					// init_ldst_begin_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, buf_rcrd, is_ld);

					// lst_alvbuf.push_back(alvbuf);
					store_merge_alvbuf(lst_alvbuf, alvbuf);
					init_ldst_alive_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, alvbuf, is_ld);

					interval = 0;
					cntns_ct = 0;
					p_idx	 = idx;
				}
			} else {}

		} else {
			// discontinuous case
			// print_merge_buf(b_addr, accm_byte_sz, buf_rcrd);
			// store_merge_buf(b_addr, accm_byte_sz, buf_rcrd, rslt);
			// re-init
			// init_ldst_begin_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, buf_rcrd, is_ld);

			// lst_alvbuf.push_back(alvbuf);
			store_merge_alvbuf(lst_alvbuf, alvbuf);
			init_ldst_alive_buf(b_addr, b_idx, b_byte_sz, accm_byte_sz, *it, alvbuf, is_ld);

			interval = 0;
			cntns_ct = 0;
			p_idx	 = idx;
		}

		p_idx = idx;
	}

	if(!buf_rcrd.empty() ) {
		// print_merge_buf(b_addr, accm_byte_sz, buf_rcrd);
		// store_merge_buf(b_addr, accm_byte_sz, buf_rcrd, rslt);
	}

	if(!alvbuf.empty() ) {
		store_merge_alvbuf(lst_alvbuf, alvbuf);
		// lst_alvbuf.push_back(alvbuf);
	}
}

void 
Liveness::merge_load_buf(std::vector<std::string> &load,
 						 std::vector<std::string> &rslt)
{
	cout << "merging load buf - num: \t" << load.size() << endl;
}

void 
Liveness::merge_store_buf(std::vector<std::string> &store,
 						  std::vector<std::string> &rslt)
{
	cout << "merging stroe buf - num: \t" << store.size() << endl;
	// Util::print_log(store);
}


// group all alive buffer in the list if any are continuous
void 
Liveness::group_cntns_buf(std::list<Alivebuf> &lst_alvbuf)
{
	cout << "grouping continuous buffers... - total alive buffers: " 
		 << dec << lst_alvbuf.size() << endl;
	// print_lst_alvbuf(lst_alvbuf);

	list<Alivebuf> lst;
	for(auto oit = lst_alvbuf.begin(); oit != lst_alvbuf.end(); ++oit) {
		// cout << "out layer alive buffer: " << endl;
		// oit->print();

		Alivebuf cbuf = Alivebuf(*oit);

		uint32_t caddr = oit->get_begin_addr();
		uint32_t csz   = oit->get_byte_sz();

		for(auto iit = next(oit, 1); iit != lst_alvbuf.end(); ) {
			// cout << "out layer alive buffer: " << endl;
			// oit->print();
			// cout << "in layer alive buffer: " << endl;
			// iit->print();

			uint32_t iaddr = iit->get_begin_addr();
			if( (caddr + csz) == iaddr) {
				// found a continuous buffer, group them and delete the orginal
				update_cntns_buf(cbuf, *iit, csz);
				iit = lst_alvbuf.erase(iit);
				// print_lst_alvbuf(lst_alvbuf);
			} else {
				++iit;
			}
		}

		lst.push_back(cbuf);
	}

	lst_alvbuf.clear();
	lst_alvbuf.insert(lst_alvbuf.begin(), lst.begin(), lst.end() );
	// print_lst_alvbuf(lst_alvbuf);
}

// concatenate the right alive buffer to the left 
void 
Liveness::update_cntns_buf(Alivebuf &l, const Alivebuf &r, uint32_t& byte_sz)
{
	if((l.get_begin_addr() + l.get_byte_sz() ) != r.get_begin_addr() ) {
		cout << "update continuous buffers - error: given two alive buffers are not continuous, exit";
		exit(1);
	}

	// cout << "updating continuous buffers..." << endl;
	l.concatenate_buf(r);
	// l.print();
	byte_sz = l.get_byte_sz();
}

void 
Liveness::init_ldst_begin_buf(uint32_t &b_addr, 
 							  uint64_t &b_idx, 
 							  uint32_t &b_byte_sz, 
 							  uint32_t &accm_byte_sz,
 							  const std::string &rec,
 							  std::vector<std::string> &buf_rcrd,
 							  bool is_ld)
{

	b_addr 	 		= get_ldst_addr(rec, is_ld);
	b_idx  	 		= get_idx(rec);
	b_byte_sz   	= get_byte_sz(rec);
	accm_byte_sz 	= b_byte_sz;

	buf_rcrd.clear();
	buf_rcrd.push_back(rec);
}

void 
Liveness::init_begin_lbuf(uint32_t &b_addr, 
						  uint64_t &b_idx, 
						  uint32_t &b_byte_sz, 
						  uint32_t &accm_byte_sz,
						  const std::string &rec,
						  vector<string> &buf_rcrd)
{
	b_addr 	 		= get_load_addr(rec);
	b_idx  	 		= get_idx(rec);
	b_byte_sz   	= get_byte_sz(rec);
	accm_byte_sz 	= b_byte_sz;
	buf_rcrd.push_back(rec);
}

void 
Liveness::init_ldst_alive_buf(uint32_t &b_addr, 
 							  uint64_t &b_idx, 
 							  uint32_t &b_byte_sz, 
 							  uint32_t &accm_byte_sz,
 							  string &rec,
 							  Alivebuf &alive_buf,
 							  bool is_ld)
{
	b_addr 	 		= get_ldst_addr(rec, is_ld);
	b_idx  	 		= get_idx(rec);
	b_byte_sz   	= get_byte_sz(rec);
	accm_byte_sz 	= b_byte_sz;

	alive_buf.clear();
	alive_buf.set_begin_addr(b_addr);
	alive_buf.set_byte_sz(accm_byte_sz);
	alive_buf.insert_rec(rec);
}

void 
Liveness::store_merge_alvbuf(std::list<Alivebuf> &lst_alive_buf,
 						     Alivebuf &alive_buf)
{
	if(alive_buf.get_byte_sz() <= MIN_BUF_SZ) {
		return;
	}

	lst_alive_buf.push_back(alive_buf);
}

void 
Liveness::store_group_list(std::vector<std::string>& rslt, 
 						   std::list<Alivebuf>& lst_alive_buf)
{

	for(auto it = lst_alive_buf.begin(); it != lst_alive_buf.end(); ++it) {
		string buf_info = it->get_buf_info();
		rslt.push_back(buf_info);
		rslt.insert(rslt.end(), it->get_recs().begin(), it->get_recs().end() );
		rslt.push_back(cons::dash_sprtr);	
	}
}

void 
Liveness::print_merge_buf(uint32_t baddr, uint32_t size, std::vector<std::string> &buf_rcrd)
{
	if(size <= MIN_BUF_SZ) {
		return;
	}
	
	cout << "merged buffer - begin: " << hex << baddr 
		 << " - size: " << dec << size << endl;
	for(auto it = buf_rcrd.begin(); it != buf_rcrd.end(); ++it) {
		cout << *it << endl;
	}

	cout << cons::dash_sprtr << endl;
}


void 
Liveness::store_merge_buf(uint32_t baddr, 
 						 uint32_t size, 
 						 std::vector<std::string> &buf_rcrd, 
 						 std::vector<std::string> &rslt)
{
	if(size <= MIN_BUF_SZ ) {
		return;
	}

	stringstream ss_baddr;
	ss_baddr << "0x" << hex << baddr;
	string s_baddr( ss_baddr.str() );

	// string s_baddr 	= to_string(baddr);
	string s_size 	= to_string(size);
	rslt.push_back(s_baddr + '\t' + s_size);
	rslt.insert(rslt.end(), buf_rcrd.begin(), buf_rcrd.end() );

	rslt.push_back(cons::dash_sprtr);	
}

// delete elements given the interval to begin, and until to end
void 
Liveness::del_call_stack(std::vector< std::vector<std::string>::iterator > &call_stack, 
 						 int interval)
{
	auto it_cs = call_stack.begin() + interval -1;
	call_stack.erase(it_cs, call_stack.end() );
}

uint32_t 
Liveness::get_func_esp(const std::string &call)
{
	vector<string> v_call = Util::split(call.c_str(), '\t');
	string s_esp	= v_call[1];
	uint32_t esp 	= stoul(s_esp, nullptr, 16);
	// cout << "function call esp: \t" << hex << esp << endl;
	return esp;
}

uint32_t 
Liveness::get_ldst_addr(const std::string &r, bool is_ld)
{
	if(is_ld) {
		return get_load_addr(r);
	} else {
		return get_store_addr(r);
	}
}

uint32_t 
Liveness::get_load_addr(const std::string &r)
{
	vector<string> v_load = Util::split(r.c_str(), '\t');
	string s_addr 	= v_load[1];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}


uint32_t 
Liveness::get_store_addr(const std::string &r)
{
	vector<string> v_st = Util::split(r.c_str(), '\t');
	string s_addr 	= v_st[4];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}

uint64_t 
Liveness::get_idx(const std::string &r)
{
	vector<string> v = Util::split(r.c_str(), '\t');
	string s_idx 	 = v.back();
	uint64_t idx 	 = stoull(s_idx, nullptr, 10);
	return idx;
}

uint32_t 
Liveness::get_byte_sz(const std::string &r)
{
	vector<string> v = Util::split(r.c_str(), '\t');
	int sz 			 = v.size();
	string s_bit_sz	 = v[sz-2];
	uint32_t bit_sz  = stoul(s_bit_sz, nullptr, 10);
	return bit_sz / 8;
}

// determines if a buf is alive:
// 1) stack: > current function call esp addr
// 2) heap: true
bool 
Liveness::is_buf_alive(const uint32_t esp, const uint32_t addr)
{
	if(addr > STACK_BEGIN) {
		if(addr > esp) {
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}