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
int keyValues[5];

#define KEY_SAMPLE_COUNT  5

void initKeypad()
{
//  pinMode(KeypadPin, INPUT);
//  pinMode(KeypadPin, INPUT_PULLUP);
}

int getKeypadVolts()
{
  return TOOLS::getMilliVoltsFromAnalog(KeypadPin);
}

int readKeypad()
{
  int ivolt = TOOLS::getMilliVoltsFromAnalog(KeypadPin, KEY_SAMPLE_COUNT);
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

}