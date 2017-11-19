#ifndef HELP_H_
#define HELP_H_ 

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Help
{
public:
	Help() {};
	~Help() {};

	static void cmpt_num_addr(std::string& fp);	
private:
	static const uint64_t MAX_REC	= 5000000;

	static void read(std::string& fp, std::vector<std::string>& log);

	static void cmpt(std::vector<std::string>& log, 
					 std::unordered_set<uint32_t>& set_of_addr,
					 std::unordered_map<uint32_t,uint32_t>& src_map,
					 std::unordered_map<uint32_t,uint32_t>& dst_map,
					 std::unordered_map<uint32_t,uint32_t>& srctmp_map,
					 std::unordered_map<uint32_t,uint32_t>& dsttmp_map,
					 uint32_t& rec_cnt, 
					 uint32_t& mem_cnt);
	static void cmpt_stat(uint32_t& avg,
						  uint32_t& max,
						  std::unordered_map<uint32_t,uint32_t>& addr_map);

	static uint32_t get_load_addr(const std::string &r);
 	static uint32_t get_store_addr(const std::string &r);
 	static uint32_t get_src_tmp(const std::string& r);
 	static uint32_t get_dst_tmp(const std::string& r);
};

#endif
