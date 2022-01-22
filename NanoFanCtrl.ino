/*
* NanoFanCtrl.ino
* Main Arduino file
*
* By: Mark Ocondi
*/
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
int newKey = KEY_NONE;
int controlState = STATE_IDLE;

static struct pt pt1, pt2; // each protothread needs one of these

// MAIN SETUP /////////////////////////////////////////////////////////////////
void setup()
{
  Serial.println(F("setup() started..."));

  // Enable Board Status LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial port debug print speed
  Serial.begin(9600);

  // Init our software modules
  if(!NANO_DISPLAY::initDisplay()) {
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

  Serial.println(F("setup() completed..."));
}

// Process keypad inputs
static int keypadThread(struct pt *pt, int interval) {
  Serial.println(F("Started keypadThread()"));
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) { // never stop 

    newKey = KEY_PAD::readKeypad();
Serial.println(F("a"));
    if (keypadKey != newKey) {
      keypadKey = newKey;
      KEY_PAD::processKeyStates(controlState, keypadKey);
    } 
Serial.println(F("b"));
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    M_CONTROL::toggleLED();
Serial.println(F("c"));
  }
  PT_END(pt);
}

// Process work
static int workerThread(struct pt *pt, int interval) {
  Serial.println(F("Started workerThread()"));
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {

    switch (controlState)
    {
    case STATE_IDLE:
    case STATE_CONTROL:
      M_CONTROL::handleFanControl();
      break;
    case STATE_CONFIG:
      M_CONFIG::handleConfig(keypadKey);
      break;
    case STATE_DEBUG:
    default:
      M_DEBUG::handleDebug(keypadKey);
      break;
    }    

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
  }
  PT_END(pt);
}

// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop() {
  // schedule the two protothreads that run indefinitely
  keypadThread(&pt1, 500);  // Process every .5 seconds
  workerThread(&pt2, 1000);       // Process every 1 second
}

