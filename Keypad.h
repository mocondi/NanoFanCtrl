#pragma once
extern void initKeypad();
extern int getKeypadVolts();
extern int readKeypad();

#define KEY_LEFT      0
#define KEY_UP        1
#define KEY_DOWN      2
#define KEY_RIGHT     3
#define KEY_ENTER     4
#define KEY_NONE      5

#define DEFAULT_LEFT  0     // 
#define DEFAULT_UP    927   // 
#define DEFAULT_DOWN  2124  // 
#define DEFAULT_RIGHT 3271  // 
#define DEFAULT_ENTER 4800  // 

enum {
  STATE_IDLE = 0,
  STATE_CONTROL,
  STATE_CONFIG,
  STATE_DEBUG,
  MAX_STATE
};