#pragma once
#include <Arduino.h>

// Analog Inputs
#define KEYBOARD_PIN        A0
#define TEMPERATURE_PIN     A1
#define FAN_TACH_PIN        A2

// Digital Outputs
#define FAN_PWM_PIN         3
#define DISPLAY_POWER_PIN   4

// Other defs
#define MAX_ANALOGS         3
#define MAX_ANALOG_SAMPLES  10

typedef struct
{
  int min;
  int max;
  int avg;
}ANALOG_CHANNEL;

namespace TOOLS
{
  // Init analog data
  void InitAnalogData();

  // Sample iterate and average
  void SampleAnalogs();

  // Retrun analog channel volts(mv)
  int ReadAnalogChannel(int aChannel);

	// Sample 10-bit AD multiple times, get avg and calc on avg
//	int GetMilliVoltsFromAnalog(int aChannel, int aAverages = 1);

  // Translate float into characters
  bool GetFromFloat(float aFlt, char *aMsg);

  // Translate interger to characters
  bool GetFromInteger(int aInt, char *aMsg);
}