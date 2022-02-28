/*
* Tools.cpp
* Generic functions and tools
*
*/
#include "Tools.h"

//static struct pt_sem full, empty;
// Analog input array
ANALOG_CHANNEL ADInputs[MAX_ANALOGS];

void TOOLS::InitAnalogData()
{
  for (int i = 0; i < MAX_ANALOGS; i++) {
//    ADInputs[i].min = (int)BIT_TEN;
//    ADInputs[i].max = 0;
    ADInputs[i].avg = 0;
  }  
}

void TOOLS::SampleAnalogs()
{
  InitAnalogData();

  // Iterate through sample count
  for (int s = 0; s < MAX_ANALOG_SAMPLES; s++) {
    // Iterate though each channel
    for (int idx = 0; idx < MAX_ANALOGS; idx++) {
      int AD = analogRead(idx);
      // Capture avg
      ADInputs[idx].avg += AD;
/*
      // Capture min
      if (AD < ADInputs[idx].min) {
        ADInputs[idx].min = AD;
      }
      // Capture max
      if (AD > ADInputs[idx].max) {
        ADInputs[idx].max = AD;
      }
*/
    }
  }
  // Calculate averages
  for (int i = 0; i < MAX_ANALOGS; i++) {
    ADInputs[i].avg /= MAX_ANALOG_SAMPLES;
  }

/*
char message[124];
char ctemp[32];
for (int i = 0; i < MAX_ANALOGS; i++) {

  itoa(ADInputs[i].max, ctemp, 10);
  sprintf(message, "Channel %d, Max: %s ", i, ctemp);
  Serial.print(message);

  itoa(ADInputs[i].min, ctemp, 10);
  sprintf(message, ",Min: %s ", ctemp);
  Serial.print(message);

  itoa(ADInputs[i].avg, ctemp, 10);
  sprintf(message, ",Avg: %s", ctemp);
  Serial.println(message);
}
*/
}

int TOOLS::ReadAnalogChannel(int aChannel) {
  if (aChannel < 0 || aChannel >= MAX_ANALOGS) {
    return 0xffffffff;
  }

  // Use average
  int avgVO = ADInputs[aChannel].avg;
  if (avgVO <= 0) return 0;
  int volts = (int)(((float)avgVO * MILLI_VOLT_MAX) / BIT_TEN);
  return volts;
}
/*
int TOOLS::GetMilliVoltsFromAnalog(int aChannel, int aAverages)
{
	int sumAD = 0;
	for (int i = 0; i < aAverages; i++) {
		sumAD += analogRead(aChannel);
	}
	int AD = sumAD / aAverages;

  int volt = (int)(((float)AD * 5000.0F) / 1024.0F);
  return volt;
}
*/

bool TOOLS::GetFromFloat(float aFlt, char *aMsg)
{
  if (aMsg == NULL) return false;

  // Run test between sprintf and dtostrf
  // dtostrf(float, 3, 1, char);
  //sprintf(aMsg, "%f", aFlt);
  dtostrf(aFlt, 4, 1, aMsg);

  return true;
}

bool TOOLS::GetFromInteger(int aInt, char *aMsg)
{
  if (aMsg == NULL) return false;

  // test between sprintf and itoa
  // itoa(ivolt, ctemp, 10);
  //sprintf(aMsg, "%d", aInt);
  itoa(aInt, aMsg, 10);

  return true;
}



//*/
/*
char message[124];
char ctemp[32];
itoa(ivolt, ctemp, 10);
sprintf(message, "Keypad Volts: %s", ctemp);
Serial.println(message);
*/
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