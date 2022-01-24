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

#define KEY_SAMPLE_COUNT  20

void KEY_PAD::initKeypad()
{
  pinMode(KeypadPin, INPUT);
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

bool KEY_PAD::readKeypad(int *aKey)
{
  int ivolt = TOOLS::getMilliVoltsFromAnalog(KeypadPin, KEY_SAMPLE_COUNT);

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

/*
void KEY_PAD::processKeyStates(int &aState, int aKey)
{
  switch (aState)
  {
  case STATE_IDLE:
  case STATE_CONTROL:
    // Any key will move to config state
    if (aKey == KEY_NONE) break;
    else aState = STATE_CONFIG;    
    break;
  case STATE_CONFIG:
    // Config will pull state out or after timeout
    break;
  case STATE_DEBUG:
  default:
    break;
  }

  switch(aKey)
  {
    case KEY_LEFT:
      break;
    case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case KEY_RIGHT:
      break;
    case KEY_ENTER:
      break;
    case KEY_NONE:
      break;
    default:
      break;  
  }

}
*/











