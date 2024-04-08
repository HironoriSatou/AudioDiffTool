#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "WAV.h"
using namespace std;

int GenerateTestInputAudio(const char* filename, unsigned int SampleRate, unsigned int n_bit, unsigned int num_ch, string wav_format, string sound_type, unsigned int length_sample);
int GenerateSin(double* buffer, unsigned int length_sample, unsigned int SampleRate, float Freq);
int GenerateNoise(double* buffer, unsigned int length_sample);
int Block2Interleave(short* buffer, unsigned int length_sample, unsigned int num_ch);
int Block2Interleave(int* buffer, unsigned int length_sample, unsigned int num_ch);
int Block2Interleave(float* buffer, unsigned int length_sample, unsigned int num_ch);
int main() {
	int rtn = 0;
	cout << "TestInputGeneratorSampleMain" << endl;
	
	rtn = GenerateTestInputAudio("../test_input_16bit.wav", 48000, 16, 4, "int", "noise", 480000);
	rtn = GenerateTestInputAudio("../test_input_24bit.wav", 48000, 24, 4, "int", "noise", 480000);
	rtn = GenerateTestInputAudio("../test_input_32bit.wav", 48000, 32, 4, "int", "noise", 480000);
	rtn = GenerateTestInputAudio("../test_input_32bit_float.wav", 48000, 32, 4, "float", "noise", 480000);
	if (rtn != 0) {
		cout << "GenerateTestInputAudio() error, code = " << rtn << endl;
	}
	return 0;
}


int GenerateTestInputAudio(const char* filename, unsigned int SampleRate, unsigned int n_bit, unsigned int num_ch, string wav_format, string sound_type, unsigned int length_sample) {

	WAV_HANDLE write_handle;	
	unsigned int write_size_byte = length_sample * num_ch * n_bit / 8;

	if (!strcmp(sound_type.c_str(), "noise")) {
		std::unique_ptr<double[]> signal_buffer_double(new double[length_sample]);
		GenerateNoise(signal_buffer_double.get(), length_sample);
		if (n_bit == 16) {
			std::unique_ptr<short[]> write_buffer_short(new short[length_sample * num_ch]);
			for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
				for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
					double normalized_sample_data = (float)signal_buffer_double[i_sample] * (unsigned int)(0x01 << 15);
					if (normalized_sample_data >= (unsigned int)(0x01 << 15)) {
						write_buffer_short[i_sample + i_ch * length_sample] = (short)(0x7fff);
					}
					else {
						write_buffer_short[i_sample + i_ch * length_sample] = (short)normalized_sample_data;
					}

				}
			}
			Block2Interleave(write_buffer_short.get(), length_sample, num_ch);
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer_short.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 24) {
			std::unique_ptr<int[]> data_buffer_int(new int[length_sample * num_ch]);
			std::unique_ptr<unsigned char[]> write_buffer_1byte_array(new unsigned char[write_size_byte]);
			for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
				for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
					data_buffer_int[i_sample + i_ch * length_sample] = signal_buffer_double[i_sample] * ((unsigned int)(0x01 << 31) - 0.5f) - 0.5f;
				}
			}
			Block2Interleave(data_buffer_int.get(), length_sample, num_ch);
			unsigned int i_byte = 0;
			for (auto i_sample = 0; i_sample < length_sample * num_ch; i_sample++) {
				int data_int = data_buffer_int[i_sample];
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0x0000FF00) >> 8);
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0x00FF0000) >> 16);
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0xFF000000) >> 24);
			}
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer_1byte_array.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 32) {
			if (wav_format == "int") {
				std::unique_ptr<int[]> write_buffer_int(new int[length_sample * num_ch]);
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
						write_buffer_int[i_sample + i_ch * length_sample] = signal_buffer_double[i_sample] * ((unsigned int)(0x01 << 31) - 0.5f) - 0.5f;
					}
				}
				Block2Interleave(write_buffer_int.get(), length_sample, num_ch);
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
				wav_fwrite(&write_handle, write_buffer_int.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else if (wav_format == "float") {
				std::unique_ptr<float[]> write_buffer_float(new float[length_sample * num_ch]);
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
						write_buffer_float[i_sample + i_ch * length_sample] = (float)signal_buffer_double[i_sample];
					}
				}
				Block2Interleave(write_buffer_float.get(), length_sample, num_ch);
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "float");
				wav_fwrite(&write_handle, write_buffer_float.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else {
				cout << "format is not supported" << endl;
				return -1;
			}
		}
		else {
			cout << "format is not supported" << endl;
			return -1;
		}
	}
	else if (!strcmp(sound_type.c_str(), "sin")) {
		float Freq = 1000.0;
		std::unique_ptr<double[]> signal_buffer_double(new double[length_sample]);
		GenerateSin(signal_buffer_double.get(), length_sample, SampleRate, Freq);
		if (n_bit == 16) {
			std::unique_ptr<short[]> write_buffer_short(new short[length_sample * num_ch]);
			for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
				for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
					double normalized_sample_data = (float)signal_buffer_double[i_sample] * (unsigned int)(0x01 << 15);
					if (normalized_sample_data >= (unsigned int)(0x01 << 15)) {
						write_buffer_short[i_sample + i_ch * length_sample] = (short)(0x7fff);
					}
					else {
						write_buffer_short[i_sample + i_ch * length_sample] = (short)normalized_sample_data;
					}
						
				}
			}
			Block2Interleave(write_buffer_short.get(), length_sample, num_ch);
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer_short.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 24) {
			std::unique_ptr<int[]> data_buffer_int(new int[length_sample * num_ch]);
			std::unique_ptr<unsigned char[]> write_buffer_1byte_array(new unsigned char[write_size_byte]);
			for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
				for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
					data_buffer_int[i_sample + i_ch * length_sample] = signal_buffer_double[i_sample] * ((unsigned int)(0x01 << 31) - 0.5f) - 0.5f;
				}
			}
			Block2Interleave(data_buffer_int.get(), length_sample, num_ch);
			unsigned int i_byte = 0;
			for (auto i_sample = 0; i_sample < length_sample * num_ch; i_sample++) {
				int data_int = data_buffer_int[i_sample];
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0x0000FF00) >> 8);
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0x00FF0000) >> 16);
				write_buffer_1byte_array[i_byte++] = (unsigned char)((data_int & 0xFF000000) >> 24);				
			}			
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer_1byte_array.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 32) {
			if (wav_format == "int") {
				std::unique_ptr<int[]> write_buffer_int(new int[length_sample * num_ch]);
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
						write_buffer_int[i_sample + i_ch * length_sample] = signal_buffer_double[i_sample] * ((unsigned int)(0x01 << 31) - 0.5f) - 0.5f;
					}
				}
				Block2Interleave(write_buffer_int.get(), length_sample, num_ch);
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
				wav_fwrite(&write_handle, write_buffer_int.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else if (wav_format == "float") {
				std::unique_ptr<float[]> write_buffer_float(new float[length_sample * num_ch]);
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
						write_buffer_float[i_sample + i_ch * length_sample] = (float)signal_buffer_double[i_sample];
					}
				}
				Block2Interleave(write_buffer_float.get(), length_sample, num_ch);
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "float");
				wav_fwrite(&write_handle, write_buffer_float.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else {
				cout << "format is not supported" << endl;
				return -1;
			}
		}
		else {
			cout << "format is not supported" << endl;
			return -1;
		}
	}
	else if (!strcmp(sound_type.c_str(), "mute")) {
		if (n_bit == 16) {
			std::unique_ptr<short[]> write_buffer(new short[length_sample * num_ch]);
			for (unsigned int i_sample = 0; i_sample < length_sample * num_ch; i_sample+=num_ch) {
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					write_buffer[i_sample + i_ch] = 0;
				}				
			}
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 24) {
			std::unique_ptr<int[]> data_buffer(new int[length_sample * num_ch]);
			std::unique_ptr<unsigned char[]> write_buffer(new unsigned char[write_size_byte]);
			for (unsigned int i_sample = 0; i_sample < length_sample * num_ch; i_sample += num_ch) {
				for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
					data_buffer[i_sample + i_ch] = 0;
				}
			}
			unsigned int i_byte = 0;
			for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
				for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
					unsigned int temp_data = data_buffer[i_sample*num_ch + i_ch];
					write_buffer[i_byte++] = (unsigned char)((temp_data & 0xFF000000) >> 24);
					write_buffer[i_byte++] = (unsigned char)((temp_data & 0x00FF0000) >> 16);
					write_buffer[i_byte++] = (unsigned char)((temp_data & 0x0000FF00) >>  8);
				}
			}
			wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
			wav_fwrite(&write_handle, write_buffer.get(), write_size_byte);
			wav_fclose(&write_handle);
		}
		else if (n_bit == 32) {
			if (wav_format == "int") {
				std::unique_ptr<int[]> write_buffer(new int[length_sample * num_ch]);
				for (unsigned int i_sample = 0; i_sample < length_sample * num_ch; i_sample += num_ch) {
					for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
						write_buffer[i_sample + i_ch] = 0;
					}
				}
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "int");
				wav_fwrite(&write_handle, write_buffer.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else if (wav_format == "float") {
				std::unique_ptr<float[]> write_buffer(new float[length_sample * num_ch]);
				for (unsigned int i_sample = 0; i_sample < length_sample * num_ch; i_sample += num_ch) {
					for (unsigned int i_ch = 0; i_ch < num_ch; i_ch++) {
						write_buffer[i_sample + i_ch] = 0.0f;
					}
				}
				wav_fopen_write(&write_handle, filename, SampleRate, n_bit, num_ch, "float");
				wav_fwrite(&write_handle, write_buffer.get(), write_size_byte);
				wav_fclose(&write_handle);
			}
			else {
				cout << "format is not supported" << endl;
				return -1;
			}
		}
		else {
			cout << "format is not supported" << endl;
			return -1;
		}
	}
	else {
		cout << "sound type is not supported" << endl;
		return -1;
	}

	return 0;
}

int GenerateSin(double* buffer, unsigned int length_sample, unsigned int SampleRate, float Freq) {
	std::vector<int> TimeVector(length_sample);
	std::iota(TimeVector.begin(), TimeVector.end(), 0);
	for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
		buffer[i_sample] = sin(2 * M_PI * Freq * TimeVector[i_sample] / SampleRate);
	}
	return 0;
}

int GenerateNoise(double* buffer, unsigned int length_sample) {
	std::vector<double> NoiseVector(length_sample);
	std::random_device seed_gen;
	std::mt19937 engine(0);
	std::uniform_real_distribution<double> dist{ -1.0, 1.0 };
	auto rng = [&engine, &dist]() { return dist(engine); };
	std::generate(NoiseVector.begin(), NoiseVector.end(), rng);
	memcpy_s(buffer, length_sample * sizeof(double), NoiseVector.data(), length_sample * sizeof(double));
	return 0;
}

int Block2Interleave(short* buffer, unsigned int length_sample, unsigned int num_ch) {
	std::unique_ptr<short[]> cpy_buffer(new short[length_sample * num_ch]);
	memcpy_s(cpy_buffer.get(), length_sample * num_ch * sizeof(short), buffer, length_sample * num_ch * sizeof(short));
	auto i_sample_dst = 0;
	for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
		for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
			buffer[i_sample_dst++] = cpy_buffer[i_sample + length_sample * i_ch];
		}
	}
	return 0;
}

int Block2Interleave(int* buffer, unsigned int length_sample, unsigned int num_ch) {
	std::unique_ptr<int[]> cpy_buffer(new int[length_sample * num_ch]);
	memcpy_s(cpy_buffer.get(), length_sample * num_ch * sizeof(int), buffer, length_sample * num_ch * sizeof(int));
	auto i_sample_dst = 0;
	for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
		for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
			buffer[i_sample_dst++] = cpy_buffer[i_sample + length_sample * i_ch];
		}
	}
	return 0;
}

int Block2Interleave(float* buffer, unsigned int length_sample, unsigned int num_ch) {
	std::unique_ptr<float[]> cpy_buffer(new float[length_sample * num_ch]);
	memcpy_s(cpy_buffer.get(), length_sample * num_ch * sizeof(float), buffer, length_sample * num_ch * sizeof(float));
	auto i_sample_dst = 0;
	for (auto i_sample = 0; i_sample < length_sample; i_sample++) {
		for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
			buffer[i_sample_dst++] = cpy_buffer[i_sample + length_sample * i_ch];
		}
	}
	return 0;
}

