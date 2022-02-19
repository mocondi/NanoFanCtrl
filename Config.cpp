/*
* Config.cpp
* Handles configurations
*
*/
#include <Arduino.h>

#include "Config.h"
#include "Display.h"
#include "Keypad.h"


static int configMenuItem = MENU_SEL_TEMP_SEL;
static volatile int configTableIndex = 0;

void M_CONFIG::UpdateConfigDisplay()
{
  NANO_DISPLAY::clearDisplay();
  switch (configMenuItem)
  {
  case MENU_SEL_TEMP_SEL:
    NANO_DISPLAY::setMessage("Config Tmp", 0, 2);
    NANO_DISPLAY::setMessage("Enter to adjust", 1, 1);
    break;
  case MENU_SEL_TEMP_ADJ:
    NANO_DISPLAY::setMessage("Adjust Tmp", 0, 2);
//    NANO_DISPLAY::setConfigData(tempTable[configTableIndex].temp, tempTable[configTableIndex].percent);
    break;
  case MENU_SEL_KEYPAD_SEL:
    NANO_DISPLAY::setMessage("Config Key", 0, 2);
    NANO_DISPLAY::setMessage("Enter to adjust", 1, 1);
    break;
  case MENU_SEL_KEYPAD_ADJ:
    NANO_DISPLAY::setMessage("Adjust Key", 0, 2);
    break;
  case MENU_SEL_LAST:
  default:
    break;
  }
  NANO_DISPLAY::refreshDisplay();
}

int M_CONFIG::ProcessConfig(int &aKey)
{
/*
  Serial.print(F("handleConfig() aKey: "));
  Serial.print(aKey);
  Serial.print(F(" MenuItem: "));
  Serial.println(configMenuItem);
*/

  // Process key inputs
  switch (configMenuItem)
  {
  case MENU_SEL_TEMP_SEL:
  {
    if (aKey == KEY_LEFT) {
      return STATE_CONTROL;
    }
    else if (aKey == KEY_RIGHT) {
      configMenuItem = MENU_SEL_TEMP_SEL;
      return STATE_DEBUG;
    }
    else if ((aKey == KEY_DOWN) || (aKey == KEY_ENTER)) {
      configMenuItem = MENU_SEL_TEMP_ADJ;
    }
  }
    break;
  case MENU_SEL_TEMP_ADJ:
  {
    if (aKey == KEY_ENTER) {
      configMenuItem = MENU_SEL_TEMP_SEL;
      return STATE_CONTROL;
    }
  }
    break;
  case MENU_SEL_KEYPAD_SEL:
  {
    if (aKey == KEY_LEFT) {
      configMenuItem = MENU_SEL_TEMP_SEL;
    }
    else if (aKey == KEY_RIGHT) {
      configMenuItem = MENU_SEL_TEMP_SEL;
      return STATE_CONTROL;
    }
    else if ((aKey == KEY_DOWN) || (aKey == KEY_ENTER)) {
      configMenuItem = MENU_SEL_KEYPAD_ADJ;
    }
  }
    break;
  case MENU_SEL_KEYPAD_ADJ:
  {
    if (aKey == KEY_ENTER) {
      configMenuItem = MENU_SEL_TEMP_SEL;
      return STATE_CONTROL;
    }
  }
    break;
  case MENU_SEL_LAST:
  default:
    break;
  }

  return STATE_CONFIG;
}