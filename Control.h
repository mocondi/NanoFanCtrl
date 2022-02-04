#pragma once

#define NANO_EEPROM_SIZE    1024
#define SERIAL_OFFSET       1020// 4 bytes at the end
#define SERIAL_NUMBER       56963

namespace M_CONTROL
{
  void initControl();
  void UpdateControlDisplay();
  int ProcessFanControl(int aKey);
  int GetFanSpeedFromTemp(float temperature);

  void toggleLED();
  void heartBeatLED();
}
