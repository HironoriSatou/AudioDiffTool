#include "pch.h"
#include "CLIWrapper.h"
#include <msclr/marshal_cppstd.h>

namespace CLIWrapper {
	int CLIWrapper::ADT(AudioDiffToolResult* result, String^ input1, String^ input2) {
		std::string input1_string = msclr::interop::marshal_as<std::string>(input1);
		std::string input2_string = msclr::interop::marshal_as<std::string>(input2);
		CompareSoundDispResult(result, input1_string, input2_string);
	}
}
