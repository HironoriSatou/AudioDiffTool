#include <msclr/marshal_cppstd.h>
#include <string>
#include "CLIWrapper.h"

using namespace msclr::interop;
using CLIWrapper::CLIWrapperClass;

int CLIWrapperClass::CompareSound(String^ input1, String^ input2)
{
	std::string input1_string;
	std::string input2_string;
	input1_string = marshal_as<std::string>(input1);
	input2_string = marshal_as<std::string>(input2);
	p->ClearADTinstance();
	return p->CompareSoundDispResult(input1_string, input2_string);
}
