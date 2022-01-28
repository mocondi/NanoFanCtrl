#pragma once

//#define SERIAL_DEBUG_ENABLED

namespace M_DEBUG
{
  void UpdateDebugDisplay();
  int ProcessDebug(int &aKey);
  void debugIO();
  void debugKeypad();
}

#ifdef SERIAL_DEBUG_ENABLED 
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#define DebugPrint(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.print(__VA_ARGS__)
#define DebugPrintln(...)  \
        Serial.print(millis());     \
        Serial.print(": ");    \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print(' ');      \
        Serial.print(__LINE__);     \
        Serial.print(' ');      \
        Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#define DebugPrint(...)
#define DebugPrintln(...)  
#endif