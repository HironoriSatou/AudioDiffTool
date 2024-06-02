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
#define ADT_INPUT_FILES 2
#define ADT_ERROR_FILE_OPEN	(-1)
#define ADT_ERROR_FS		(-2)
#define ADT_ERROR_NUM_CH	(-3)
#define ADT_ERROR_RUNTIME	(-4)

struct AudioDiffTool
{
public:
	AudioDiffTool();
	~AudioDiffTool();
	unsigned int num_ch[ADT_INPUT_FILES];
	unsigned int num_samples[ADT_INPUT_FILES];
	unsigned int compare_samples;
	unsigned int fs[ADT_INPUT_FILES];
	unsigned int bit[ADT_INPUT_FILES];
	float max_diff_dB_ch_array[ADT_MAX_CH_NUM];
	int max_diff_index_ch_array[ADT_MAX_CH_NUM];
	float max_diff_dB;
	int max_diff_ch;
	int max_diff_index;
	int CompareSoundDispResult(std::string input1, std::string input2);
	int ClearADTinstance();
	std::unique_ptr<std::unique_ptr<std::unique_ptr<float[]>[]>[]> audio_buffer;
	std::unique_ptr<std::unique_ptr<float[]>[]> diff_buffer;
};

static int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer);
static int TestReadWavFile(char* input_filename);
static int TestWriteWavFile(char* input_filename);
