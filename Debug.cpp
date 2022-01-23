/*
* Debug.cpp
* Handles debugging and output information
*
*/
#include <Arduino.h>

#include "Debug.h"
#include "Tools.h"
#include "Display.h"
#include "Keypad.h"

void M_DEBUG::UpdateDebugDisplay()
{
  NANO_DISPLAY::clearDisplay();
  NANO_DISPLAY::setMessage("Debug", 0, 2);
  NANO_DISPLAY::refreshDisplay();
}

int M_DEBUG::ProcessDebug(int &aKey)
{
//  debugIO();
  if (aKey != KEY_NONE) {
    return STATE_CONTROL;
  }
  return STATE_DEBUG;
}

void M_DEBUG::debugIO()
{
  // Read all analogs and update voltages
  int A0_Keypad = TOOLS::getMilliVoltsFromAnalog(A0, 10);
  int A1_Temp = TOOLS::getMilliVoltsFromAnalog(A1, 10);
  int A2_Tach = TOOLS::getMilliVoltsFromAnalog(A2);

  NANO_DISPLAY::clearDisplay();
  NANO_DISPLAY::setMessage("DEBUG :-)", 0, 2);

  char message[128];
  sprintf(message, "Keypad: %d", A0_Keypad);
  NANO_DISPLAY::setMessage(message, 1, 1);
  sprintf(message, "Temp: %d", A1_Temp);
  NANO_DISPLAY::setMessage(message, 2, 1);
  sprintf(message, "Tach: %d", A2_Tach);
  NANO_DISPLAY::setMessage(message, 3, 1);

  NANO_DISPLAY::refreshDisplay();
}

void M_DEBUG::debugKeypad()
{
  NANO_DISPLAY::clearDisplay();
  NANO_DISPLAY::setMessage(0, "Keypad Debug");
  char message[128];

  int key = 0;
  if (!KEY_PAD::readKeypad(&key)) return;
  switch(key)
  {
    case KEY_LEFT:
      sprintf(message, "LEFT");
      break;
    case KEY_UP:
      sprintf(message, "UP");
      break;
    case KEY_DOWN:
      sprintf(message, "DOWN");
      break;
    case KEY_RIGHT:
      sprintf(message, "RIGHT");
      break;
    case KEY_ENTER:
      sprintf(message, "ENTER");
      break;
    case KEY_NONE:
      sprintf(message, "NONE");
      break;
    default:
      sprintf(message, "Default");
      break;
  }
  NANO_DISPLAY::setMessage(message, 1, 2);
  NANO_DISPLAY::refreshDisplay();

}

