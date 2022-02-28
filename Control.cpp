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


static float probeTemp = 0;
static int setSpeed = 0;
static int fanSpeed = 0;
float gM;
int gB;
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
    tempCtrlStr.MinFanPcnt = 30;
    tempCtrlStr.MaxTemp = 85.0;
    tempCtrlStr.MaxFanPcnt = 98;

    // Store defaults into EEPROM
    EEPROM.put(0, tempCtrlStr);
    EEPROM.put(SERIAL_OFFSET, SERIAL_NUMBER);
  }
  // Read settings
  else {
    EEPROM.get(0, tempCtrlStr);
  }
  // Calculate slope values
  float tmpX[2];
  int tmpY[2];
  tmpX[0] = tempCtrlStr.MinTemp;
  tmpX[1] = tempCtrlStr.MaxTemp;
  tmpY[0] = tempCtrlStr.MinFanPcnt;
  tmpY[1] = tempCtrlStr.MaxFanPcnt;
  gM = getSlope(tmpX, tmpY);
  gB = getYIntercept(tmpX[0], tmpY[0], gM);
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
  if (temperature < tempCtrlStr.MinTemp) {
//Serial.println("return below level");
    return 0;
  }
  // 2nd if above max
  if (temperature >= tempCtrlStr.MaxTemp) {
//Serial.println("return above max");
    return tempCtrlStr.MinFanPcnt;
  }

  // Control on slope equation
  float rc = (gM * temperature) + gB;
  if (rc < tempCtrlStr.MinFanPcnt)
    return tempCtrlStr.MinFanPcnt;
  else if (rc > tempCtrlStr.MaxFanPcnt)
    return tempCtrlStr.MaxFanPcnt;
  else 
    return rc;
}

float M_CONTROL::getSlope(float X[2], int Y[2])
{
  return ((Y[1] - Y[0]) / (X[1] - X[0]));
}

int M_CONTROL::getYIntercept(float X, int Y, float M)
{
  float b = ((M * X) * -1) + Y;
  return (int)b;
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


