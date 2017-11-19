#include <fstream>
#include <iostream>

#include "flag.h"
#include "help.h"
#include "util.h"

using namespace std;

void 
Help::cmpt_num_addr(std::string& fp)
{
	vector<string> log;
	read(fp, log);
}

void 
Help::read(std::string& fp, std::vector<std::string>& log)
{

	unordered_set<uint32_t> set_of_addr;
	unordered_map<uint32_t,uint32_t> src_map;
	unordered_map<uint32_t,uint32_t> dst_map;
	unordered_map<uint32_t,uint32_t> srctmp_map;
	unordered_map<uint32_t,uint32_t> dsttmp_map;

	uint32_t avg, max, rec_cnt, mem_cnt;

	cout << "reading log :\t" << fp << "..." << endl;

	ifstream fl(fp.c_str() );
	if(fl.is_open() ) {
		uint64_t lc	= 0;
		uint32_t sc = 0;
		rec_cnt = 0;
		mem_cnt = 0;
		cout << "compute num diff addr:\t" 
			 << dec << sc << "\t" 
			 << MAX_REC << "\t lines" << endl;

		string ln;
		while(getline(fl, ln) ) { 
			if(log.size() > MAX_REC) {
				cmpt(log, set_of_addr, src_map, dst_map, srctmp_map, dsttmp_map, rec_cnt, mem_cnt);
				log.clear();
				
				sc++;
				cout << "compute num diff addr:\t" 
					 << dec << sc << "\t" 
					 << MAX_REC << "\t lines" << endl;
			} else {
				log.push_back(ln);
				lc++;
			}
		}
		fl.close();

		cout << "total lines read:\t" << dec << lc << endl;
		cout << "total different addr:\t" << dec << set_of_addr.size() << endl;

		avg = 0;
		max = 0;
		cmpt_stat(avg, max, src_map);
		cout << "source address - mem => mem/register/temp:\n"
			 << "\t max propagation\t" << dec << max << "\n"
			 << "\t avg propagation\t" << dec << avg << endl;

		cmpt_stat(avg, max, dst_map);
		cout << "destination address - mem/register/temp => mem:\n"
			 << "\t max propagation\t" << dec << max << "\n"
			 << "\t avg propagation\t" << dec << avg << endl;

		cmpt_stat(avg, max, srctmp_map);
		cout << "register/temp => mem/register/temp:\n"
			 << "\t max propagation\t" << dec << max << "\n"
			 << "\t avg propagation\t" << dec << avg << endl;

		cmpt_stat(avg, max, dsttmp_map);
		cout << "mem/register/temp => register/temp\n"
			 << "\t max propagation\t" << dec << max << "\n"
			 << "\t avg propagation\t" << dec << avg << endl;

		cout << "total lines with memory access:\t" << dec << mem_cnt << endl;
		cout << "total lines without involving memory:\t" << dec << rec_cnt << endl;
	}else {
		cout << "read - error open file: \t" << fp << endl;
		exit(1);
	}
}

void 
Help::cmpt(std::vector<std::string>& log, 
	 	   std::unordered_set<uint32_t>& set_of_addr,
	 	   std::unordered_map<uint32_t,uint32_t>& src_map,
	 	   std::unordered_map<uint32_t,uint32_t>& dst_map,
	 	   std::unordered_map<uint32_t,uint32_t>& srctmp_map,
		   std::unordered_map<uint32_t,uint32_t>& dsttmp_map,
	 	   uint32_t& rec_cnt,
	 	   uint32_t& mem_cnt)
{
	uint32_t addr = 0;
	uint32_t stmp, dtmp;

	for(auto it = log.begin(); it != log.end(); ++it) {
		string fg = Util::get_flag(*it);

		if(!Util::is_mark(fg ) ) {
			if(Util::equal_mark(fg, flag::TCG_QEMU_ST) ) {
				addr = get_store_addr(*it);

				if(set_of_addr.count(addr) == 0) {
					set_of_addr.insert(addr);
					// cout << "store addr:\t" << hex << addr << endl;
				}

				// compute dst propagations
				if(dst_map.count(addr) == 0) {
					pair<uint32_t,uint32_t> dst_cnt(addr,1);
					dst_map.insert(dst_cnt);
				}else {
					uint32_t cnt = dst_map[addr];
					// cout << "store - find a hit - addr:\t" << hex << addr << "\n"
					// 	 << "\tbefore update:\t" << dec << cnt << "\n";
					cnt++;
					dst_map[addr] = cnt;
					cnt = dst_map[addr];
					// cout << "\tafter update:\t" << dec << cnt << endl;
				}

				mem_cnt++;

			}else if(Util::equal_mark(fg, flag::TCG_QEMU_LD) ) {
				addr = get_load_addr(*it);

				// compute different addr
				if(set_of_addr.count(addr) == 0) {
					set_of_addr.insert(addr);
					// cout << "load addr:\t" << hex << addr << endl;
				}

				// compute src propagations
				if(src_map.count(addr) == 0) {
					pair<uint32_t,uint32_t> src_cnt(addr,1);
					src_map.insert(src_cnt);
				}else {
					uint32_t cnt = src_map[addr];
					cnt++;
					src_map[addr] = cnt;
				}

				mem_cnt++;
			} else {
				rec_cnt++;

				stmp = get_src_tmp(*it);
				if(srctmp_map.count(stmp) == 0) {
					pair<uint32_t,uint32_t> s_cnt(stmp, 1);
					srctmp_map.insert(s_cnt);
				} else {
					uint32_t cnt = srctmp_map[stmp];
					cnt++;
					srctmp_map[stmp] = cnt;
				}

				dtmp = get_dst_tmp(*it);
				if(dsttmp_map.count(dtmp) == 0) {
					pair<uint32_t,uint32_t> d_cnt(dtmp, 1);
					dsttmp_map.insert(d_cnt);
				} else {
					uint32_t cnt = dsttmp_map[dtmp];
					cnt++;
					dsttmp_map[dtmp] = cnt;
				}
			}
		}
	}	
}

void 
Help::cmpt_stat(uint32_t& avg,
		  		uint32_t& max,
		  		std::unordered_map<uint32_t,uint32_t>& addr_map)
{
	uint32_t total_cnt = 0;

	max = addr_map.begin()->second;
	for(auto it = addr_map.begin(); it != addr_map.end(); ++it) {
		total_cnt += it->second;

		if(max < it->second) {
			max = it->second;
		}
	}

	avg = total_cnt / addr_map.size();
}

uint32_t 
Help::get_load_addr(const std::string &r)
{
	vector<string> v_load = Util::split(r.c_str(), '\t');
	string s_addr 	= v_load[1];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}


uint32_t 
Help::get_store_addr(const std::string &r)
{
	vector<string> v_st = Util::split(r.c_str(), '\t');
	string s_addr 	= v_st[4];
	uint32_t addr 	= stoul(s_addr, nullptr, 16);
	// cout << "load addr: " << hex << addr << endl;
	return addr;
}

uint32_t 
Help::get_src_tmp(const std::string& r)
{
	vector<string> v = Util::split(r.c_str(), '\t');
	string s_stmp	= v[1];
	uint32_t stmp 	= stoul(s_stmp, nullptr, 16);
	// cout << "src temp:\t" << hex << stmp << endl;
	return stmp;
}
uint32_t 
Help::get_dst_tmp(const std::string& r)
{
	vector<string> v = Util::split(r.c_str(), '\t');
	string s_dtmp	= v[4];
	uint32_t dtmp 	= stoul(s_dtmp, nullptr, 16);
	// cout << "dst temp:\t" << hex << dtmp << endl;
	return dtmp;
}