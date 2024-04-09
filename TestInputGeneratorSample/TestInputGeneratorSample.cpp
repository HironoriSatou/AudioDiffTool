#include "TestInputGenerator.h"

using namespace std;
int main() {
	int rtn = 0;
	cout << "TestInputGeneratorSampleMain" << endl;

	GenerateTestInputWavFile("../test_input_16bit_noise.wav", 480000, 4, 16, 48000, -6.0f, "int", "noise");
	GenerateTestInputWavFile("../test_input_24bit_noise.wav", 480000, 4, 24, 48000, -6.0f, "int", "noise");
	GenerateTestInputWavFile("../test_input_32bit_noise.wav", 480000, 4, 32, 48000, -6.0f, "int", "noise");
	GenerateTestInputWavFile("../test_input_32bit_float_noise.wav", 480000, 4, 32, 48000, -6.0f, "float", "noise");

	GenerateTestInputWavFile("../test_input_16bit_sin.wav", 480000, 4, 16, 48000, -20.0f, "int", "sin");
	GenerateTestInputWavFile("../test_input_24bit_sin.wav", 480000, 4, 24, 48000, -20.0f, "int", "sin");
	GenerateTestInputWavFile("../test_input_32bit_sin.wav", 480000, 4, 32, 48000, -20.0f, "int", "sin");
	GenerateTestInputWavFile("../test_input_32bit_float_sin.wav", 480000, 4, 32, 48000, -20.0f, "float", "sin");

	GenerateTestInputWavFile("../test_input_16bit_mute.wav", 480000, 4, 16, 48000, 0.0f, "int", "mute");
	GenerateTestInputWavFile("../test_input_24bit_mute.wav", 480000, 4, 24, 48000, 0.0f, "int", "mute");
	GenerateTestInputWavFile("../test_input_32bit_mute.wav", 480000, 4, 32, 48000, 0.0f, "int", "mute");
	GenerateTestInputWavFile("../test_input_32bit_float_mute.wav", 480000, 4, 32, 48000, 0.0f, "float", "mute");

	return 0;
}
