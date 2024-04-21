#include <iostream>
#include <string>
#include "wav.h"

using namespace std;

int wav_fopen_read(WAV_HANDLE* wav_handle, const char* input_filename) {
	
	//TODO: support other wav format such as MATLAB, etc.
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
	error = fread_s(&wav_handle->header, sizeof(WAV_HEADER), sizeof(WAV_HEADER) - 8, 1, wav_handle->fp);
	search_data_chunk(wav_handle->fp);
	fread_s(&wav_handle->header.data_size_byte, sizeof(int), sizeof(int), 1, wav_handle->fp);
	wav_handle->num_samples = wav_handle->header.data_size_byte / wav_handle->header.num_channels / wav_handle->header.bits_per_samples * 8;
	wav_handle->mode = 1; // read mode
	return 0;
}

int wav_fopen_write(WAV_HANDLE* wav_handle, const char* filename, int fs, int BitsPersample, int ch, std::string wav_format) {
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
	if (wav_format == "int") {
		header.format = 0x01;
	}
	else if (wav_format == "float") {
		header.format = 0x03;
	}
	else {
		cout << "wav_format is not supported" << endl;
		return -1;
	}
	
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

int wav_data_load(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<short[]>[]>& data_buffer, unsigned int load_samples) {
	int load_success_samples = 0;
	std::unique_ptr<unsigned char[]> read_buffer(new unsigned char[load_samples * wav_handle->header.num_channels * sizeof(short)]);
	int load_success_bytes = fread_s(read_buffer.get(), load_samples * wav_handle->header.num_channels * sizeof(unsigned char) * sizeof(short),
														sizeof(unsigned char),
														load_samples * wav_handle->header.num_channels * sizeof(short),
														wav_handle->fp);
	if (wav_handle->header.bits_per_samples == 16) {
		for (auto n_byte = 0; n_byte < load_success_bytes; n_byte += sizeof(short) * wav_handle->header.num_channels) {
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				std::bitset<16> bs1 = (read_buffer[n_byte + n_ch * sizeof(short) + 0]) & 0x00ff;
				std::bitset<16> bs2 = (read_buffer[n_byte + n_ch * sizeof(short) + 1]) & 0x00ff;
				std::bitset<16> bs_or = (bs1 | (bs2 << 8));
				data_buffer[n_ch][load_success_samples] = (short)bs_or.to_ulong();
			}
			load_success_samples++;
		}
	}
	else {
		cout << "format wrong" << endl;
		return 0;
	}
	return load_success_samples;
}

int wav_data_load(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<int[]>[]>& data_buffer, unsigned int load_samples) {
	int load_success_samples = 0;	
	if (wav_handle->header.bits_per_samples == 24) {
		std::unique_ptr<unsigned char[]> read_buffer(new unsigned char[load_samples * wav_handle->header.num_channels * SIZE_OF_INT_24_BIT_BYTE]);
		int load_success_bytes = fread_s(read_buffer.get(), 
										load_samples * wav_handle->header.num_channels * sizeof(unsigned char) * SIZE_OF_INT_24_BIT_BYTE,
										sizeof(unsigned char),
										load_samples * wav_handle->header.num_channels * SIZE_OF_INT_24_BIT_BYTE,
										wav_handle->fp);
		for (auto n_byte = 0; n_byte < load_success_bytes; n_byte += SIZE_OF_INT_24_BIT_BYTE * wav_handle->header.num_channels) {
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				std::bitset<32> bs1 = (read_buffer[n_byte + n_ch * SIZE_OF_INT_24_BIT_BYTE + 0]) & 0x000000ff;
				std::bitset<32> bs2 = (read_buffer[n_byte + n_ch * SIZE_OF_INT_24_BIT_BYTE + 1]) & 0x000000ff;
				std::bitset<32> bs3 = (read_buffer[n_byte + n_ch * SIZE_OF_INT_24_BIT_BYTE + 2]) & 0x000000ff;
				if (bs3[7] == 1) {
					bs3 |= 0xffffff00;
				}
				std::bitset<32> bs_or = (bs1 | (bs2 << 8) | (bs3 << 16));
				data_buffer[n_ch][load_success_samples] = (int)bs_or.to_ullong();
			}
			load_success_samples++;
		}
	}
	else if (wav_handle->header.bits_per_samples == 32) {
		std::unique_ptr<unsigned char[]> read_buffer(new unsigned char[load_samples * wav_handle->header.num_channels * sizeof(int)]);
		int load_success_bytes = fread_s(read_buffer.get(), 
										load_samples * wav_handle->header.num_channels * sizeof(unsigned char) * sizeof(int),
										sizeof(unsigned char),
										load_samples * wav_handle->header.num_channels * sizeof(int),
										wav_handle->fp);
		for (auto n_byte = 0; n_byte < load_success_bytes; n_byte += sizeof(int) * wav_handle->header.num_channels) {
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				std::bitset<32> bs1 = (read_buffer[n_byte + n_ch * sizeof(int) + 0]) & 0x000000ff;
				std::bitset<32> bs2 = (read_buffer[n_byte + n_ch * sizeof(int) + 1]) & 0x000000ff;
				std::bitset<32> bs3 = (read_buffer[n_byte + n_ch * sizeof(int) + 2]) & 0x000000ff;
				std::bitset<32> bs4 = (read_buffer[n_byte + n_ch * sizeof(int) + 3]) & 0x000000ff;
				if (bs4[7] == 1) {
					bs4 |= 0xffffff00;
				}
				std::bitset<32> bs_or = (bs1 | (bs2 << 8) | (bs3 << 16) | (bs4 << 24));
				data_buffer[n_ch][load_success_samples] = (int)bs_or.to_ullong();
			}
			load_success_samples++;
		}
	}
	else {
		cout << "format wrong" << endl;
		return 0;
	}
	return load_success_samples;
}

int wav_fread(WAV_HANDLE* wav_handle, void* read_buffer, size_t read_size_byte) {
	return fread_s(read_buffer, read_size_byte, 1, read_size_byte, wav_handle->fp);
}

int wav_fwrite(WAV_HANDLE* wav_handle, void* write_buffer, size_t write_size_byte) {
	if (wav_handle == nullptr || write_buffer == nullptr) {
		cout << "input pointer NULL" << endl;
		return -1;
	}

	wav_handle->write_size_byte += fwrite(write_buffer, 1, write_size_byte, wav_handle->fp);
	return 0;
}

int search_data_chunk(FILE* fp) {
	std::unique_ptr<unsigned char[]> data_buffer(new unsigned char[MAX_HEADER_SIZE_BYTE]);

	for (auto i_byte = 0; i_byte < MAX_HEADER_SIZE_BYTE; i_byte++) {
		fread_s(&data_buffer[i_byte], MAX_HEADER_SIZE_BYTE * sizeof(unsigned char), sizeof(unsigned char), 1, fp);
		if (data_buffer[i_byte] == 0x64) {
			fread_s(&data_buffer[i_byte + 1], MAX_HEADER_SIZE_BYTE * sizeof(unsigned char), sizeof(unsigned char), 3, fp);
			if (data_buffer[i_byte + 1] == 0x61 && data_buffer[i_byte + 2] == 0x74 && data_buffer[i_byte + 3] == 0x61) {
				break;
			}
			else {
				i_byte += 3;
			}
		}
	}
	return 0;
}
