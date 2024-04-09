#include "AudioDiffTool.h"

int main(int argc, const char* argv[]) {
	AudioDiffToolResult result;

	CompareSoundDispResult(&result, argv[1], argv[2]);
	return 0;
}

