#include <cassert>
#include <fstream>
#include <iostream>

#include "cons.h"
#include "flag.h"
#include "file.h"
#include "util.h"

using namespace std;

// Reads the log into a local buffer (log_), if the buf is full, preprossse
// the buffer then flush it. 
// If dump, writes the buffer to a output file before flushing. 
void
xt::File::read() {
	// cout << "Input Log: \t" << fp_ << endl;
	// cout << "Output Drec: \t" << od_ << endl;
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fin(fp_.c_str() );

	if(fin.is_open() ) {
		string op = get_op();	
		ofstream fout(op.c_str() );
		if(is_dump_) {
			if(!fout.is_open() ) {
				cout << "read - error open dump file:\t" << op << endl;
				return; 
			} 
		}

		int lc = 0;
		int sc = 0;
		string	line;
		
		while(getline(fin, line) ) {
			if(log_.size() >= MAX_LINE_) {
				// preprocess();
				// if(is_dump_) {
				// 	dump(fout);
				// }
				// log_.clear();

				help_preprocess(fout);

				sc++;
				cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
			} else {
				log_.push_back(line);
			}
			lc++;
		}

		// preprocess the last
		if(!log_.empty() ) {
			cout << "preprocess last records" << endl;
			// preprocess();
			// if(is_dump_) {
			// 	dump(fout);
			// }
			// log_.clear();
			help_preprocess(fout);
		}
		cout << "finish reading - total lines: \t" << lc << endl;
		
		if(is_dump_) {
			cout << "finish dumping output" << endl;
			fout.close(); 
		}
	} else{
		cout << "read - error open file: \t" << fp_ << endl;
		return;
	}

	fin.close();
}

void 
xt::File::help_preprocess(ofstream &fout)
{
	preprocess();
	if(is_dump_) {
		dump(fout);
	}
	log_.clear();
}

void
xt::File::preprocess() {
	filter_empty_fmark();
	filter_invalid_fmark();
	filter_insn_mark();
}

// If an insn_mark following with a second insn_mark, then first is unused.
// Filter out all unused insn_mark.
void 
xt::File::filter_insn_mark() {
	cout << "filtering unused insn mark..." << endl;

	vector<string> v;
	for(auto it = log_.begin(); it != log_.end()-1; ++it) {
		auto next = it+1;
		string cf = get_flag(*it);
		string nf = get_flag(*next);

		if(cf.compare(nf) == 0 
			&& Util::equal_mark(cf, flag::INSN_MARK) ) {
			continue;
		} else {
			v.push_back(*it);
		}
	}
	// always saves the last item
	v.push_back(log_.back() );

	log_.clear();
	log_.insert(log_.begin(), v.begin(), v.end() );
}

// Empty function mark:
//	CALL_INSN
//	CALL_INSN_SEC
//	RET_INSN
//	RET_INSN_SEC
// fiters out all such empty functoin marks
void 
xt::File::filter_empty_fmark()
{
	cout << "filtering empty function mark..." << endl;

	vector<string> v;

	for(auto it = log_.begin(); it != log_.end(); ++it) {
		string cf = get_flag(*it);

		if(Util::equal_mark(cf, flag::RET_INSN_SEC) 
			&& (v.size() >= 3) ) {
			string ret = v.back();

			// if empty fmark, last 3rd rec is the call mark
			string call  = v[v.size()-3];
			string callf = get_flag(call);
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

	log_.clear();
	log_.insert(log_.begin(), v.begin(), v.end() );
}

// If between a matched function call marks, does not contain any data
// flow record, then it is consider invalid. 
// Flters out all such marks, O(n^2)? 
void 
xt::File::filter_invalid_fmark()
{
	cout << "filtering invalid function marks..." << endl;
	vector<string> v;

	vector<string> call_s;
	vector<string>::iterator it_call;
	vector<string>::iterator it_ret;

	bool has_valid	= false;

	for(auto it = log_.begin(); it != log_.end(); ++it) {
		bool is_del	= false;

		string cf 	 = get_flag(*it);
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

				if(is_match_fmark(call, ret) && !has_valid) {
					call_s.pop_back();

					// del all marks between invalid match call and ret 
					string rec = v.back();
					while(rec.compare(call) == 0) {
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

	log_.clear();
	log_.insert(log_.begin(), v.begin(), v.end() );
}

void 
xt::File::dump(ofstream &fout) {
	if(log_.empty() ) {
		cout << "dump - log vector is empty" << endl;
		return;
	} else {
		for(auto it = log_.begin() ; it != log_.end(); ++it) {
			fout << *it << '\n';
		}	
	}
}

bool 
xt::File::is_match_fmark(const std::string &call, const std::string &ret)
{
	vector<string> v_call = Util::split(call.c_str(), '\t');
	vector<string> v_ret  = Util::split(ret.c_str(), '\t');
	assert(v_call.size() == v_ret.size() );

	// if esp vals are eaqual, matched marks
	string c_esp = v_call[1];
	string r_esp = v_ret[1];
	if(c_esp.compare(r_esp) == 0) {
		return true;
	} else {
		return false;
	}
}

// returns the flag
//		first 2 character of the record
string 
xt::File::get_flag(const string &r) {
	return r.substr(0,2);
}

// returns the output file path
string 
xt::File::get_op() {
	string ln, op;
    vector<string> v_fp = Util::split(fp_.c_str(), '/');

    ln = v_fp.back();
    ln = ln.substr(0, ln.size() - 4);

    op = od_ + ln + cons::preprocess + ct_ + cons::ext;
    cout << "dump file to:\t" << op << endl;

    return op;
}