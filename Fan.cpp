#include <Arduino.h>
#include "Fan.h"

const int FanPWM =    3;
const int FanTach =  A2;
extern unsigned long highTime;
extern unsigned long lowTime;

void M_FAN::initFan()
{
    pinMode(FanPWM, OUTPUT);
    pinMode(FanTach, INPUT);
}

// Return RPM speed
int M_FAN::getFanSpeed()
{
  unsigned long highTime = pulseIn(FanTach, HIGH);  // read high time
  unsigned long lowTime = pulseIn(FanTach, LOW);    // read low time
  float period = highTime + lowTime;                // Period = Ton + Toff

Serial.println();
Serial.println(period);
  if(period <= 0.0F) return 0;
  float freq = 1000000 / period;                    // getting frequency with totalTime is in Micro seconds
  float RPM = (freq / 2) * 60;                      // we div by 2 since the fan tach outputs 2 pulses per revolution
Serial.println(RPM);

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

  analogWrite(FanPWM, (int)dac);
}