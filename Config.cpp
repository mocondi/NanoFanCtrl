/*
* Config.cpp
* Handles configurations
*
*/
#include <Arduino.h>

#include "Config.h"
#include "Display.h"
#include "Keypad.h"

int configMenuItem = MENU_SEL_TEMP_SEL;

int M_CONFIG::handleConfig(int &aKey)
{
  Serial.print(F("handleConfig() aKey: "));
  Serial.print(aKey);
  Serial.print(F(" MenuItem: "));
  Serial.println(configMenuItem);

//  if (aKey == KEY_NONE) return STATE_CONFIG;
//*
  NANO_DISPLAY::clearDisplay();
  switch (configMenuItem)
  {
  case MENU_SEL_TEMP_SEL:
    NANO_DISPLAY::setMessage("Config Tmp", 0, 2);
    NANO_DISPLAY::setMessage("Enter to start", 1, 1);
    NANO_DISPLAY::setMessage("Left or Right to exit", 2, 1);
    if(aKey == KEY_LEFT) {
      configMenuItem = MENU_SEL_TEMP_SEL;
      aKey = KEY_NONE;
      return STATE_CONTROL;
    }
    else if (aKey == KEY_RIGHT) {
      configMenuItem = MENU_SEL_KEYPAD_SEL; }
    break;
  case MENU_SEL_KEYPAD_SEL:
    NANO_DISPLAY::setMessage("Config Key", 0, 2);
    if(aKey == KEY_LEFT) {
      configMenuItem = MENU_SEL_TEMP_SEL;
    } 
    else if (aKey == KEY_RIGHT) {
      configMenuItem = MENU_SEL_DEBUG; }
    break;
  case MENU_SEL_DEBUG:
    NANO_DISPLAY::setMessage("Config DBG", 0, 2);
    if(aKey == KEY_LEFT)  {
      configMenuItem = MENU_SEL_KEYPAD_SEL; }
    else if (aKey == KEY_RIGHT) { 
      configMenuItem = MENU_SEL_TEMP_SEL; 
      aKey = KEY_NONE;
      return STATE_CONTROL; }
    break;

  default:
    break;
  }
  aKey = KEY_NONE;
  NANO_DISPLAY::updateDisplay();

  return STATE_CONFIG;
}