/*
* Temperature.cpp
* Handles thermister reading and calculations
*
*/
#include <Arduino.h>
#include "Temperature.h"
#include "Tools.h"

#define TEMP_SAMPLE_COUNT     10
static const float MX =     -28.12F;
static const float B =      4941.4F;

void M_TEMPERATURE::initTemperature()
{
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT_PULLUP);
}

float M_TEMPERATURE::sampleTempPower()
{
  return TOOLS::ReadAnalogChannel(TEMP_CHANNEL);
}

float M_TEMPERATURE::sampleTemperature()
{
//Serial.println(F("sampleTemperature()"));

  // Translate to voltage
  float volt = sampleTempPower();

//Serial.println(volt);
  // Translate voltage to temperature F
  // Y = Mx+b, Y = -22.5 + 4575
  // X = (Y-B)/M
  float temp = (volt-B)/MX;
//Serial.println(temp);

  return temp;
}