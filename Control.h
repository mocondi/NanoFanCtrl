#pragma once

#define NANO_EEPROM_SIZE    1024
#define SERIAL_OFFSET       1020// 4 bytes at the end
#define SERIAL_NUMBER       5696

namespace M_CONTROL
{
  void initControl();
  void UpdateControlDisplay();
  int ProcessFanControl(int aKey);
  int GetFanSpeedFromTemp(float temperature);
  float getSlope(float X[2], int Y[2]);
  int getYIntercept(float X, int Y, float M);

  void toggleLED();
  void heartBeatLED();
}
