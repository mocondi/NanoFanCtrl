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

static const int keypadIntrval = 250;
static const int workerInterval = 1000;
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
  PT_BEGIN(pt);
  static unsigned long timestamp = 0;
  Serial.println(F("Started keypadThread()"));

  while(1) { // never stop 

    keypadKey = KEY_PAD::readKeypad();
    if (KEY_NONE != keypadKey) {
      KEY_PAD::processKeyStates(controlState, keypadKey);
    } 
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    M_CONTROL::toggleLED();
  }
  PT_END(pt);
}

// Process work
static int workerThread(struct pt *pt, int interval) {
  PT_BEGIN(pt);
  Serial.println(F("Started workerThread()"));
  static unsigned long timestamp = 0;
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

    PT_WAIT_UNTIL(pt, millis() - timestamp > workerInterval );
    timestamp = millis();
  }
  PT_END(pt);
}


// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop() {
  // schedule the two protothreads that run indefinitely
  keypadThread(&pt1, keypadIntrval);  // Process every .5 seconds
  workerThread(&pt2, workerInterval);       // Process every 1 second
}

