#include <iostream>
#include "file.h"

using namespace std;

void
xt::File::preprocess() {
	cout << "Input Log: \t" << fp_ << endl;
	cout << "Output Drec: \t" << od_ << endl;
}