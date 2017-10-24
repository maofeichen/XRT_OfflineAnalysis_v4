#include <fstream>
#include <iostream>

#include "cons.h"
#include "flag.h"
#include "file.h"
#include "util.h"

using namespace std;

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
				preprocess();
				if(is_dump_) {
					dump(fout);
				}
				log_.clear();

				sc++;
				cout << "read " << sc << "\t" << MAX_LINE_ << " lines" << endl;
			} else {
				log_.push_back(line);
			}
			lc++;
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
		} else {
			v.push_back(*it);
		}
	}
	// always saves the last item
	v.push_back(log_.back() );

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

// returns the flag
//		first 2 character of the record
string 
xt::File::get_flag(string &r) {
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