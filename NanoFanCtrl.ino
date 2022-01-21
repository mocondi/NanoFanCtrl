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
const int OLED_POWER = 4;
int setSpeed = 20;
int index = 25;
bool loopIttr = false;
int keypadState = STATE_IDLE;

static struct pt pt1, pt2; // each protothread needs one of these

void setup()
{
  Serial.println(F("setup() started..."));

  // Enable Board Status LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Enable LCD power
  pinMode(OLED_POWER, OUTPUT);
  digitalWrite(OLED_POWER, LOW);

  // Serial port debug print speed
  Serial.begin(9600);

  if(!OLED_initDisplay()) {
    Serial.println(F("OLED or driver failed!"));
  }

  // Init our software modules
  initTemperature();
  initFan();
  initControl();
  initKeypad();

  // Start with fan off
  controlFanSpeed(0);

  // If keypad left arrow pressed during startup, set to debug mode
  if (getKeypadVolts() <= 100) {
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
  setSpeed = processFanControl(probeTemp);

  // Control fan speed
  controlFanSpeed(setSpeed);

  // Read fan speed
  int fanSpeed = getFanSpeed();
  
  // Update LCD
    OLED_setTempAndSpeed(probeTemp, setSpeed, fanSpeed);
/*
  // If after boot time, turn off LCD if not running
  if (iterationCounter >= 5 && setSpeed == 0) {
    digitalWrite(OLED_POWER, HIGH);
  } else {
    digitalWrite(OLED_POWER, LOW);
  }
*/  
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


void processKeypad(int kstate /*, unsigned long ms*/)
{
  if (kstate == STATE_IDLE) {
    keypadState = STATE_DEBUG;
  } else {
    keypadState = STATE_IDLE;
  }

  if (keypadState == STATE_DEBUG) {
    debugIO();
  }
/*  
  switch(keypadState) {
    case STATE_IDLE:
      break;
    case STATE_CONTROL:
      break;
    case STATE_CONFIG:
      break;
    case STATE_DEBUG:
      break;
    default:
      break;
  }
*/
}

void debugIO()
{
  // Read all analogs and update voltages
  int A0_Keypad = TOOLS::getMilliVoltsFromAnalog(A0);
  int A1_Temp = TOOLS::getMilliVoltsFromAnalog(A1);
  int A2_Tach = TOOLS::getMilliVoltsFromAnalog(A2);

  OLED_debugClear();
  OLED_debug(0, "DEBUG");

  char message[128];
  sprintf(message, "Keypad: %d", A0_Keypad);
  OLED_debug(1, message);
  sprintf(message, "Temp: %d", A1_Temp);
  OLED_debug(2, message);
  sprintf(message, "Tach: %d", A2_Tach);
  OLED_debug(3, message);

  OLED_debugWrite();

}

/* This function toggles the LED after 'interval' ms passed */
static int protothread1(struct pt *pt, int interval) {
  
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) { // never stop 
    keypadState = readKeypad();  

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    toggleLED();
  }
  PT_END(pt);
  
}
/* exactly the same as the protothread1 function */
static int protothread2(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {

    if(gDebugMode)
      debugIO();
    else 
      smartFanControl();

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
    toggleLED();
  }
  PT_END(pt);
}


void loop() {
  protothread1(&pt1, 500); // schedule the two protothreads
  protothread2(&pt2, 1000); // by calling them infinitely
//    int key = readKeypad();
//      processKeypad(key, currentMillis);
/*
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > processInterval) {
    previousMillis = currentMillis;
    switch(key)
    {
      case KEY_LEFT:
        OLED_debug("Left");
        break;
      case KEY_UP:
        OLED_debug("Up");
        break;
      case KEY_DOWN:
        OLED_debug("Down");
        break;
      case KEY_RIGHT:
        OLED_debug("Right");
        break;
      case KEY_ENTER:
        OLED_debug("Enter");
        break;
      
      case KEY_NONE:
      default:
        OLED_debug("None");
        break;
    }
//*/
//    smartFanControl(currentMillis);
//    LED_HeartBeat();
//    iterationCounter++;
//  }
}

