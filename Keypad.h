#pragma once

namespace KEY_PAD
{
  void initKeypad();
  int getKeypadVolts();
  bool readKeypad(int *aKey);
  void processKeyStates(int &aState, int aKey);
}


#define POWER_USB 1

#ifndef POWER_USB
  #define DEFAULT_LEFT    0     // 
  #define DEFAULT_UP      927   // 
  #define DEFAULT_DOWN    2124  // 
  #define DEFAULT_RIGHT   3271  // 
  #define DEFAULT_ENTER   4800  // 
  #define DEFAULT_NONE    4995  // 
#else
  #define DEFAULT_LEFT    0     // 
  #define DEFAULT_UP      595   // 
  #define DEFAULT_DOWN    1362  // 
  #define DEFAULT_RIGHT   2093  // 
  #define DEFAULT_ENTER   3095  // 
  #define DEFAULT_NONE    4262  // 
#endif

#define V_OFFSET        100

// Used for int keypadKey
enum {
  KEY_LEFT = 0,
  KEY_UP,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_NONE
};

// Used for int controlState
enum {
  STATE_IDLE = 0,
  STATE_CONTROL,
  STATE_CONFIG,
  STATE_DEBUG,
  MAX_STATE
};