/*
* Config.cpp
* Handles configurations
*
*/
#include <Arduino.h>

#include "Config.h"
#include "Display.h"

int configMenuItem = MENU_SEL_LAST;

void M_CONFIG::handleConfig(int aKey)
{
/*
  switch (configMenuItem)
  {
  case MENU_SEL_TEMP_ADJ:
    break;
  case MENU_SEL_KEYPAD_ADJ:
    break;
  case MENU_SEL_DEBUG:
    break;
  case MENU_SEL_LAST:
    configMenuItem = MENU_SEL_TEMP_ADJ;
    break;
  default:
    break;
  }
*/
  NANO_DISPLAY::clearDisplay();
  //NANO_DISPLAY::setTitle("Config Mnu");
  NANO_DISPLAY::setMessage("Config Mnu", 0, 2);
  NANO_DISPLAY::setMessage("mARK IT EH STB", 1, 1);
  NANO_DISPLAY::setMessage("Something", 1, 2);
  NANO_DISPLAY::setMessage("Have fun", 1, 3);
  NANO_DISPLAY::setMessage("Eat more!", 1 ,4);
  NANO_DISPLAY::updateDisplay();
}