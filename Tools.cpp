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

//*/
/*
    Serial.println();
    Serial.print("AD   : ");
    Serial.println(AD);
    Serial.print("Volt: ");
    Serial.println(volt, 4);
    Serial.print("iVolt   : ");
    Serial.println(ivolt);

  char message[128];
  char ctemp[16];
//  char ftemp[16];
//  dtostrf(volt, 3, 1, ftemp);
  itoa(volt, ctemp, 10);
  sprintf(message, "Volts: %s", ctemp);
  OLED_debug(message);
  return 0;
//*/  