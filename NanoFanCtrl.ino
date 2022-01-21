/*
* NanoFanCtrl.ino
* Main Arduino file
*
* By: Mark Ocondi
*/


//#include <stdlib.h>
//#include <StandardCplusplus.h>
#include <pt.h> // Protothreads 3rd party library, Near-true multi-task

#include "Display.h"
#include "Temperature.h"
#include "Fan.h"
#include "Control.h"
#include "Keypad.h"
#include "Tools.h"

unsigned long processInterval = 1000;
unsigned long previousMillis = 0;
unsigned long iterationCounter = 0;
bool gDebugMode = false;
int setSpeed = 20;
int index = 25;
bool loopIttr = false;
int keypadKey = KEY_NONE;
int newKey = KEY_NONE;
int controlState = STATE_IDLE;

static struct pt pt1, pt2; // each protothread needs one of these

void setup()
{
  Serial.println(F("setup() started..."));

  // Enable Board Status LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial port debug print speed
  Serial.begin(9600);

  if(!NANO_DISPLAY::initDisplay()) {
    Serial.println(F("OLED or driver failed!"));
  }

  // Init our software modules
  initTemperature();
  initFan();
  M_CONTROL::initControl();
  KEY_PAD::initKeypad();

  // Start with fan off
  controlFanSpeed(0);

  // If keypad left arrow pressed during startup, set to debug mode
  if (KEY_PAD::getKeypadVolts() <= 100) {
    gDebugMode = true;
  }

  PT_INIT(&pt1);  // initialise the two
  PT_INIT(&pt2);  // protothread variables

  Serial.println(F("setup() completed..."));
}

void smartFanControl(/* unsigned long &curMillisec */)
{
  // Read temperature
  float probeTemp = sampleTemperature();
  
  // Set fan speed
  setSpeed = M_CONTROL::processFanControl(probeTemp);

  // Control fan speed
  controlFanSpeed(setSpeed);

  // Read fan speed
  int fanSpeed = getFanSpeed();
  
  // Update LCD
  NANO_DISPLAY::setTempAndSpeed(probeTemp, setSpeed, fanSpeed);
 
}

void toggleLED() {
  boolean ledstate = digitalRead(LED_BUILTIN); // get LED state
  ledstate ^= 1;   // toggle LED state using xor
  digitalWrite(LED_BUILTIN, ledstate); // write inversed state back
}

void LED_HeartBeat()
{
  const int hDelay = 80; 
  const int hOff = hDelay * 6;
  if (loopIttr) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(hDelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(hDelay*2);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(hDelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(hDelay);
    loopIttr = false;
  } else {
//    delay(hOff);
    delay(180);
    loopIttr = true;
  }
}

void debugIO()
{
  // Read all analogs and update voltages
  int A0_Keypad = TOOLS::getMilliVoltsFromAnalog(A0, 10);
  int A1_Temp = TOOLS::getMilliVoltsFromAnalog(A1, 10);
  int A2_Tach = TOOLS::getMilliVoltsFromAnalog(A2);

  NANO_DISPLAY::debugClear();
  NANO_DISPLAY::debug(0, "DEBUG");

  char message[128];
  sprintf(message, "Keypad: %d", A0_Keypad);
  NANO_DISPLAY::debug(1, message);
  sprintf(message, "Temp: %d", A1_Temp);
  NANO_DISPLAY::debug(2, message);
  sprintf(message, "Tach: %d", A2_Tach);
  NANO_DISPLAY::debug(3, message);

  NANO_DISPLAY::debugWrite();
}

void debugKeypad()
{
  NANO_DISPLAY::debugClear();
  NANO_DISPLAY::debug(0, "Keypad Debug");
  char message[128];

  switch(KEY_PAD::readKeypad())
  {
    case KEY_LEFT:
      sprintf(message, "LEFT");
      break;
    case KEY_UP:
      sprintf(message, "UP");
      break;
    case KEY_DOWN:
      sprintf(message, "DOWN");
      break;
    case KEY_RIGHT:
      sprintf(message, "RIGHT");
      break;
    case KEY_ENTER:
      sprintf(message, "ENTER");
      break;
    case KEY_NONE:
      sprintf(message, "NONE");
      break;
    default:
      sprintf(message, "Default");
      break;
  }
  NANO_DISPLAY::debug(2, message);
  NANO_DISPLAY::debugWrite();
}

// Process keypad inputs
static int keypadThread(struct pt *pt, int interval) {
  
  static unsigned long timestamp = 0;
  static char message[124];
  static char ctemp[32];
  PT_BEGIN(pt);
  while(1) { // never stop 
    newKey = KEY_PAD::readKeypad();
    if (keypadKey != newKey) {
      controlState = newKey;
      KEY_PAD::processKeyStates(controlState, keypadKey);
    } 

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    toggleLED();
  }
  PT_END(pt);
  
}
// Process work
static int fanCtrlThread(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {

    switch (controlState)
    {
    case STATE_IDLE:
    case STATE_CONTROL:
      smartFanControl();
      break;
    case STATE_CONFIG:
      break;
    case STATE_DEBUG:
    default:
      debugIO();
      break;
    }    

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
//    toggleLED();
  }
  PT_END(pt);
}


void loop() {
  // schedule the two protothreads that run indefinitely
  keypadThread(&pt1, 500);    // Process every .5 seconds
  fanCtrlThread(&pt2, 1000);  // Process every 1 second
//debugIO();
//debugKeypad();
}

