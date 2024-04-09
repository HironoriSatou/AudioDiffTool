#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include "wav.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG

#define ADT_MAX_CH_NUM 128

struct AudioDiffToolResult
{
public:
	AudioDiffToolResult();
	unsigned int num_ch;
	float max_diff_dB[ADT_MAX_CH_NUM];
	int max_diff_index[ADT_MAX_CH_NUM];
private:
	
};

int CompareSoundDispResult(AudioDiffToolResult* result, std::string input1, std::string input2);
int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer);
int TestReadWavFile(char* input_filename);
int TestWriteWavFile(char* input_filename);