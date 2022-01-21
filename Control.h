#pragma once
namespace M_CONTROL
{
  void initControl();
  int processFanControl(float temperature);

  void toggleLED();
  void heartBeatLED();
}
