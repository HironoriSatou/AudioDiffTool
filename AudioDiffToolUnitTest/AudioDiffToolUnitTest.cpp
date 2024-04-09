#include "pch.h"
#include "CppUnitTest.h"
#include "AudioDiffTool.h"
#include "TestInputGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioDiffToolUnitTest
{
	TEST_CLASS(AudioDiffToolUnitTest)
	{
	public:	
		TEST_METHOD(NormalTest1)
		{
			unsigned int SampleRate = 48000;
			unsigned int num_ch = 2;
			unsigned int length_sample = 480000;
			unsigned int n_bit = 16;
			float max_dbFS = 0.0f;
			std::string signal_type = "sin";
			GenerateTestInputWavFile("test1.wav", 480000, num_ch, n_bit, SampleRate, max_dbFS, "int", signal_type);
			GenerateTestInputWavFile("test2.wav", 480000, num_ch, n_bit, SampleRate, max_dbFS, "int", signal_type);
			AudioDiffToolResult result;
			int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
			Assert::AreEqual(0, rtn);
			Assert::AreEqual(num_ch, result.num_ch);
			for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
				Assert::IsTrue(isinf(result.max_diff_dB[i_ch]));
			}
			remove("test1.wav");
			remove("test2.wav");
		}
	};
}
