#include <iostream>
#include "wav.h"

using namespace std;

int wav_fopen_read(WAV_HANDLE* wav_handle, const char* input_filename) {
	errno_t error = 0;
	
	if (wav_handle == nullptr) {
		cout << "wav.cpp : wav_handle null" << endl;
		return -1;
	}
	
	error = fopen_s(&wav_handle->fp, input_filename, "rb");
	if (error != 0) {
		cout << "wav.cpp : input file " << input_filename << " open faild" << endl;
		return -1;
	}
	error = fread_s(&wav_handle->header, sizeof(WAV_HEADER), sizeof(WAV_HEADER), 1, wav_handle->fp);
	wav_handle->num_samples = wav_handle->header.data_size / wav_handle->header.num_channels / wav_handle->header.bits_per_samples * 8;
	wav_handle->mode = 1; // read mode
	return 0;
}

int wav_fopen_write(WAV_HANDLE* wav_handle, const char* filename, int fs, int BitsPersample, int ch) {
	errno_t error = 0;
	WAV_HEADER header;

	if (wav_handle == nullptr) {
		cout << "wav.cpp : wav_handle null" << endl;
		return -1;
	}

	error = fopen_s(&wav_handle->fp, filename, "wb");
	if (error != 0) {
		cout << "wav.cpp : output file " << filename << " open faild" << endl;
		return -1;
	}

	wav_handle->mode = 2; //write mode
	header.fs = fs;
	header.bits_per_samples = BitsPersample;
	header.num_channels = ch;
	header.block_size = ch * BitsPersample / 8;
	header.bytes_per_sec = fs * header.block_size;
	header.format = 0x01;
	fwrite(&header, sizeof(WAV_HEADER), 1, wav_handle->fp);

	return 0;
}

int wav_fclose(WAV_HANDLE* wav_handle) {
	errno_t error = 0;
	if (wav_handle == nullptr) {
		cout << "input pointer NULL" << endl;
		return -1;
	}

	if (wav_handle->mode == 2) {
		// write data size
		error = fseek(wav_handle->fp, -(wav_handle->write_size_byte + 4), SEEK_CUR);
		fwrite(&wav_handle->write_size_byte, sizeof(int), 1, wav_handle->fp);
		// write chank size
		error = fseek(wav_handle->fp, 4, SEEK_SET);
		int chank_size = wav_handle->write_size_byte + sizeof(WAV_HEADER) - 8;
		fwrite(&chank_size, sizeof(int), 1, wav_handle->fp);
	}
	error = fclose(wav_handle->fp);
	return 0;
}

int wav_fread(WAV_HANDLE* wav_handle, void* read_buffer, size_t read_size_byte) {
	errno_t error = 0;
	error = fread_s(read_buffer, read_size_byte, read_size_byte, 1, wav_handle->fp);
	return 0;
}

int wav_fwrite(WAV_HANDLE* wav_handle, void* write_buffer, size_t write_size_byte) {
	if (wav_handle == nullptr || write_buffer == nullptr) {
		cout << "input pointer NULL" << endl;
		return -1;
	}

	wav_handle->write_size_byte += fwrite(write_buffer, 1, write_size_byte, wav_handle->fp);
	return 0;
}
