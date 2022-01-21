/*
* Tools.cpp
* Generic functions and tools
*
*/
#include "Tools.h"

int TOOLS::getMilliVoltsFromAnalog(int channel)
{
  int AD = analogRead(channel);
  int volt = (int)(((float)AD * 5000.0F) / 1024.0F);
  return volt;
}