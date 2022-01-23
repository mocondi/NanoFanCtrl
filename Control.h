#pragma once
namespace M_CONTROL
{
  void initControl();
  void UpdateControlDisplay();
  int ProcessFanControl(int &aKey);
  int processFanControl(float temperature);

  void toggleLED();
  void heartBeatLED();
}
