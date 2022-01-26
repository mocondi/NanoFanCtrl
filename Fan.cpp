#include <Arduino.h>
#include "Fan.h"
#include "Tools.h"

volatile unsigned long highTime = 1;
volatile unsigned long lowTime = 1;

void pulseCountHigh()
{
  highTime++;
}

void pulseCountLow()
{
  lowTime++;
}

void M_FAN::initFan()
{
    pinMode(FAN_PWM_PIN, OUTPUT);
    pinMode(FAN_TACH_PIN, INPUT);
}


// Return RPM speed
int M_FAN::getFanSpeed()
{
  // 5000000 = 5 sec, 250000 .25 seconds
  highTime = pulseIn(FAN_TACH_PIN, HIGH, 50000);  // read high time
  lowTime = pulseIn(FAN_TACH_PIN, LOW, 50000);    // read low time

  float period = highTime + lowTime;                // Period = Ton + Toff

  if(period <= 0.0F) return 0;
  float freq = 1000000 / period; // getting frequency with totalTime is in Micro seconds
  float RPM = (freq / 2) * 60;   // we div by 2 since the fan tach outputs 2 pulses per revolution

  if (RPM > 99999.0F) return 9999;

  return (int)RPM;
}

// Set speed on percentage
void M_FAN::controlFanSpeed(int percent)
{
  // Regular PWM conversion
  //float dac = ((float)percent * 256.0F) / 100.0F;
  // Inverted PWM
  float dac = ((float)percent * -2.56F) + 255.0F;

  analogWrite(FAN_PWM_PIN, (int)dac);
}