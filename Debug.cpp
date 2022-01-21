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

void M_DEBUG::debugIO()
{
  // Read all analogs and update voltages
  int A0_Keypad = TOOLS::getMilliVoltsFromAnalog(A0, 10);
  int A1_Temp = TOOLS::getMilliVoltsFromAnalog(A1, 10);
  int A2_Tach = TOOLS::getMilliVoltsFromAnalog(A2);

  NANO_DISPLAY::debugClear();
  NANO_DISPLAY::debug(0, "DEBUG");

  char message[128];
  sprintf(message, "Keypad: %d", A0_Keypad);
  NANO_DISPLAY::debug(1, message);
  sprintf(message, "Temp: %d", A1_Temp);
  NANO_DISPLAY::debug(2, message);
  sprintf(message, "Tach: %d", A2_Tach);
  NANO_DISPLAY::debug(3, message);

  NANO_DISPLAY::debugWrite();
}

void M_DEBUG::debugKeypad()
{
  NANO_DISPLAY::debugClear();
  NANO_DISPLAY::debug(0, "Keypad Debug");
  char message[128];

  switch(KEY_PAD::readKeypad())
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
  NANO_DISPLAY::debug(2, message);
  NANO_DISPLAY::debugWrite();

}

