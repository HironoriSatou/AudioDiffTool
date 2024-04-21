#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include "wav.h"

#define DllExport __declspec( dllexport )
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG

#define ADT_MAX_CH_NUM 128
#define ADT_ERROR_FILE_OPEN	(-1)
#define ADT_ERROR_FS		(-2)
#define ADT_ERROR_NUM_CH	(-3)
#define ADT_ERROR_RUNTIME	(-4)

extern "C"
{
	struct DllExport AudioDiffToolResult
	{
	public:
		AudioDiffToolResult();
		unsigned int num_ch;
		float max_diff_dB[ADT_MAX_CH_NUM];
		int max_diff_index[ADT_MAX_CH_NUM];
	private:
	
	};
	DllExport int CompareSoundDispResult(AudioDiffToolResult* result, std::string input1, std::string input2);
	DllExport int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer);
}

static int TestReadWavFile(char* input_filename);
static int TestWriteWavFile(char* input_filename);
