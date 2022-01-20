//#include "Tools.h"
#include <Arduino.h>

#define SAMPLE_COUNT      10
const int TempPin =      A1;
//const float MX =     -26.0F;
//const float B =     4820.0F;
//const float MX =     -22.5F;
//const float B =     4575.0F;
//const float MX =     -32.5F;
//const float B =     5175.0F;
const float MX =     -28.12F;
const float B =     4941.4F;

void initTemperature()
{
    pinMode(TempPin, INPUT);
    pinMode(TempPin, INPUT_PULLUP);
}

float sampleTempPower()
{
    int ADAvg = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        ADAvg += analogRead(TempPin);
    }
    int AD = (ADAvg / SAMPLE_COUNT);
    // Translate to voltage
    float volt = ((float)AD * 5000.0F) / 1024.0F;

    return volt;
}

float sampleTemperature()
{
    // Sample 10-bit AD multiple times, get avg and calc on avg
    int ADAvg = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        ADAvg += analogRead(TempPin);
    }
    int AD = (ADAvg / SAMPLE_COUNT);
    // Translate to voltage
    float volt = ((float)AD * 5000.0F) / 1024.0F;
    // Translate voltage to temperature F
    // Y = Mx+b, Y = -22.5 + 4575
    // X = (Y-B)/M
    float temp = (volt-B)/MX;
/*
    Serial.println();
    Serial.print("AD   : ");
    Serial.println(AD);
    Serial.print("Volt: ");
    Serial.println(volt, 4);
    Serial.print("Temp: ");
    Serial.println(temp, 4);
*/
    return temp;
}