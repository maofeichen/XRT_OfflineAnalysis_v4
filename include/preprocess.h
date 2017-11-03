#ifndef PREPROCESS_H_
#define PREPROCESS_H_ 

#include <string>
#include <vector>

class Preproc
{
public:
	Preproc() {};
	~Preproc() {};

	static void preprocess(std::vector<std::string> &log, uint64_t &idx);
	
private:
	static void filter_insn_mark(std::vector<std::string> &log);
	static void filter_empty_fmark(std::vector<std::string> &log);
	static void filter_invalid_fmark(std::vector<std::string> &log);

	static void parse_buf_size(std::vector<std::string> &log);
	static std::string get_buf_size(const int ifg, int &tcg_encode);
	static std::string update_buf_size(const std::string &rec, const std::string &sz, const int tcg_encode);

	static void add_idx(std::vector<std::string> &log, uint64_t &idx);
};
#endif