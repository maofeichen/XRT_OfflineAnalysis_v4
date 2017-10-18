#include <iostream>
#include <fstream>

#include "flag.h"
#include "file.h"
#include "util.h"

using namespace std;

void
xt::File::read() {
	cout << "Input Log: \t" << fp_ << endl;
	cout << "Output Drec: \t" << od_ << endl;
	
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fl(fp_.c_str() );

	if(fl.is_open() ) {
		int lc = 0;
		int sc = 0;
		string	line;
		
		while(getline(fl, line) ) {
			if(log_.size() >= MAX_LINE_) {
				preprocess();
				log_.clear();

				sc++;
				cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
			} else {
				log_.push_back(line);
			}
			lc++;
		}
		cout << "finish reading - total lines: \t" << lc << endl;
	} else{
		cout << "read - error open file: \t" << fp_ << endl;
	}

	fl.close();
}

void
xt::File::preprocess() {
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
		}
	}
}

// returns the flag
//		first 2 character of the record
string 
xt::File::get_flag(string &r) {
	return r.substr(0,2);
}