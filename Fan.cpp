#include <Arduino.h>
#include "Fan.h"

const int FanPWM =    3;
const int FanTach =  A2;
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
    pinMode(FanPWM, OUTPUT);
    pinMode(FanTach, INPUT);
//    attachInterrupt(digitalPinToInterrupt(FanTach), pulseCountHigh, CHANGE);
//    attachInterrupt(digitalPinToInterrupt(FanTach), pulseCountHigh, RISING);
//    attachInterrupt(digitalPinToInterrupt(FanTach), pulseCountLow, FALLING);
//    attachInterrupt(FanTach, pulseCountHigh, CHANGE);
}


// Return RPM speed
int M_FAN::getFanSpeed()
{
//  return 1;
//  unsigned long highTime = pulseIn(FanTach, HIGH);  // read high time
//  unsigned long lowTime = pulseIn(FanTach, LOW);    // read low time
  // 5000000 = 5 sec, 250000 .25 seconds
highTime = pulseIn(FanTach, HIGH, 50000);  // read high time
lowTime = pulseIn(FanTach, LOW, 50000);    // read low time

  float period = highTime + lowTime;                // Period = Ton + Toff
/* 
Serial.print("Pusle Hi: ");
Serial.println(highTime);
Serial.print("Pusle Low: ");
Serial.println(lowTime);
Serial.print("Period: ");
Serial.println(period);
//return 123;
//*/
  if(period <= 0.0F) return 0;
  float freq = 1000000 / period; // getting frequency with totalTime is in Micro seconds
  float RPM = (freq / 2) * 60;   // we div by 2 since the fan tach outputs 2 pulses per revolution
//Serial.println(RPM);

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