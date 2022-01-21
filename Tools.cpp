/*
* Tools.cpp
* Generic functions and tools
*
*/
#include "Tools.h"

int TOOLS::getMilliVoltsFromAnalog(int aChannel, int aAverages)
{
	int sumAD = 0;
	for (int i = 0; i < aAverages; i++) {
		sumAD += analogRead(aChannel);
	}
	int AD = sumAD / aAverages;

  int volt = (int)(((float)AD * 5000.0F) / 1024.0F);
  return volt;
}