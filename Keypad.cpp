/*
* Keypad.cpp
* Handles keypad interface
*
*/
#include <Arduino.h>

#include "Keypad.h"
#include "Display.h"
#include "Tools.h"

static int keyValues[KEY_NONE];

#define KEY_SAMPLE_COUNT  20

void KEY_PAD::initKeypad()
{
  pinMode(KEYBOARD_PIN, INPUT);
//  pinMode(KEYBOARD_PIN, INPUT_PULLUP);

  keyValues[KEY_LEFT]   = DEFAULT_LEFT;
  keyValues[KEY_UP]     = DEFAULT_UP;
  keyValues[KEY_DOWN]   = DEFAULT_DOWN;
  keyValues[KEY_RIGHT]  = DEFAULT_RIGHT;
  keyValues[KEY_ENTER]  = DEFAULT_ENTER;
  keyValues[KEY_NONE]   = DEFAULT_NONE;
}

int KEY_PAD::getKeypadVolts()
{
  return TOOLS::ReadAnalogChannel(KEYBOARD_CHANNEL);
}

bool KEY_PAD::readKeypad(int *aKey)
{
  int ivolt = TOOLS::ReadAnalogChannel(KEYBOARD_CHANNEL);

  // Check idle and voltage source
  if (ivolt >= (DEFAULT_NONE-V_OFFSET) && ivolt <= (DEFAULT_NONE-V_OFFSET)) {
    return false;
  }
  if (ivolt < 10) {
    *aKey = KEY_LEFT;
    return true;
  }

  for (int i=KEY_LEFT; i<KEY_NONE; i++) {
    if (ivolt >= (keyValues[i] - V_OFFSET) && ivolt <= (keyValues[i] + V_OFFSET)) {
      *aKey = i;
      return true;
    }
  }

  return false;
}












