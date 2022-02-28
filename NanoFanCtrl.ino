/*
* NanoFanCtrl.ino
* Main Arduino file
*
* By: Mark Ocondi
*
* TODO: linear | log fan pcnt to temp calc.
*       Config settings complete
*/

#include "Display.h"
#include "Temperature.h"
#include "Fan.h"
#include "Control.h"
#include "Keypad.h"
#include "Tools.h"
#include "Config.h"
#include "Debug.h"

// Globals
int keypadKey = KEY_NONE;
int oldKeyPadKey = KEY_NONE;
bool keypadTrigger = false;
int controlState = STATE_IDLE;
unsigned long currentTime;
unsigned long lastTime;
const unsigned long setControlInterval = 1000;

// MAIN SETUP /////////////////////////////////////////////////////////////////
void setup()
{
  Serial.println(F("setup() started..."));

  // Enable Board Status LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial port debug print speed
  Serial.begin(9600);

  // Init our software modules
  if (!NANO_DISPLAY::initDisplay()) {
    Serial.println(F("OLED or driver failed!"));
  }
  TOOLS::InitAnalogData();
  M_TEMPERATURE::initTemperature();
  M_FAN::initFan();
  M_CONTROL::initControl();
  KEY_PAD::initKeypad();

  // Start with fan off
  M_FAN::controlFanSpeed(0);

  // If keypad left arrow pressed during startup, set to debug mode
  TOOLS::SampleAnalogs();
/*
  if (KEY_PAD::getKeypadVolts() <= 100) {
    controlState = STATE_DEBUG;
  }
*/
  currentTime = millis();
  lastTime = currentTime;

  Serial.println(F("setup() completed..."));
}
///////////////////////////////////////////////////////////////////////////////

void ReadAnalogs()
{
  TOOLS::SampleAnalogs();

  if (KEY_PAD::readKeypad(&oldKeyPadKey)) {
    if ((!keypadTrigger) && oldKeyPadKey != keypadKey) {
      keypadKey = oldKeyPadKey;
      keypadTrigger = true;
      Serial.print("Key   : ");
      Serial.println(keypadKey);
    }
  }
}

void UpdateControl()
{
  //Serial.println(F("UpdateControl() started..."));
  // Process the current state
  switch (controlState)
  {
  case STATE_IDLE:
  case STATE_CONTROL:
    controlState = M_CONTROL::ProcessFanControl(keypadKey);
    break;
  case STATE_CONFIG:
    controlState = M_CONFIG::ProcessConfig(keypadKey);
    break;
  case STATE_DEBUG:
  default:
    //      controlState = M_DEBUG::ProcessDebug(keypadKey);
    //      aInterval = 500;
    break;
  }

  // Reset keypad input trigger after processing
  if (keypadKey != KEY_NONE) {
    keypadKey = KEY_NONE;
    keypadTrigger = false;
  }
}

void UpdateDisplay()
{
  M_CONTROL::UpdateControlDisplay();
}

// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop() {

  currentTime = millis();
  if (currentTime - lastTime >= setControlInterval) {
    // Read analogs
    ReadAnalogs();

    // Update control
    UpdateControl();

    // Update display
    UpdateDisplay();

    // Updates lastTime
    lastTime = currentTime;
  }
}
//////////////////////////////////////////////////////////////////////////////
