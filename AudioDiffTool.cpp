#include <iostream>
#include <memory>
#include "wav.h"


#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG

using namespace std;
int test_read_function(char* input_filename);
int test_write_function(char* input_filename);

int main(int argc, char* argv[]) {

	cout << "This is a test" << endl;
	int rtn = 0;
	rtn = test_read_function(argv[1]);
	if (rtn != 0) {
		cout << "test_read_function failed" << endl;
		return rtn;
	}

	rtn = test_write_function(argv[1]);
	if (rtn != 0) {
		cout << "test__write_function failed" << endl;
		return rtn;
	}


#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

int test_read_function(char* input_filename) {
	int rtn = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle(new WAV_HANDLE);
	
	// file open
	rtn = wav_fopen(wav_handle.get(), input_filename, "rb");
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	std::unique_ptr<short> read_buffer(new short[wav_handle->num_samples]);
	// data read
	rtn = wav_fread(wav_handle.get(), read_buffer.get(), wav_handle->num_samples * sizeof(short));

	// file close
	rtn = wav_fclose(wav_handle.get());
	return 0;
}

int test_write_function(char* input_filename) {
	int rtn = 0;
	std::unique_ptr<WAV_HANDLE> wav_handle_read(new WAV_HANDLE);
	std::unique_ptr<WAV_HANDLE> wav_handle_write(new WAV_HANDLE);

	// file open
	rtn = wav_fopen(wav_handle_read.get(), input_filename, "rb");
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	rtn = wav_fopen(wav_handle_write.get(), ".\\TestData\\output.wav", "wb");
	if (rtn != 0) {
		cout << "File open failed" << endl;
		return rtn;
	}

	std::unique_ptr<short> read_buffer(new short[wav_handle_read->num_samples]);
	// data read
	rtn = wav_fread(wav_handle_read.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	// data edit
	memset(read_buffer.get(), 0, wav_handle_read->num_samples * sizeof(short));
	// data write
	rtn = wav_fwrite(wav_handle_write.get(), read_buffer.get(), wav_handle_read->num_samples * sizeof(short));

	// file close
	rtn = wav_fclose(wav_handle_read.get());
	rtn = wav_fclose(wav_handle_write.get());
	return 0;
}

