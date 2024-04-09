#pragma once
#include <iostream>
#include <string>
#include <stdio.h>

struct WAV_HEADER {
	char	RIFF[4] = {'R','I','F','F'};
	int		chank_size = 0;
	char	WAVE[4] = {'W','A','V','E'};
	char	fmt[4] = {'f','m','t',' '};
	int		fmt_size = 16;
	short	format = 0;
	short	num_channels = 0;
	int		fs = 0;
	int		bytes_per_sec = 0;
	short	block_size = 0;
	short	bits_per_samples = 0;
	char	data[4] = {'d', 'a', 't', 'a'};
	int		data_size_byte = 0;
};

struct WAV_HANDLE {
	FILE* fp;
	WAV_HEADER header;
	int num_samples = 0;
	int mode = 0; // 1:read , 2:write
	int write_size_byte = 0;
};

int wav_fopen_read(WAV_HANDLE* wav_handle, const char* input_filename);
int wav_fopen_write(WAV_HANDLE* wav_handle, const char* input_filename, int fs, int BitsPersample, int ch, std::string wav_format);
int wav_fclose(WAV_HANDLE* wav_handle);
int wav_fread(WAV_HANDLE* wav_handle, void* read_buffer, size_t read_size_byte);
int wav_fwrite(WAV_HANDLE* wav_handle, void* write_buffer, size_t write_size_byte);
