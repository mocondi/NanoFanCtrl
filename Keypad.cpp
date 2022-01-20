#include <Arduino.h>

#include "Keypad.h"
#include "Display.h"
#include "Tools.h"

const int KeypadPin = A0;

#define SAMPLE_COUNT      5

void initKeypad()
{
//  pinMode(KeypadPin, INPUT);
//  pinMode(KeypadPin, INPUT_PULLUP);
}

int getKeypadVolts()
{
  return TOOLS::getMilliVoltsFromAnalog(KeypadPin);
}

int handleKeypad()
{
  int ADAvg = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
      ADAvg += analogRead(KeypadPin);
  }
  int AD = (ADAvg / SAMPLE_COUNT);
  float volt = ((float)AD * 5000.0F) / 1024.0F;
  int ivolt = volt;
//*
  if (ivolt > 4900) return KEY_NONE;
  // 0.0
  if (ivolt < 10) return KEY_LEFT;
  // 927
  if (ivolt >= 900 && ivolt < 1100) return KEY_UP;
  // 2124
  if (ivolt >= 2000 && ivolt < 2500) return KEY_DOWN;
  // 3271
  if (ivolt >= 3000 && ivolt < 3500) return KEY_RIGHT;
  // 4800
  if (ivolt >= 4700 && ivolt < 4850) return KEY_ENTER;

  return KEY_NONE;
//*/
/*
    Serial.println();
    Serial.print("AD   : ");
    Serial.println(AD);
    Serial.print("Volt: ");
    Serial.println(volt, 4);
    Serial.print("iVolt   : ");
    Serial.println(ivolt);

  char message[128];
  char ctemp[16];
//  char ftemp[16];
//  dtostrf(volt, 3, 1, ftemp);
  itoa(volt, ctemp, 10);
  sprintf(message, "Volts: %s", ctemp);
  OLED_debug(message);
  return 0;
//*/  
}