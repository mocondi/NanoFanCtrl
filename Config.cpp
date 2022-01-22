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
//*
  NANO_DISPLAY::clearDisplay();
  switch (configMenuItem)
  {
  case MENU_SEL_TEMP_ADJ:
    NANO_DISPLAY::setMessage("Config Tmp", 0, 2);
    NANO_DISPLAY::setMessage("mARK IT EH STB", 1, 1);
    configMenuItem = MENU_SEL_KEYPAD_ADJ; 
    break;
  case MENU_SEL_KEYPAD_ADJ:
    NANO_DISPLAY::setMessage("Config Key", 0, 2);
    configMenuItem = MENU_SEL_KEYPAD_ADJ;
    break;
  case MENU_SEL_DEBUG:
    NANO_DISPLAY::setMessage("Config DBG", 0, 2);
    configMenuItem = MENU_SEL_DEBUG;
    break;
  case MENU_SEL_LAST:
    NANO_DISPLAY::setMessage("Kit Enter", 0, 2);
    NANO_DISPLAY::setMessage("to leave", 2, 2);
    NANO_DISPLAY::setMessage("Configrtn", 4, 2);
    configMenuItem = MENU_SEL_TEMP_ADJ;
    break;
  default:
    break;
  }
//*/
  //NANO_DISPLAY::setTitle("Config Mnu");
 
  NANO_DISPLAY::setMessage("Something", 2, 1);
  NANO_DISPLAY::setMessage("Have fun", 3, 1);
  NANO_DISPLAY::setMessage("Eat more!", 4 ,1);
  NANO_DISPLAY::updateDisplay();
}