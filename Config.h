#pragma once
namespace M_CONFIG
{
  // process config screen IO and return state
  int handleConfig(int &aKey);
}

enum {
    MENU_SEL_TEMP_SEL = 0,
    MENU_SEL_TEMP_ADJ,
    MENU_SEL_KEYPAD_SEL,
    MENU_SEL_KEYPAD_ADJ,
    MENU_SEL_DEBUG,
    MENU_SEL_LAST
};