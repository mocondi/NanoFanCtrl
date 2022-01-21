#pragma once
#include <Arduino.h>

namespace TOOLS
{
	// Sample 10-bit AD multiple times, get avg and calc on avg
	int getMilliVoltsFromAnalog(int aChannel, int aAverages = 1);

}
