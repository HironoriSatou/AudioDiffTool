#include <msclr/marshal_cppstd.h>
#include <string>
#include "CLIWrapper.h"

using namespace msclr::interop;
using CLIWrapper::CLIWrapperClass;

int CLIWrapperClass::CompareSound(String^ input1, String^ input2)
{
	int rtn = 0;
	std::string input1_string;
	std::string input2_string;
	input1_string = marshal_as<std::string>(input1);
	input2_string = marshal_as<std::string>(input2);

	//clear
	p->ClearADTinstance();
	delete audio_buffer;

	//compare processing
	rtn = p->CompareSoundDispResult(input1_string, input2_string);
	audio_buffer = gcnew array<array<array<float>^>^>(ADT_INPUT_FILES);
	for (auto n_files = 0; n_files < ADT_INPUT_FILES; n_files++) {
		audio_buffer[n_files] = gcnew array<array<float>^>(GetNumCh(0));
		for (auto n_ch = 0; n_ch < GetNumCh(0); n_ch++) {
			audio_buffer[n_files][n_ch] = gcnew array<float>(p->compare_samples);
		}
	}

	for (auto n_files = 0; n_files < 2; n_files++) {
		for (auto n_ch = 0; n_ch < GetNumCh(0); n_ch++) {
			for (auto n_sample = 0; n_sample < p->compare_samples; n_sample++) {
				audio_buffer[n_files][n_ch][n_sample] = p->audio_buffer[n_files][n_ch][n_sample];
			}
		}
	}

	return rtn;
}
