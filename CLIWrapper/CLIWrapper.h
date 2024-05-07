#pragma once
#include "AudioDiffTool.h"
using namespace System;

namespace CLIWrapper {
	public ref class CLIWrapperClass
	{
	private:
		AudioDiffTool* p;
	public:
		CLIWrapperClass() { 
			p = new AudioDiffTool();
		}
		~CLIWrapperClass() { 
			this->!CLIWrapperClass(); 
		}

		!CLIWrapperClass() 
		{
			delete p;
			p = nullptr;
		}

		int CompareSound(String^ input1, String^ input2);

		unsigned int^ GetNumCh(unsigned int file_index) {
			return p->num_ch[file_index];
		}

		unsigned int^ GetNumSamples(unsigned int file_index) {
			return p->num_samples[file_index];
		}

		unsigned int^ GetFs(unsigned int file_index) {
			return p->fs[file_index];
		}

		unsigned int^ GetBit(unsigned int file_index) {
			return p->bit[file_index];
		}

		property float^ max_diff_dB
		{
			float^ get() { return this->p->max_diff_dB; }
		}
		property int^ max_diff_index
		{
			int^ get() { return this->p->max_diff_index; }
		}
		property int^ max_diff_ch
		{
			int^ get() { return this->p->max_diff_ch + 1; }
		}

	};
}
