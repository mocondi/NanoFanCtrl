#pragma once
extern void initKeypad();
extern int getKeypadVolts();
extern int handleKeypad();

#define KEY_LEFT    1
#define KEY_UP      2
#define KEY_DOWN    3
#define KEY_RIGHT   4
#define KEY_ENTER   5
#define KEY_NONE    0

enum {
  STATE_IDLE = 0,
  STATE_CONTROL,
  STATE_CONFIG,
  STATE_DEBUG,
  MAX_STATE
};