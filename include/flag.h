#ifndef FLAG_H_
#define FLAG_H_ 

#include <string>

namespace flag {
	const std::string INSN_MARK	= "32";

	const std::string CALL_INSN           = "14";
	const std::string CALL_INSN_SEC       = "15";
	const std::string CALL_INSN_FF2       = "1a";
	const std::string CALL_INSN_FF2_SEC   = "1b";

	const std::string RET_INSN            = "18";
	const std::string RET_INSN_SEC        = "19";	
}

#endif