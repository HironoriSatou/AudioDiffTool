#pragma once
#include <stdio.h>

struct WAV_HEADER {
	char	RIFF[4];
	int		chank_size;
	char	WAVE[4];
	char	fmt[4];
	int		fmt_size;
	short	format;
	short	num_channels;
	int		fs;
	int		bytes_per_sec;
	short	block_size;
	short	bits_per_samples;
	char	data[4];
	int		data_size;
};

struct WAV_HANDLE {
	FILE* fp;
	WAV_HEADER header;
	int num_samples;
};

int wav_fopen(WAV_HANDLE* wav_handle, const char* input_filename, const char* mode);
int wav_fclose(WAV_HANDLE* wav_handle);
int wav_fread(WAV_HANDLE* wav_handle, void* read_buffer, size_t read_size_byte);
int wav_fwrite(WAV_HANDLE* wav_handle, void* write_buffer, size_t write_size_byte);
