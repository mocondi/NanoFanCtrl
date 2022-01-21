#pragma once
#include <Arduino.h>

namespace TOOLS
{
	// Sample 10-bit AD multiple times, get avg and calc on avg
	int getMilliVoltsFromAnalog(int aChannel, int aAverages = 1);

}


/*
char message[124];
char ctemp[32];
itoa(ivolt, ctemp, 10);
sprintf(message, "Keypad Volts: %s", ctemp);
Serial.println(message);
*/