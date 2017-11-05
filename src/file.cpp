#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "cons.h"
#include "flag.h"
#include "file.h"
#include "liveness.h"
#include "preprocess.h"
#include "util.h"

using namespace std;

// Reads the log into a local buffer (log_), if the buf is full, preprocess
// the buffer then flush it. 
// If dump, writes the buffer to a output file before flushing. 
void
xt::File::preproc_read() {
	// cout << "Input Log: \t" << fp_ << endl;
	// cout << "Output Drec: \t" << od_ << endl;
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fin(fp_.c_str() );

	if(fin.is_open() ) {
		string op = get_op(cons::preprocess);	
		ofstream fout(op.c_str() );
		if(is_dump_) {
			if(!fout.is_open() ) {
				cout << "read - error open dump file:\t" << op << endl;
				return; 
			} 
		}

		int lc 		 = 0;
		int sc 		 = 0;
		uint64_t idx = 0;
		string	line;

		cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
		while(getline(fin, line) ) {
			if(log_.size() >= MAX_LINE_) {

				preproc_flow(fout, idx);
				sc++;
				cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
			} else {
				log_.push_back(line);
			}
			lc++;
		}

		// preprocess the rest records 
		if(!log_.empty() ) {
			// cout << "preprocess last records" << endl;
			preproc_flow(fout, idx);
		}

		cout << "finish reading - total lines: \t" << dec << lc 
			 << " - total index: " << dec << idx << endl;
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

// Reads the log into a local buffer (log_), if the buf is full, analyze liveness 
// the buffer then flush it. 
// If dump, writes the buffer to a output file before flushing. 
//
// There is a limit:
//   if the call and ret marks cross log_ boundary, 
//   then we can't handle it currently. 
void
xt::File::liveness_read() 
{
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fin(fp_.c_str() );
	vector<string> rslt;	// the result vec

	if(fin.is_open() ) {
		int lc = 0;
		int sc = 0;
		string	line;

		cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;

		while(getline(fin, line) ) {
			if(log_.size() >= MAX_LINE_) {
				liveness_flow(rslt);
				sc++;
				cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
			} else {
				log_.push_back(line);
			}
			lc++;
		}
		// analyze all rest records 
		if(!log_.empty() ) {
			liveness_flow(rslt);
		}

		cout << "finish reading and analyzing liveness - total lines: \t" << dec << lc << endl;
		cout << "total alive records: \t" << dec << rslt.size() << endl;

		if(is_dump_) {
			string op = get_op(cons::alivemem);	
			ofstream fout(op.c_str() );
			if(fout.is_open() ) {
				dump(fout, rslt);
			} else {
				cout << "liveness read - error open dump file:\t" << op << endl;
				return; 
			}
		}
	} else{
		cout << "liveness_read - error open file: \t" << fp_ << endl;
		return;
	}

	fin.close();
}

void 
xt::File::mergebuf_read()
{
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fin(fp_.c_str() );
	vector<string> rslt;	// the result vec

	if(fin.is_open() ) {
		int fc = 0;
		string	line;

		while(getline(fin, line) ) {
			if(line.compare(cons::dash_sprtr) == 0) {
				mergebuf_flow(rslt);
				fc++;
			} else {
				log_.push_back(line);
			}
		}

		cout << "finish merging continuous buffers - total functions: \t" << fc << endl; 
		// Util::print_log(rslt);

		if(is_dump_) {
			string op = get_op(cons::merge);	
			ofstream fout(op.c_str() );
			if(fout.is_open() ) {
				dump(fout, rslt);
			} else {
				cout << "liveness read - error open dump file:\t" << op << endl;
				return; 
			}
		}
	} else{
		cout << "mergebuf_read- error open file: \t" << fp_ << endl;
		return;
	}

	fin.close();
}

void 
xt::File::preproc_flow(ofstream &fout, uint64_t &idx)
{
	Preproc::preprocess(log_, idx);
	if(is_dump_) {
		dump(fout, log_);
	}
	log_.clear();
}

// Control flow of analyzing liveness while in reading
// Result
//   rslt
void 
xt::File::liveness_flow(std::vector<std::string> &rslt)
{
	Liveness::analyze_liveness(log_, rslt);
	log_.clear();
}

void 
xt::File::mergebuf_flow(std::vector<std::string> &rslt)
{
	Liveness::merge_buf(log_, rslt);
	log_.clear();	
}

void 
xt::File::dump(ofstream &fout, vector<string> &out) {
	if(out.empty() ) {
		cout << "dump - log vector is empty" << endl;
		return;
	} else {
		for(auto it = out.begin() ; it != out.end(); ++it) {
			fout << *it << '\n';
		}	
	}
}

// returns the output file path
string 
xt::File::get_op(const string fns) {
	string ln, op;
    vector<string> v_fp = Util::split(fp_.c_str(), '/');

    ln = v_fp.back();
    ln = ln.substr(0, ln.size() - 4);

    op = od_ + ln + fns + ct_ + cons::ext;
    cout << "dump file to:\t" << op << endl;

    return op;
}