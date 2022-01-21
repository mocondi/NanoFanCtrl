#pragma once
namespace M_CONTROL
{
  void initControl();
  void handleFanControl();
  int processFanControl(float temperature);

  void toggleLED();
  void heartBeatLED();
}
