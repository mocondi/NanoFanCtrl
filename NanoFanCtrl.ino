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

#define CONTROL_INTERVAL        1000
#define CONFIG_INTERVAL          100

// Globals
int keypadKey = KEY_NONE;
int oldKeyPadKey = KEY_NONE;
bool keypadTrigger = false;
int controlState = STATE_CONTROL;
unsigned long currentTime;
unsigned long lastTime;
unsigned long setControlInterval = CONTROL_INTERVAL;

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

// Read analog inputs
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

// Process control variabless
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

// Udpate LCD display
void UpdateDisplay()
{
  M_CONTROL::UpdateControlDisplay();
}

// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop() {

  // Process interval
  currentTime = millis();
  if (currentTime - lastTime >= setControlInterval) {

    // Adjust interval based on state
    switch (controlState) {
    case STATE_CONTROL:
      setControlInterval = CONTROL_INTERVAL;
      break;
    case STATE_CONFIG:
      setControlInterval = CONFIG_INTERVAL;
      break;
    case STATE_DEBUG:
      setControlInterval = CONTROL_INTERVAL;
      break;
    case STATE_IDLE:
    default:
      setControlInterval = CONTROL_INTERVAL;
      break;
    }

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
