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
static struct pt pt1, pt2, pt3, pt4; // each protothread needs one of these
static const int displayInterval = 800;//250;
static const int controlInterval = 1000;

extern volatile unsigned long highTime;
extern volatile unsigned long lowTime;

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
///////////////////////////////////////////////////////////////////////////////

void test()
{
  NANO_DISPLAY::test();

  M_CONTROL::toggleLED();
  delay(500);
}

// Keypad input thread
static int KeypadThread(struct pt *pt, int aInterval)
{
  PT_BEGIN(pt);
  Serial.println(F("Started KeypadThread()"));
  static unsigned long timestamp = 0;
  while (1) {
    // Capture analog keypad inputs
    PT_WAIT_UNTIL(pt, KEY_PAD::readKeypad(&oldKeyPadKey) == true);
    if ((!keypadTrigger) && oldKeyPadKey != keypadKey) {
      keypadKey = oldKeyPadKey;
      keypadTrigger = true;
      Serial.print("Key   : ");
      Serial.println(keypadKey);
    }
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
      controlState = M_DEBUG::ProcessDebug(keypadKey);
      break;
    }

    // Reset keypad input trigger after processing
    if (keypadKey != KEY_NONE) {
      keypadKey = KEY_NONE;
      keypadTrigger = false;
    }

    PT_WAIT_UNTIL(pt, millis() - timestamp > aInterval);
    timestamp = millis();
  }
  PT_END(pt);
}

static int PulseThread(struct pt *pt, int aInterval)
{
  PT_BEGIN(pt);
  Serial.println(F("Started PulseThread()"));
  static unsigned long timestamp = 0;
  while (1) {
    highTime = pulseIn(A2, HIGH);  // read high time
   lowTime = pulseIn(A2, LOW);    // read low time

    PT_WAIT_UNTIL(pt, millis() - timestamp > aInterval);
    timestamp = millis();
  }
  PT_END(pt);
}

// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop() {
  test();
/*
  KeypadThread(&pt1, NULL);
  DisplayThread(&pt2, displayInterval);
  ControlThread(&pt3, controlInterval);
*/
}
//////////////////////////////////////////////////////////////////////////////
