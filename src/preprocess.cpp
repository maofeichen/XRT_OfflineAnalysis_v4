#include "flag.h"
#include "preprocess.h"
#include "util.h"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

void 
Preproc::preprocess(std::vector<std::string> &log,
					uint64_t &idx)
{
	filter_empty_fmark(log);
	filter_invalid_fmark(log);
	filter_insn_mark(log);
	parse_buf_size(log);
	add_idx(log, idx);
}

// If an insn_mark following with a second insn_mark, then first is unused.
// Filter out all unused insn_mark.
void 
Preproc::filter_insn_mark(vector<string> &log)
{
	cout << "filtering unused insn mark..." << endl;

	vector<string> v;
	for(auto it = log.begin(); it != log.end()-1; ++it) {
		auto next = it+1;
		string cf = Util::get_flag(*it);
		string nf = Util::get_flag(*next);

		if(cf.compare(nf) == 0 
			&& Util::equal_mark(cf, flag::INSN_MARK) ) {
			continue;
		} else {
			v.push_back(*it);
		}
	}
	// always saves the last item
	v.push_back(log.back() );

	log.clear();
	log.insert(log.begin(), v.begin(), v.end() );
}

// Empty function mark:
//	CALL_INSN
//	CALL_INSN_SEC
//	RET_INSN
//	RET_INSN_SEC
// fiters out all such empty functoin marks
void 
Preproc::filter_empty_fmark(std::vector<std::string> &log)
{
	cout << "filtering empty function mark..." << endl;

	vector<string> v;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string cf = Util::get_flag(*it);

		if(Util::equal_mark(cf, flag::RET_INSN_SEC) 
			&& (v.size() >= 3) ) {
			string ret = v.back();

			// if empty fmark, last 3rd rec is the call mark
			string call  = v[v.size()-3];
			string callf = Util::get_flag(call);
			if(Util::equal_mark(callf, flag::CALL_INSN) 
				|| Util::equal_mark(callf, flag::CALL_INSN_FF2) ) {
				vector<string> v_call = Util::split(call.c_str(), '\t');
				vector<string> v_ret  = Util::split(ret.c_str(), '\t');
				assert(v_call.size() == v_ret.size() );

				// if esp vals are eaqual, matched marks
				string c_esp = v_call[1];
				string r_esp = v_ret[1];
				if(c_esp.compare(r_esp) == 0) {
					// erase the last 3 marks, and no push
					v.erase(v.end()-3, v.end() );
				} else {
					v.push_back(*it);
				}
			} else {
				v.push_back(*it);
			}
		} else {
			v.push_back(*it);
		}
	}

	log.clear();
	log.insert(log.begin(), v.begin(), v.end() );
}

// If between a matched function call marks, does not contain any data
// flow record, then it is consider invalid. 
// Flters out all such marks, O(n^2)? 
void 
Preproc::filter_invalid_fmark(std::vector<std::string> &log)
{
	cout << "filtering invalid function marks..." << endl;
	vector<string> v;

	vector<string> call_s;
	vector<string>::iterator it_call;
	vector<string>::iterator it_ret;

	bool has_valid	= false;

	for(auto it = log.begin(); it != log.end(); ++it) {
		bool is_del	= false;

		string cf 	 = Util::get_flag(*it);
		bool is_mark = Util::is_mark(cf);

		if(is_mark) {
			if(Util::equal_mark(cf, flag::CALL_INSN) 
				|| Util::equal_mark(cf, flag::CALL_INSN_FF2) ) {

				call_s.push_back(*it);
				has_valid = false;	// init for each call mark, assume no valid

			} else if(Util::equal_mark(cf, flag::RET_INSN_SEC) 
					  && !call_s.empty() ) {
				string call = call_s.back();	
				it_ret  	= it - 1;
				string ret  = *it_ret;

				if(Util::is_match_fmark(call, ret) && !has_valid) {
					call_s.pop_back();

					// del all marks between invalid match call and ret 
					// != 0 due to if they are not same keep pop until same, stop
					string rec = v.back();
					while(rec.compare(call) != 0) {
						v.pop_back();
						rec = v.back();
					}
					v.pop_back();	// pop the last call mark

					is_del = true;
				}

			} else {

			}

		} else {
			has_valid = true;
		}

		if(!is_del) {
			v.push_back(*it);
		}
	} // end for

	log.clear();
	log.insert(log.begin(), v.begin(), v.end() );
}

void 
Preproc::parse_buf_size(vector<string> &log)
{
	cout << "parsing buffer record size..." << endl;

	vector<string> v;
	string sz;
	string o_rec, n_rec;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string cf = Util::get_flag(*it);
		o_rec = *it;

		if(!Util::is_mark(cf) ) {
			int icf = stoi(cf, nullptr, 16);
			if(Util::is_buf_range(icf) ) {
				int tcg_encode = 0;
				sz = get_buf_size(icf, tcg_encode);
				n_rec = update_buf_size(o_rec, sz, tcg_encode);
				v.push_back(n_rec);
			} else {
				v.push_back(*it);
			}
		} else {
			v.push_back(*it);
		}
	}

	log.clear();
	log.insert(log.begin(), v.begin(), v.end() );
}

string 
Preproc::get_buf_size(const int ifg, int &tcg_encode)
{
	int sz = 0;

	if(ifg > flag::NUM_TCG_ST_POINTER) {
		sz 	= ifg - flag::NUM_TCG_ST_POINTER;
		tcg_encode = flag::NUM_TCG_ST_POINTER;
	} else if(ifg > flag::NUM_TCG_ST) {
		sz 	= ifg - flag::NUM_TCG_ST;
		tcg_encode = flag::NUM_TCG_ST;
	} else if(ifg > flag::NUM_TCG_LD_POINTER) {
		sz 	= ifg - flag::NUM_TCG_LD_POINTER;
		tcg_encode = flag::NUM_TCG_LD_POINTER;
	} else if(ifg > flag::NUM_TCG_LD) {
		sz 	= ifg - flag::NUM_TCG_LD;
		tcg_encode = flag::NUM_TCG_LD;
	} else {
		cout << "get_buf_size - unknown flag encode, exit"  << endl;
		exit(1);
	}

	switch(sz) {
		case 1:
			return "8";
		case 2:
			return "16";
		case 3: 
			return "32";
		default:
			cout << "get_buf_size - unknown size, exit" << endl;
			exit(1);
	}
}

string 
Preproc::update_buf_size(const string &rec, const string &sz, const int tcg_encode)
{
	vector<string> v;

	string rec_new;	
	string s_tcg_encode;
	ostringstream o_tcg_encode;

	o_tcg_encode << std::hex << tcg_encode;
	s_tcg_encode = o_tcg_encode.str();

	v = Util::split(rec.c_str(), '\t');

	rec_new = s_tcg_encode + '\t';
	rec_new += v[1] + '\t';
	rec_new += v[2] + '\t';

	rec_new += s_tcg_encode + '\t';
	rec_new += v[4] + '\t';
	rec_new += v[5] + '\t';
	rec_new += sz;

	return rec_new;
}

void 
Preproc::add_idx(std::vector<std::string> &log, uint64_t &idx)
{
	cout << "adding index to log..." << endl;
	vector<string> v;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string cflag = Util::get_flag(*it);
		string s_idx = to_string(idx);

		if(Util::equal_mark(cflag, flag::TCG_QEMU_LD) 
			|| Util::equal_mark(cflag, flag::TCG_QEMU_ST) ) {
			(*it) += '\t';
			(*it) += s_idx;
		} else {
			(*it) += s_idx;
		}

		v.push_back(*it);
		idx++;
	}	

	log.clear();
	log.insert(log.begin(), v.begin(), v.end() );
}