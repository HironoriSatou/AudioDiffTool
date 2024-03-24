#include <iostream>
#include "wav.h"

using namespace std;

int wav_fopen(WAV_HANDLE* wav_handle, const char* input_filename, const char* mode) {
	errno_t error = 0;
	error = fopen_s(&wav_handle->fp, input_filename, mode);
	if (error != 0) {
		cout << "wav.cpp : input file " << input_filename << " open faild" << endl;
		return -1;
	}
	if (mode == "rb") {
		error = fread_s(&wav_handle->header, sizeof(WAV_HEADER), sizeof(WAV_HEADER), 1, wav_handle->fp);
		wav_handle->num_samples = wav_handle->header.data_size / wav_handle->header.num_channels / wav_handle->header.bits_per_samples * 8;
	}
	else if (mode == "wb") {
		
	}
		return 0;
}
int wav_fclose(WAV_HANDLE* wav_handle) {
	errno_t error = 0;
	error = fclose(wav_handle->fp);
	return 0;
}
int wav_fread(WAV_HANDLE* wav_handle, void* read_buffer, size_t read_size_byte) {
	errno_t error = 0;
	error = fread_s(read_buffer, read_size_byte, read_size_byte, 1, wav_handle->fp);
	return 0;
}
int wav_fwrite(WAV_HANDLE* wav_handle, void* write_buffer, size_t write_size_byte) {
	return 0;
}
