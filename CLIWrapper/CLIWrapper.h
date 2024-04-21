#pragma once
#include "AudioDiffTool.h"
using namespace System;

namespace CLIWrapper {
	public ref class CLIWrapper
	{
		// TODO: このクラスのメソッドをここに追加します。
	public:
		int ADT(AudioDiffToolResult* result, String^ input1, String^ input2);
	};
}
