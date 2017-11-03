#include <iostream>
#include <vector>

#include "record.h"
#include "util.h"

using namespace std;

bool 
Record::init_record(std::string rec) 
{
  if(rec.empty() ) {
    cout << "init record - given string is empty" << endl;
    return false;
  }

  vector<string> v_rec = Util::split(rec.c_str(), '\t');

  string flag = v_rec[0];
  is_mark_ = Util::is_mark(flag);

  if(is_mark_) {
    // src_.print_node();
    string addr = v_rec[1];
    string val  = v_rec[2];
    src_        = Node(index_, is_mark_, flag, addr, val);
    // src_.print_node();
  } else {
    //    cout << "non mark record" << endl;
    //    for(auto it = v_rec.begin(); it != v_rec.end(); ++it) {
    //      cout << *it << endl;
    //    }

    string src_addr = v_rec[1];
    string src_val  = v_rec[2];
    src_            = Node(index_, is_mark_, flag, src_addr, src_val);

    string dst_addr = v_rec[4];
    string dst_val  = v_rec[5];
    dst_            = Node(index_, is_mark_, flag, dst_addr, dst_val);
  }

  return true;
}

bool 
Record::is_mark() { return is_mark_; }

void 
Record::set_index(uint64_t index) { index_ = index; }

uint64_t 
Record::get_index() { return index_; }

Node 
Record::get_src() { return  src_; }

Node 
Record::get_dst() { return dst_; }

void 
Record::print_record() 
{
  cout << "src: ";
  src_.print_node();
  cout << "dst: ";
  dst_.print_node();
}