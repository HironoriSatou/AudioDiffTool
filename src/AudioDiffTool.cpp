#include "AudioDiffTool.h"

AudioDiffTool::AudioDiffTool() {
	num_ch = 0;
	memset(max_diff_dB, 0, ADT_MAX_CH_NUM * sizeof(float));
	memset(max_diff_index, 0, ADT_MAX_CH_NUM * sizeof(int));
}

AudioDiffTool::~AudioDiffTool() {

}

using namespace std;
int AudioDiffTool::CompareSoundDispResult(string input1, string input2) {
	int rtn = 0;
	int compare_length = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle_input1(new WAV_HANDLE);
	std::unique_ptr<WAV_HANDLE> wav_handle_input2(new WAV_HANDLE);

	rtn = wav_fopen_read(wav_handle_input1.get(), input1.data());
	if (rtn != 0) {
		cout << "File1 open failed" << endl;
		return ADT_ERROR_FILE_OPEN;
	}
	rtn = wav_fopen_read(wav_handle_input2.get(), input2.data());
	if (rtn != 0) {
		cout << "File2 open failed" << endl;
		wav_fclose(wav_handle_input1.get());
		return ADT_ERROR_FILE_OPEN;
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
		wav_fclose(wav_handle_input1.get());
		wav_fclose(wav_handle_input2.get());
		return ADT_ERROR_FS;
	}
	if (wav_handle_input1->header.num_channels != wav_handle_input2->header.num_channels) {
		cout << "Error, number of chanels not matched" << endl;
		wav_fclose(wav_handle_input1.get());
		wav_fclose(wav_handle_input2.get());
		return ADT_ERROR_NUM_CH;
	}
	if (wav_handle_input1->header.num_channels > ADT_MAX_CH_NUM) {
		cout << "Error, number of channels greater than the specified number has been entered" << endl;
		wav_fclose(wav_handle_input1.get());
		wav_fclose(wav_handle_input2.get());
		return ADT_ERROR_NUM_CH;
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
	std::unique_ptr<std::unique_ptr<float []>[]> input_buffer1(new std::unique_ptr<float []>[wav_handle_input1->header.num_channels]);
	std::unique_ptr<std::unique_ptr<float []>[]> input_buffer2(new std::unique_ptr<float []>[wav_handle_input2->header.num_channels]);
	std::unique_ptr<std::unique_ptr<float []>[]> diff_buffer(new std::unique_ptr<float []>[wav_handle_input1->header.num_channels]);
	std::unique_ptr<float []> max_diff_buffer(new float[wav_handle_input1->header.num_channels]);
	std::unique_ptr<unsigned int []> max_diff_index(new unsigned int[wav_handle_input1->header.num_channels]);
	for (auto n_ch = 0; n_ch < wav_handle_input1->header.num_channels; n_ch++) {
		input_buffer1[n_ch] = std::make_unique<float []>(wav_handle_input1->num_samples);
		input_buffer2[n_ch] = std::make_unique<float []>(wav_handle_input2->num_samples);
		diff_buffer[n_ch] = std::make_unique<float []>(wav_handle_input1->num_samples);
		max_diff_buffer[n_ch] = 0.0f;
		max_diff_index[n_ch] = 0;
	}

	// compare audio data
	rtn = StoreSoundData(wav_handle_input1.get(), input_buffer1);
	if (rtn != 0) {
		cout << "store audio data failed" << endl;
		wav_fclose(wav_handle_input1.get());
		wav_fclose(wav_handle_input2.get());
		return ADT_ERROR_RUNTIME;
	}
	rtn = StoreSoundData(wav_handle_input2.get(), input_buffer2);
	if (rtn != 0) {
		cout << "store audio data failed" << endl;
		wav_fclose(wav_handle_input1.get());
		wav_fclose(wav_handle_input2.get());
		return ADT_ERROR_RUNTIME;
	}
	for (auto n_ch = 0; n_ch < wav_handle_input1->header.num_channels; n_ch++) {
		for (auto i_sample = 0; i_sample < compare_length; i_sample++) {
			float diff_value = abs(input_buffer1[n_ch][i_sample] - input_buffer2[n_ch][i_sample]);
			diff_buffer[n_ch][i_sample] = diff_value;
			if (diff_value > max_diff_buffer[n_ch]) {
				max_diff_buffer[n_ch] = diff_value;
				max_diff_index[n_ch] = i_sample;
			}
		}
		max_diff_dB[n_ch] = 20 * log10(max_diff_buffer[n_ch]);
		max_diff_index[n_ch] = max_diff_index[n_ch];
		cout << "	max different value " << n_ch << "ch: " <<  20 * log10(max_diff_buffer[n_ch]) << " [dB] at " << max_diff_index[n_ch] << " sample" << endl;
	}

	wav_fclose(wav_handle_input1.get());
	wav_fclose(wav_handle_input2.get());
	num_ch = wav_handle_input1->header.num_channels;
	return 0;
}

static int StoreSoundData(WAV_HANDLE* wav_handle, std::unique_ptr<std::unique_ptr<float[]>[]>& input_buffer) {
		
	// file read and type conversion
	if (wav_handle->header.format == 0x01) { //PCM int data
		if (wav_handle->header.bits_per_samples == 16) {
			std::unique_ptr<std::unique_ptr<short[]>[]> read_buffer(new std::unique_ptr<short[]>[wav_handle->header.num_channels]);
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				read_buffer[n_ch] = std::make_unique<short[]>(wav_handle->num_samples);
			}
			wav_data_load(wav_handle, read_buffer, wav_handle->num_samples);
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				for (auto n_sample = 0; n_sample < wav_handle->num_samples; n_sample++) {
					input_buffer[n_ch][n_sample] = ((float)read_buffer[n_ch][n_sample]) / (int)(0x01 << 15);
				}
			}
		}
		else {
			unsigned int normalize_factor = 0;
			if (wav_handle->header.bits_per_samples == 24) {
				normalize_factor = (0x01 << 23);
			}else if (wav_handle->header.bits_per_samples == 32) {
				normalize_factor = (0x01 << 31);				
			}
			else {
				cout << "Error! input file format unsupported" << endl;
				return -1;
			}
			std::unique_ptr<std::unique_ptr<int[]>[]> read_buffer(new std::unique_ptr<int[]>[wav_handle->header.num_channels]);
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				read_buffer[n_ch] = std::make_unique<int[]>(wav_handle->num_samples);
			}
			wav_data_load(wav_handle, read_buffer, wav_handle->num_samples);
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				for (auto n_sample = 0; n_sample < wav_handle->num_samples; n_sample++) {
					input_buffer[n_ch][n_sample] = ((float)read_buffer[n_ch][n_sample]) / normalize_factor;				
				}
			}	
		}
	}
	else if (wav_handle->header.format == 0x03) { //PCM 32bit float data
		std::unique_ptr<float[]> read_buffer(new float[wav_handle->num_samples * wav_handle->header.num_channels]);
		wav_fread(wav_handle, read_buffer.get(), wav_handle->header.data_size_byte);
		// Interleave to block conversion
		auto n_sample = 0;
		for (auto dst_sample = 0; dst_sample < wav_handle->num_samples; dst_sample++) {
			for (auto n_ch = 0; n_ch < wav_handle->header.num_channels; n_ch++) {
				input_buffer[n_ch][dst_sample] = read_buffer[n_sample++];
			}
		}
	}
	else {
		cout << "Error! input file format unsupported" << endl;
		return -1;
	}
	return 0;
}

static int TestReadWavFile(char* input_filename) {
	int rtn = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle(new WAV_HANDLE);
	
	rtn = wav_fopen_read(wav_handle.get(), input_filename);
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	std::unique_ptr<short> read_buffer(new short[wav_handle->num_samples]);
	wav_fread(wav_handle.get(), read_buffer.get(), wav_handle->num_samples * sizeof(short));
	wav_fclose(wav_handle.get());
	return 0;
}

static int TestWriteWavFile(char* input_filename) {
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
	wav_fread(wav_handle_read.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	memset(read_buffer.get(), 0, wav_handle_read->num_samples * sizeof(short));
	wav_fwrite(wav_handle_write.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	wav_fclose(wav_handle_read.get());
	wav_fclose(wav_handle_write.get());
	return 0;
}

