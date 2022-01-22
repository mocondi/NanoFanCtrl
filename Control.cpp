/*
* Control.cpp
* Handles fan control
*
*/
#include <Arduino.h>
#include "Control.h"
#include "Temperature.h"
#include "Fan.h"
#include "Display.h"

typedef struct _PAIR{
  float temp;
  int percent;
} PAIR;

#define MAX_TABLE 6

_PAIR tempTable[MAX_TABLE];
int setSpeed = 20;
const int minFanSpeed = 25; // Min speed in percentage
const int maxFanSpeed = 100; // Max speed in percentage
bool gIsRunning = false;
bool loopIttr = false;


void M_CONTROL::initControl()
{
    tempTable[0].temp = 72.0F;
    tempTable[0].percent = 30;

    tempTable[1].temp = 75.0F;
    tempTable[1].percent = 35;

    tempTable[2].temp = 80.0F;
    tempTable[2].percent = 50;

    tempTable[3].temp = 85.0F;
    tempTable[3].percent = 75;

    tempTable[4].temp = 90.0F;
    tempTable[4].percent = 80;

    tempTable[5].temp = 100.0F;
    tempTable[5].percent = 90;
  }

void M_CONTROL::handleFanControl()
{
  // Read temperature
  float probeTemp = M_TEMPERATURE::sampleTemperature();

  // Set fan speed
  setSpeed = processFanControl(probeTemp);

  // Control fan speed
  M_FAN::controlFanSpeed(setSpeed);

// MJO getFanSpeed() kills kb task!  
M_FAN::getFanSpeed();
  // Read fan speed
//  int fanSpeed = M_FAN::getFanSpeed(); 
int fanSpeed; 
  NANO_DISPLAY::setTempAndSpeed(probeTemp, setSpeed, fanSpeed);
}

int M_CONTROL::processFanControl(float temperature)
{
  // 1st if below lowest disable fan
  if (temperature < tempTable[0].temp) {
    return 0;
  }
  // 2nd if above max
  if (temperature >= tempTable[MAX_TABLE - 1].temp) {
    return maxFanSpeed;
  }

  // Control from table
  for( int i =1; i < MAX_TABLE; i++) {
    if ((temperature > tempTable[i].temp) && 
        (temperature <= tempTable[i+1].temp)) {
      return tempTable[i].percent;
    }
  }

}

void M_CONTROL::toggleLED() {
  boolean ledstate = digitalRead(LED_BUILTIN); // get LED state
  ledstate ^= 1;   // toggle LED state using xor
  digitalWrite(LED_BUILTIN, ledstate); // write inversed state back
}

void M_CONTROL::heartBeatLED()
{
  const int hDelay = 80; 
  const int hOff = hDelay * 6;
  if (loopIttr) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(hDelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(hDelay*2);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(hDelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(hDelay);
    loopIttr = false;
  } else {
//    delay(hOff);
    delay(180);
    loopIttr = true;
  }
}


