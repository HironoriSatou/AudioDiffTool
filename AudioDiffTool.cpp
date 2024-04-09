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

using namespace std;
int CompareSoundDispResult(string input1, string input2);
int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer);
int TestReadWavFile(char* input_filename);
int TestWriteWavFile(char* input_filename);

int main(int argc, char* argv[]) {

	int rtn = 0;

	rtn = CompareSoundDispResult(argv[1], argv[2]);
	if (rtn != 0) {
		cout << "test_sound_diff failed" << endl;
		return rtn;
	}

#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

int CompareSoundDispResult(string input1, string input2) {
	int rtn = 0;
	int compare_length = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle_input1(new WAV_HANDLE);
	std::unique_ptr<WAV_HANDLE> wav_handle_input2(new WAV_HANDLE);

	rtn = wav_fopen_read(wav_handle_input1.get(), input1.data());
	if (rtn != 0) {
		cout << "File1 open failed" << endl;
		return rtn;
	}
	rtn = wav_fopen_read(wav_handle_input2.get(), input2.data());
	if (rtn != 0) {
		cout << "File2 open failed" << endl;
		return rtn;
	}

	cout << "\n" << endl;
	cout << "**********************************************" << endl;
	cout << "		File1 Info		" << endl;
	cout << "	File name:	" + input1 << endl;
	cout << "	Fs:		" << wav_handle_input1->header.fs << endl;
	cout << "	BitsPersample:	" << wav_handle_input1->header.bits_per_samples << endl;
	cout << "	Num channels:	" << wav_handle_input1->header.num_channels << endl;
	cout << "	Num Samples:	" << wav_handle_input1->num_samples << endl;
	cout << "\n" << endl;
	cout << "		File2 Info		" << endl;
	cout << "	File name:	" + input2 << endl;
	cout << "	Fs:		" << wav_handle_input2->header.fs << endl;
	cout << "	BitsPersample:	" << wav_handle_input2->header.bits_per_samples << endl;
	cout << "	Num channels:	" << wav_handle_input2->header.num_channels << endl;
	cout << "	Num Samples:	" << wav_handle_input2->num_samples << endl;
	cout << "**********************************************" << endl;
	cout << "\n" << endl;

	// file check
	if (wav_handle_input1->header.fs != wav_handle_input2->header.fs) {
		cout << "Error, fs not matched" << endl;
		return -1;
	}
	if (wav_handle_input1->header.num_channels != wav_handle_input2->header.num_channels) {
		cout << "Error, number of chanels not matched" << endl;
		return -1;
	}
	if (wav_handle_input1->num_samples != wav_handle_input2->num_samples) {
		cout << "Warning, number of samples not matched, compare by shorter length" << endl;
		if (wav_handle_input1->num_samples > wav_handle_input2->num_samples) {
			compare_length = wav_handle_input2->num_samples;
		}
		else {
			compare_length = wav_handle_input1->num_samples;
		}
	}
	else {
		compare_length = wav_handle_input1->num_samples;
	}

	// allocate buffer
	std::unique_ptr<std::unique_ptr<float []>[]> input_buffer1(new std::unique_ptr<float[]>[wav_handle_input1->header.num_channels]);
	std::unique_ptr<std::unique_ptr<float []>[]> input_buffer2(new std::unique_ptr<float[]>[wav_handle_input2->header.num_channels]);
	std::unique_ptr<std::unique_ptr<float []>[]> diff_buffer(new std::unique_ptr<float[]>[wav_handle_input1->header.num_channels]);
	std::unique_ptr<float []> max_diff_buffer(new float[wav_handle_input1->header.num_channels]);
	std::unique_ptr<unsigned int []> max_diff_index(new unsigned int[wav_handle_input1->header.num_channels]);
	for (auto n_ch = 0; n_ch < wav_handle_input1->header.num_channels; n_ch++) {
		input_buffer1[n_ch] = std::make_unique<float[]>(wav_handle_input1->num_samples);
		input_buffer2[n_ch] = std::make_unique<float[]>(wav_handle_input2->num_samples);
		diff_buffer[n_ch] = std::make_unique<float[]>(wav_handle_input1->num_samples);
		max_diff_buffer[n_ch] = 0.0f;
		max_diff_index[n_ch] = 0;
	}

	// compare audio data
	for (auto n_ch = 0; n_ch < wav_handle_input1->header.num_channels; n_ch++) {
		rtn = StoreSoundData(wav_handle_input1.get(), input_buffer1);
		if (rtn != 0) {
			cout << "store audio data failed" << endl;
			return rtn;
		}
		rtn = StoreSoundData(wav_handle_input2.get(), input_buffer2);
		if (rtn != 0) {
			cout << "store audio data failed" << endl;
			return rtn;
		}
		for (auto i_sample = 0; i_sample < compare_length; i_sample++) {
			float diff_value = abs(input_buffer1.get()[n_ch][i_sample] - input_buffer2.get()[n_ch][i_sample]);
			diff_buffer.get()[n_ch][i_sample] = diff_value;
			if (diff_value > max_diff_buffer[n_ch]) {
				max_diff_buffer[n_ch] = diff_value;
				max_diff_index[n_ch] = i_sample;
			}
		}
		cout << "	max different value " << n_ch << "ch: " <<  20 * log10(max_diff_buffer[n_ch]) << " [dB] at " << max_diff_index[n_ch] << " sample" << endl;
	}

	rtn = wav_fclose(wav_handle_input1.get());
	rtn = wav_fclose(wav_handle_input2.get());
	return 0;
}

int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer) {
	
	for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
		// file read and type conversion
		if (wav_handle->header.format == 0x01) { //PCM int data
			std::unique_ptr<unsigned char[]> read_buffer(new unsigned char[wav_handle->header.data_size]);
			wav_fread(wav_handle, read_buffer.get(), wav_handle->header.data_size);
			if (wav_handle->header.bits_per_samples == 16) {
				auto n_sample = 0;
				for (auto n_byte = 0; n_byte < wav_handle->header.data_size; n_byte += 2) {
					std::bitset<16> bs1 = (read_buffer[n_byte]) & 0x00ff;
					std::bitset<16> bs2 = (read_buffer[n_byte + 1]) & 0x00ff;
					std::bitset<16> bs_or = (bs1 | (bs2 << 8));
					short sample_data = (short)bs_or.to_ulong();
					input_buffer.get()[n_ch][n_sample++] = ((float)sample_data) / (int)(0x01 << 15);
				}
			}
			else if (wav_handle->header.bits_per_samples == 24) {
				auto n_sample = 0;
				for (auto n_byte = 0; n_byte < wav_handle->header.data_size; n_byte += 3) {
					std::bitset<32> bs1 = (read_buffer[n_byte]) & 0x000000ff;
					std::bitset<32> bs2 = (read_buffer[n_byte + 1]) & 0x000000ff;
					std::bitset<32> bs3 = (read_buffer[n_byte + 2]) & 0x000000ff;
					if (bs3[7] == 1) {
						bs3 |= 0xffffff00;
					}
					std::bitset<32> bs_or = (bs1 | (bs2 << 8) | (bs3 << 16));
					int sample_data = (int)bs_or.to_ullong();
					input_buffer.get()[n_ch][n_sample++] = ((float)sample_data) / (int)(0x01 << 23);
				}
			}
			else if (wav_handle->header.bits_per_samples == 32) {
				auto n_sample = 0;
				for (auto n_byte = 0; n_byte < wav_handle->header.data_size; n_byte += 4) {
					std::bitset<32> bs1 = (read_buffer[n_byte]) & 0x000000ff;
					std::bitset<32> bs2 = (read_buffer[n_byte + 1]) & 0x000000ff;
					std::bitset<32> bs3 = (read_buffer[n_byte + 2]) & 0x000000ff;
					std::bitset<32> bs4 = (read_buffer[n_byte + 3]) & 0x000000ff;
					if (bs4[7] == 1) {
						bs4 |= 0xffffff00;
					}
					std::bitset<32> bs_or = (bs1 | (bs2 << 8) | (bs3 << 16) | (bs4 << 24));
					int sample_data = (int)bs_or.to_ullong();
					input_buffer.get()[n_ch][n_sample++] = ((float)sample_data) / (unsigned int)(0x01 << 31);
				}
			}
			else {
				cout << "Error! input file format unsupported" << endl;
				return -1;
			}
		}
		else if (wav_handle->header.format == 0x03) { //PCM 32bit float data
			wav_fread(wav_handle, input_buffer.get()[n_ch].get(), wav_handle->header.data_size);
		}
		else {
			cout << "Error! input file format unsupported" << endl;
			return -1;
		}
	}	

	return 0;
}

int TestReadWavFile(char* input_filename) {
	int rtn = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle(new WAV_HANDLE);
	
	rtn = wav_fopen_read(wav_handle.get(), input_filename);
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	std::unique_ptr<short> read_buffer(new short[wav_handle->num_samples]);
	rtn = wav_fread(wav_handle.get(), read_buffer.get(), wav_handle->num_samples * sizeof(short));
	rtn = wav_fclose(wav_handle.get());
	return 0;
}

int TestWriteWavFile(char* input_filename) {
	int rtn = 0;
	int fs = 0;
	int BitsPersample = 0;
	int ch = 0;

	std::unique_ptr<WAV_HANDLE> wav_handle_read(new WAV_HANDLE);
	std::unique_ptr<WAV_HANDLE> wav_handle_write(new WAV_HANDLE);

	rtn = wav_fopen_read(wav_handle_read.get(), input_filename);
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}
	fs = wav_handle_read->header.fs;
	BitsPersample = wav_handle_read->header.bits_per_samples;
	ch = wav_handle_read->header.num_channels;

	rtn = wav_fopen_write(wav_handle_write.get(), ".\\TestData\\output2.wav", fs, BitsPersample, ch, "int");
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	std::unique_ptr<short> read_buffer(new short[wav_handle_read->num_samples]);
	rtn = wav_fread(wav_handle_read.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	memset(read_buffer.get(), 0, wav_handle_read->num_samples * sizeof(short));
	rtn = wav_fwrite(wav_handle_write.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	rtn = wav_fclose(wav_handle_read.get());
	rtn = wav_fclose(wav_handle_write.get());
	return 0;
}

