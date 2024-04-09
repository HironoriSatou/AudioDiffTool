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
			unsigned int length_sample = 48000;
			float max_dbFS = 0.0f;
			std::string signal_type = "sin";
			for (unsigned int num_ch = 1; num_ch <= 14; num_ch++) {
				for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
					GenerateTestInputWavFile("test1.wav", length_sample, num_ch, n_bit, SampleRate, max_dbFS, "int", signal_type);
					GenerateTestInputWavFile("test2.wav", length_sample, num_ch, n_bit, SampleRate, max_dbFS, "int", signal_type);
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
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch, 32, SampleRate, max_dbFS, "float", signal_type);
				GenerateTestInputWavFile("test2.wav", length_sample, num_ch, 32, SampleRate, max_dbFS, "float", signal_type);
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
		}
		TEST_METHOD(NormalTest2)
		{
			float max_dbFS1 =   0.0f;
			float max_dbFS2 = -20.0f;
			unsigned int SampleRate = 48000;
			unsigned int length_sample = 48000;
			std::string signal_type = "sin";
			for (unsigned int num_ch = 1; num_ch <= 14; num_ch++) {
				for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
					GenerateTestInputWavFile("test1.wav", length_sample, num_ch, n_bit, SampleRate, max_dbFS1, "int", signal_type);
					GenerateTestInputWavFile("test2.wav", length_sample, num_ch, n_bit, SampleRate, max_dbFS2, "int", signal_type);
					AudioDiffToolResult result;
					int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
					Assert::AreEqual(0, rtn);
					Assert::AreEqual(num_ch, result.num_ch);
					for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
						float exp_val = 20 * log10(abs(pow(10.0, max_dbFS1 / 20) - pow(10.0, max_dbFS2 / 20)));
						Assert::IsTrue(abs(exp_val - result.max_diff_dB[i_ch]) <= 1e-3);
					}
					remove("test1.wav");
					remove("test2.wav");
				}
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch, 32, SampleRate, max_dbFS1, "float", signal_type);
				GenerateTestInputWavFile("test2.wav", length_sample, num_ch, 32, SampleRate, max_dbFS2, "float", signal_type);
				AudioDiffToolResult result;
				int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
				Assert::AreEqual(0, rtn);
				Assert::AreEqual(num_ch, result.num_ch);
				for (auto i_ch = 0; i_ch < num_ch; i_ch++) {
					float exp_val = 20 * log10(abs(pow(10.0, max_dbFS1 / 20) - pow(10.0, max_dbFS2 / 20)));
					Assert::IsTrue(abs(exp_val - result.max_diff_dB[i_ch]) <= 1e-3);
				}
				remove("test1.wav");
				remove("test2.wav");
			}
		}
		TEST_METHOD(AbnormalTest1)
		{
			float max_dbFS1 = 0.0f;
			float max_dbFS2 = -20.0f;
			unsigned int SampleRate1 = 48000;
			unsigned int SampleRate2 = 44100;
			unsigned int length_sample = 48000;
			std::string signal_type = "sin";
			unsigned int num_ch = 1;
			for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch, n_bit, SampleRate1, max_dbFS1, "int", signal_type);
				GenerateTestInputWavFile("test2.wav", length_sample, num_ch, n_bit, SampleRate2, max_dbFS2, "int", signal_type);
				AudioDiffToolResult result;
				int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
				Assert::AreEqual(ADT_ERROR_FS, rtn);
				remove("test1.wav");
				remove("test2.wav");
			}
			GenerateTestInputWavFile("test1.wav", length_sample, num_ch, 32, SampleRate1, max_dbFS1, "float", signal_type);
			GenerateTestInputWavFile("test2.wav", length_sample, num_ch, 32, SampleRate2, max_dbFS2, "float", signal_type);
			AudioDiffToolResult result;
			int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
			Assert::AreEqual(ADT_ERROR_FS, rtn);
			remove("test1.wav");
			remove("test2.wav");
		}
		TEST_METHOD(AbnormalTest2)
		{
			float max_dbFS1 = 0.0f;
			float max_dbFS2 = -20.0f;
			unsigned int SampleRate = 48000;
			unsigned int length_sample = 48000;
			std::string signal_type = "sin";
			unsigned int num_ch = 1;
			for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch, n_bit, SampleRate, max_dbFS1, "int", signal_type);
				AudioDiffToolResult result;
				int rtn = CompareSoundDispResult(&result, "test1.wav", "test_not_exist.wav");
				Assert::AreEqual(ADT_ERROR_FILE_OPEN, rtn);
				remove("test1.wav");
			}
			GenerateTestInputWavFile("test1.wav", length_sample, num_ch, 32, SampleRate, max_dbFS1, "float", signal_type);
			AudioDiffToolResult result;
			int rtn = CompareSoundDispResult(&result, "test1.wav", "test_not_exist.wav");
			Assert::AreEqual(ADT_ERROR_FILE_OPEN, rtn);
			remove("test1.wav");
		}
		TEST_METHOD(AbnormalTest3)
		{
			float max_dbFS1 = 0.0f;
			float max_dbFS2 = -20.0f;
			unsigned int SampleRate = 48000;
			unsigned int length_sample = 48000;
			std::string signal_type = "sin";
			unsigned int num_ch1 = 1;
			unsigned int num_ch2 = 2;
			for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch1, n_bit, SampleRate, max_dbFS1, "int", signal_type);
				GenerateTestInputWavFile("test2.wav", length_sample, num_ch2, n_bit, SampleRate, max_dbFS2, "int", signal_type);
				AudioDiffToolResult result;
				int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
				Assert::AreEqual(ADT_ERROR_NUM_CH, rtn);
				remove("test1.wav");
				remove("test2.wav");
			}
			GenerateTestInputWavFile("test1.wav", length_sample, num_ch1, 32, SampleRate, max_dbFS1, "float", signal_type);
			GenerateTestInputWavFile("test2.wav", length_sample, num_ch2, 32, SampleRate, max_dbFS2, "float", signal_type);
			AudioDiffToolResult result;
			int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
			Assert::AreEqual(ADT_ERROR_NUM_CH, rtn);
			remove("test1.wav");
			remove("test2.wav");
		}
		TEST_METHOD(AbnormalTest4)
		{
			float max_dbFS1 = 0.0f;
			float max_dbFS2 = -20.0f;
			unsigned int SampleRate = 48000;
			unsigned int length_sample = 48000;
			std::string signal_type = "sin";
			unsigned int num_ch1 = 129;
			unsigned int num_ch2 = 129;
			for (unsigned int n_bit = 16; n_bit <= 32; n_bit += 8) {
				GenerateTestInputWavFile("test1.wav", length_sample, num_ch1, n_bit, SampleRate, max_dbFS1, "int", signal_type);
				GenerateTestInputWavFile("test2.wav", length_sample, num_ch2, n_bit, SampleRate, max_dbFS2, "int", signal_type);
				AudioDiffToolResult result;
				int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");				
				Assert::AreEqual(ADT_ERROR_NUM_CH, rtn);
				remove("test1.wav");
				remove("test2.wav");
			}
			GenerateTestInputWavFile("test1.wav", length_sample, num_ch1, 32, SampleRate, max_dbFS1, "float", signal_type);
			GenerateTestInputWavFile("test2.wav", length_sample, num_ch2, 32, SampleRate, max_dbFS2, "float", signal_type);
			AudioDiffToolResult result;
			int rtn = CompareSoundDispResult(&result, "test1.wav", "test2.wav");
			Assert::AreEqual(ADT_ERROR_NUM_CH, rtn);
			remove("test1.wav");
			remove("test2.wav");
		}
	};
}
