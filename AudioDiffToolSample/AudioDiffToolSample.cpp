#include "AudioDiffTool.h"

int main(int argc, const char* argv[]) {
	AudioDiffTool result;

	result.CompareSoundDispResult(argv[1], argv[2]);
	return 0;
}

