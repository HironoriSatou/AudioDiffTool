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

		property int num_ch
		{
			int get() { return this->p->num_ch; }
			void set(int value) { this->p->num_ch = value; }
		}
		property float^ max_diff_dB
		{
			float^ get() { return this->p->max_diff_dB[0]; }
		}
		property int^ max_diff_index
		{
			int^ get() { return this->p->max_diff_index[0]; }
		}
	};
}
