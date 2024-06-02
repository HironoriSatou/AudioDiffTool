#pragma once
#include <List>
#include "AudioDiffTool.h"
using namespace System;

namespace CLIWrapper {
	public ref class CLIWrapperClass
	{
	private:
		AudioDiffTool* p;
		array<array<array<float>^>^>^ audio_buffer = nullptr; //n_sample * n_ch * 2files
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

		unsigned int GetNumCh(unsigned int file_index) {
			return p->num_ch[file_index];
		}

		unsigned int GetNumSamples(unsigned int file_index) {
			return p->num_samples[file_index];
		}

		unsigned int GetFs(unsigned int file_index) {
			return p->fs[file_index];
		}

		unsigned int GetBit(unsigned int file_index) {
			return p->bit[file_index];
		}

		array<array<array<float>^>^>^ GetAudioBuffer() {
			return audio_buffer;
		}

		property float max_diff_dB
		{
			float get() { return this->p->max_diff_dB; }
		}
		property int max_diff_index
		{
			int get() { return this->p->max_diff_index; }
		}
		property int max_diff_ch
		{
			int get() { return this->p->max_diff_ch + 1; }
		}
	};
}
