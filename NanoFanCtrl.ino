/*
* NanoFanCtrl.ino
* Main Arduino file
*
* By: Mark Ocondi
*/
//*
#include <pt.h> // Protothreads 3rd party library, Near-true multi-task

#include "Display.h"
#include "Temperature.h"
#include "Fan.h"
#include "Control.h"
#include "Keypad.h"
#include "Tools.h"
#include "Config.h"
#include "Debug.h"
int keypadKey = KEY_NONE;
int oldKeyPadKey = KEY_NONE;
volatile bool keypadTrigger = false;
volatile int controlState = STATE_IDLE;
static struct pt pt1, pt2, pt3; // each protothread needs one of these
static const int displayInterval = 250;
static const int controlInterval = 500;

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
  M_TEMPERATURE::initTemperature();
  M_FAN::initFan();
  M_CONTROL::initControl();
  KEY_PAD::initKeypad();

  // Start with fan off
  M_FAN::controlFanSpeed(0);

  // If keypad left arrow pressed during startup, set to debug mode
  if (KEY_PAD::getKeypadVolts() <= 100) {
    controlState = STATE_DEBUG;
  }

  PT_INIT(&pt1);  // initialise the two
  PT_INIT(&pt2);  // protothread variables
  PT_INIT(&pt3);

  Serial.println(F("setup() completed..."));
}


// Keypad input thread
static int KeypadThread(struct pt *pt, int aInterval)
{
  PT_BEGIN(pt);
  Serial.println(F("Started KeypadThread()"));
  static unsigned long timestamp = 0;
  while (1) {
    // Do something
    PT_WAIT_UNTIL(pt, KEY_PAD::readKeypad(&oldKeyPadKey) == true);
    if ((!keypadTrigger) && oldKeyPadKey != keypadKey) {
//    if (!keypadTrigger) {
      keypadKey = oldKeyPadKey;
      keypadTrigger = true;
      Serial.print("Key   : ");
      Serial.println(keypadKey);
    }
// Set flag to handle keypad
//    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
//    timestamp = millis();
  }
  PT_END(pt);
}

// OLED Display thread
static int DisplayThread(struct pt *pt, int aInterval)
{
  PT_BEGIN(pt);
  Serial.println(F("Started DisplayThread()"));
  static unsigned long timestamp = 0;
  while (1) 
  {
      switch (controlState) {
      case STATE_IDLE:
      case STATE_CONTROL:
        M_CONTROL::UpdateControlDisplay();
        break;
      case STATE_CONFIG:
        M_CONFIG::UpdateConfigDisplay();
        break;
      case STATE_DEBUG:
        M_DEBUG::UpdateDebugDisplay();
      default:
        break;
      }

    PT_WAIT_UNTIL(pt, millis() - timestamp > aInterval);
    timestamp = millis();
  }
  PT_END(pt);
}

// Contorl thread
static int ControlThread(struct pt *pt, int aInterval)
{
  PT_BEGIN(pt);
  Serial.println(F("Started ControlThread()"));
  static unsigned long timestamp = 0;
  while (1) {
    // Do something
    switch (controlState)
    {
    case STATE_IDLE:
    case STATE_CONTROL:
      controlState = M_CONTROL::ProcessFanControl(keypadKey);
//      if (keypadKey != KEY_NONE) {
//        keypadKey = KEY_NONE;
//        keypadTrigger = false;
//      }
      break;
    case STATE_CONFIG:
      controlState = M_CONFIG::ProcessConfig(keypadKey);
//      if (controlState == STATE_CONTROL) {
//        keypadTrigger = false;
//      }
   
      break;
    case STATE_DEBUG:
    default:
//      M_DEBUG::handleDebug(keypadKey);
      break;
    }
    
    if (keypadKey != KEY_NONE) {
      keypadKey = KEY_NONE;
      keypadTrigger = false;
    }

    PT_WAIT_UNTIL(pt, millis() - timestamp > aInterval);
    timestamp = millis();
  }
  PT_END(pt);
}

// MAIN LOOP /////////////////////////////////////////////////////////////////
static unsigned long counter = 0;
void loop() {
  KeypadThread(&pt1, NULL);
  DisplayThread(&pt2, displayInterval);
  ControlThread(&pt3, controlInterval);

  // schedule the two protothreads that run indefinitely
  //  keypadThread(&pt1, keypadIntrval);  // Process every .5 seconds
  //  workerThread(&pt2, workerInterval);       // Process every 1 second


}

//*/
