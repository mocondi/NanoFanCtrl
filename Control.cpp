/*
* Control.cpp
* Handles fan control
*
*/
#include <Arduino.h>
#include <EEPROM.h>
#include "Control.h"
#include "Temperature.h"
#include "Fan.h"
#include "Display.h"
#include "Keypad.h"
#include "Debug.h"


static volatile float probeTemp = 0;
static volatile int setSpeed = 0;
static volatile int fanSpeed = 0;

//const int minFanSpeed = 25; // Min speed in percentage
static const int maxFanSpeed = 98; // Max speed in percentage
static bool loopIttr = false;

typedef struct {
  float MinTemp;
  float MaxTemp;
  int MinFanPcnt;
  int MaxFanPcnt;
} TEMP_CONTROL;

TEMP_CONTROL tempCtrlStr;


void M_CONTROL::initControl()
{
  int serial = 0;
  EEPROM.get(SERIAL_OFFSET, serial);
  // Write defaults
  if (serial != SERIAL_NUMBER) {
    // Set defaults
    tempCtrlStr.MinTemp = 74.5;

/*
    tempTable[0].temp = 74.5F;
    tempTable[0].percent = 30;
    tempTable[1].temp = 76.0F;
    tempTable[1].percent = 35;
    tempTable[2].temp = 77.0F;
    tempTable[2].percent = 50;
    tempTable[3].temp = 78.0F;
    tempTable[3].percent = 75;
    tempTable[4].temp = 79.0F;
    tempTable[4].percent = 80;
    tempTable[5].temp = 80.0F;
    tempTable[5].percent = 90;
*/
    // Store defaults into EEPROM
/*
    int eOffset = 0;
    for (int i = 0; i < MAX_TABLE; i++) {
        EEPROM.put(eOffset, tempTable[i]);
        eOffset += sizeof(_PAIR);
    }
*/
    EEPROM.put(0, tempCtrlStr);
    EEPROM.put(SERIAL_OFFSET, SERIAL_NUMBER);
  }
  // Read settings
  else {
/*
    int eOffset = 0;
    for (int i = 0; i < MAX_TABLE; i++) {
      EEPROM.get(eOffset, tempTable[i]);
      eOffset += sizeof(_PAIR);
    Serial.print("Table: ");
    Serial.println(i);
    Serial.print("Temp: ");
    Serial.println(tempTable[i].temp);
    Serial.print("Pcnt: ");
    Serial.println(tempTable[i].percent);
    }
*/
  }
}

void M_CONTROL::UpdateControlDisplay()
{
  NANO_DISPLAY::setTempAndSpeed(probeTemp, setSpeed, fanSpeed);
}

int M_CONTROL::ProcessFanControl(int aKey)
{
//  DEBUG_PRINTLN("ProcessFanControl()");
//  DEBUG_PRINTLN(aKey);

  // Hendle keys
  if (aKey != KEY_NONE) return STATE_CONFIG;

  // Read temperature
  probeTemp = M_TEMPERATURE::sampleTemperature();
//  DEBUG_PRINTLN(probeTemp);

  // Set fan speed
  setSpeed = GetFanSpeedFromTemp(probeTemp);
//  DEBUG_PRINTLN(setSpeed);

  // Control fan speed
  M_FAN::controlFanSpeed(setSpeed);

  // Read fan speed
  fanSpeed = M_FAN::getFanSpeed();
//  DEBUG_PRINTLN(fanSpeed);

  return STATE_CONTROL;
}

int M_CONTROL::GetFanSpeedFromTemp(float temperature)
{
//  Serial.print(F("processFanControl(): "));
//  Serial.println(temperature);

  // 1st if below lowest disable fan
  if (temperature < tempTable[0].temp) {
//Serial.println("return below level");
    return 0;
  }
  // 2nd if above max
  if (temperature >= tempTable[MAX_TABLE - 1].temp) {
//Serial.println("return above max");
    return maxFanSpeed;
  }

  // Control from table
  for( int i =0; i < MAX_TABLE; i++) {
    if ((temperature >= tempTable[i].temp) && 
        (temperature <= tempTable[i+1].temp)) {
//Serial.print("From table index: ");
//Serial.println(i);
      return tempTable[i].percent;
    }
  }
//Serial.println("Finished loop");
  return 0;
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
    delay(180);
    loopIttr = true;
  }
}


