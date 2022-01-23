#pragma once
namespace M_CONTROL
{
  void initControl();
  int handleFanControl(int &aKey);
  int processFanControl(float temperature);

  void toggleLED();
  void heartBeatLED();
}
