#include <Arduino.h>

const int FanPWM =    3;
const int FanTach =  A2;

void initFan()
{
    pinMode(FanPWM, OUTPUT);
    pinMode(FanTach, INPUT);
}

// Return RPM speed
int getFanSpeed()
{
  unsigned long highTime = pulseIn(FanTach, HIGH);  // read high time
  unsigned long lowTime = pulseIn(FanTach, LOW);    // read low time
  float period = highTime + lowTime;                // Period = Ton + Toff
  float freq = 1000000 / period;                    // getting frequency with totalTime is in Micro seconds
  float RPM = (freq / 2) * 60;                      // we div by 2 since the fan tach outputs 2 pulses per revolution

/*
    Serial.println();
    Serial.println(period);
    Serial.println(RPM);
*/
  return (int)RPM;
}

// Set speed on percentage
void controlFanSpeed(int percent)
{
//    float dac = ((float)percent * 256.0F) / 100.0F;
    // Inverted PWM
    float dac = ((float)percent * -2.56F) + 255.0F;

    analogWrite(FanPWM, (int)dac);
}