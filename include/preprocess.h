#ifndef PREPROCESS_H_
#define PREPROCESS_H_ 

#include <string>
#include <vector>

// store byte unit <addr, val> info
typedef struct ByteAddrVal_
{
  std::string addr;
  std::string val;
} ByteAddrVal;

class Preproc
{
public:
	Preproc() {};
	~Preproc() {};

	static void preprocess(std::vector<std::string> &log, uint64_t &idx);
	static void printlog(std::vector<std::string> &log);
	
private:
    // a maximum byte value in string, its max len
    static const unsigned int BYTE_VAL_STR_LEN = 2;

	static void filter_insn_mark(std::vector<std::string> &log);
	static void filter_empty_fmark(std::vector<std::string> &log);
	static void filter_invalid_fmark(std::vector<std::string> &log);

	static void parse_buf_size(std::vector<std::string> &log);
	static std::string get_buf_size(const int ifg, int &tcg_encode);
	static std::string update_buf_size(const std::string &rec, const std::string &sz, const int tcg_encode);

	static void split_multibyte_record(std::vector<std::string> &log);
	static void parse_multibyte_record(std::string record, std::vector<std::string> &result);
	static void parse_multibyte_ld(std::string record, std::vector<std::string> &result);
	static void parse_multibyte_st(std::string record, std::vector<std::string> &result);
	static std::vector<ByteAddrVal> split_multibyte_mem(std::string addr, std::string val, int bytesz);

	static void add_idx(std::vector<std::string> &log, uint64_t &idx);
};
#endif
