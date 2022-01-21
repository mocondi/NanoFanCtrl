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

void KEY_PAD::initKeypad()
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

int KEY_PAD::getKeypadVolts()
{
  return TOOLS::getMilliVoltsFromAnalog(KeypadPin);
}

int KEY_PAD::readKeypad()
{
  int ivolt = TOOLS::getMilliVoltsFromAnalog(KeypadPin, KEY_SAMPLE_COUNT);

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

  return KEY_NONE;
}

void KEY_PAD::processKeyStates(int &aState, int aKey)
{

}