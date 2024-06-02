#include <string>
#include "AudioDiffTool.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;
int test(string input_filename1, string input_filename2);

int main(int argc, const char* argv[]) {
	test(argv[1], argv[2]);

	_CrtDumpMemoryLeaks();
	return 0;
}

int test(string input_filename1, string input_filename2)
{
	AudioDiffTool result;
	result.CompareSoundDispResult(input_filename1, input_filename2);

	return 0;
}

