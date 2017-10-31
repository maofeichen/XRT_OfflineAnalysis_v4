#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "cons.h"
#include "flag.h"
#include "file.h"
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
		cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
		while(getline(fin, line) ) {
			if(log_.size() >= MAX_LINE_) {

				preprocess_flow(fout);
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
			preprocess_flow(fout);
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
xt::File::preprocess_flow(ofstream &fout)
{
	Preproc::preprocess(log_);
	if(is_dump_) {
		dump(fout);
	}
	log_.clear();
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