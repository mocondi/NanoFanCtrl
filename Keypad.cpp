/*
* Keypad.cpp
* Handles keypad interface
*
*/
#include <Arduino.h>

#include "Keypad.h"
#include "Display.h"
#include "Tools.h"

const int KeypadPin =	    A0;
int keyValues[KEY_NONE];

#define KEY_SAMPLE_COUNT  5

void initKeypad()
{
//  pinMode(KeypadPin, INPUT);
//  pinMode(KeypadPin, INPUT_PULLUP);
  keyValues[KEY_LEFT]   = DEFAULT_LEFT;
  keyValues[KEY_UP]     = DEFAULT_UP;
  keyValues[KEY_DOWN]   = DEFAULT_DOWN;
  keyValues[KEY_RIGHT]  = DEFAULT_RIGHT;
  keyValues[KEY_ENTER]  = DEFAULT_ENTER;
  keyValues[KEY_NONE]   = DEFAULT_NONE;
}

int getKeypadVolts()
{
  return TOOLS::getMilliVoltsFromAnalog(KeypadPin);
}

int readKeypad()
{
  int ivolt = TOOLS::getMilliVoltsFromAnalog(KeypadPin, KEY_SAMPLE_COUNT);

char message[124];
char ctemp[32];
itoa(ivolt, ctemp, 10);
sprintf(message, "Keypad Volts: %s", ctemp);
Serial.println(message);

  // Check idle and voltage source
  if (ivolt >= (DEFAULT_NONE-V_OFFSET) && ivolt <= (DEFAULT_NONE-V_OFFSET)) {
    return KEY_NONE;
  }
  if (ivolt < 10) return KEY_LEFT;

  for (int i=KEY_LEFT; i<KEY_NONE; i++) {
    if (ivolt >= (keyValues[i] - V_OFFSET) && ivolt <= (keyValues[i] + V_OFFSET)) {
      return i;
    }
  }
/*
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
*/
  return KEY_NONE;

}