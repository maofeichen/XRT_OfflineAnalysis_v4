#include <iostream>
#include <fstream>

#include "file.h"

using namespace std;

void
xt::File::preprocess() {
	cout << "Input Log: \t" << fp_ << endl;
	cout << "Output Drec: \t" << od_ << endl;

	read();
}

void
xt::File::read() {
	cout << "reading log :\t" << fp_ << "..." << endl;

	ifstream fl(fp_.c_str() );

	if(fl.is_open() ) {
		int lc = 0;
		int sc = 0;
		string	line;
		
		while(getline(fl, line) ) {
			if(log_.size() >= MAX_LINE_) {
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