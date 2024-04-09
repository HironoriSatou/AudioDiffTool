#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "wav.h"

int GenerateTestInputWavFile(const char* filename, unsigned int length_sample,
	unsigned int num_ch, unsigned int n_bit, unsigned int SampleRate, float max_dbFS, std::string wav_format, std::string sound_type);
int TypeConvertWriteWavFile(WAV_HANDLE* wav_handle, double* data, unsigned int length_sample,
	unsigned int num_ch, unsigned int n_bit, unsigned int SampleRate, const char* filename, std::string wav_format);
int NormalizeSignal(double* data_buffer, unsigned int length_sample, float max_dbFS);
int GenerateSin(double* buffer, unsigned int length_sample, unsigned int SampleRate, float Freq);
int GenerateNoise(double* buffer, unsigned int length_sample);
int Block2Interleave(short* buffer, unsigned int length_sample, unsigned int num_ch);
int Block2Interleave(int* buffer, unsigned int length_sample, unsigned int num_ch);
int Block2Interleave(float* buffer, unsigned int length_sample, unsigned int num_ch);

