/*
  3 Color OLED .96" module driver
*
*
* Library: https://github.com/desklab/desklab-arduino-lib
// Size 1     (12345678901234567890) 20 Max?
// Size 2       (1234567890) 10 Max
// Size 3         (1234567)   7 Max
*/

#include <Wire.h>
#include <Adafruit_GFX.h> // 3rd party library Adafruit GFX 
#include <Adafruit_SSD1306.h> // 3rd party library Adafruit SSD1306
#include "Display.h"
#include "Tools.h"
#include "Debug.h"
//*
static const unsigned char PROGMEM icon_LeftArrow[] = {
  0x01, 0x80,
  0x03, 0x80,
  0x07, 0x80,
  0x0f, 0x80,
  0x1f, 0xff,
  0x3f, 0xff,
  0x7f, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0x7f, 0xff,
  0x3f, 0xff,
  0x1f, 0xff,
  0x0f, 0x80,
  0x07, 0x80,
  0x03, 0x80,
  0x01, 0x80
};

static const unsigned char PROGMEM icon_RightArrow[] = {
  0x01, 0x80,
  0x01, 0xc0,
  0x01, 0xe0,
  0x01, 0xf0,
  0xff, 0xf8,
  0xff, 0xfc,
  0xff, 0xfe,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xfe,
  0xff, 0xfc,
  0xff, 0xf8,
  0x01, 0xf0,
  0x01, 0xe0,
  0x01, 0xc0,
  0x01, 0x80
};

static const unsigned char PROGMEM icon_UpArrow[] = {
  0x01, 0x80,
  0x03, 0xc0,
  0x07, 0xe0,
  0x0f, 0xf0,
  0x1f, 0xf8,
  0x3f, 0xfc,
  0x7f, 0xfe,
  0xff, 0xff,
  0xff, 0xff,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0
};

static const unsigned char PROGMEM icon_DownArrow[] = {
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0x0f, 0xf0,
  0xff, 0xff,
  0xff, 0xff,
  0x7f, 0xfe,
  0x3f, 0xfc,
  0x1f, 0xf8,
  0x0f, 0xf0,
  0x07, 0xe0,
  0x03, 0xc0,
  0x01, 0x80
};

static const unsigned char PROGMEM icon_Smile[] = {
  0x00, 0x00,
  0x00, 0x00,
  0x18, 0x18,
  0x3c, 0x3c,
  0x3c, 0x3c,
  0x18, 0x18,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x20, 0x04,
  0x10, 0x08,
  0x10, 0x08,
  0x08, 0x10,
  0x07, 0xe0,
  0x00, 0x00,
  0x00, 0x00
};

static const unsigned char PROGMEM icon_Home[] = {
  0x01, 0x80,
  0x02, 0x40,
  0x04, 0x20,
  0x08, 0x10,
  0x10, 0x08,
  0x3f, 0xfc,
  0x50, 0x0a,
  0x13, 0xc8,
  0x12, 0x48,
  0x12, 0x48,
  0x13, 0xc8,
  0x10, 0x08,
  0x1f, 0xf8,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00
};
//*/

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 200000, 100000);


bool NANO_DISPLAY::initDisplay() {

  // Enable LCD power
  pinMode(DISPLAY_POWER_PIN, OUTPUT);
  digitalWrite(DISPLAY_POWER_PIN, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
      return false;
  }
  delay(20);

  display.clearDisplay();
  display.setTextColor(WHITE);

  // Startup logo animation
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
  delay(500);

  // Show company
  display.clearDisplay();
  setMessage("Pho Que", 1, 3);
  display.display();
  delay(500);

  return true;
}

void NANO_DISPLAY::setTempAndSpeed(float aTemp, int aFan, int aRPM)
{
/*
  DebugPrintln(NULL);
  DEBUG_PRINTLN(aTemp);
  DEBUG_PRINTLN(aFan);
  DEBUG_PRINTLN(aRPM);
*/
  char cBuff[16];

  // Clear and set title
  clearDisplay();
  setMessage("Temp   Fan", 0, DISP_TOP_SIZE);

  // Temperature
  display.setTextSize(DISP_TEMP_SIZE);
  display.setCursor(0, Y_OFFSET);
  // Integer if 100 >
  if (aTemp >= 100.0F) {
      itoa((int)aTemp, cBuff, 10);
  }
  // Float if 100 < with 1 decimal point
  else {
      dtostrf(aTemp, 3, 1, cBuff);
  }
  display.println(cBuff);

  // Fan control percent
  display.setTextSize(DISP_FAN_SIZE);
  display.setCursor(80, Y_OFFSET);
  memset(cBuff, NULL, sizeof(cBuff));
  if (aFan <= 0) {
    sprintf(cBuff, "0%%");
  }
  else {
    sprintf(cBuff, "%3d%%", aFan);
  }
  display.println(cBuff);
/*
  // Fan RPM reading
  memset(cBuff, NULL, sizeof(cBuff));
  display.setTextSize(DISP_RPM_SIZE);
  display.setCursor(25, Y_OFFSET +28);
//  sprintf(cBuff, "%4d RPM", aRPM);
 sprintf(cBuff, "%d RPM", aRPM);
//  display.println("1234 RPM");
  display.println(cBuff);
*/
//  display.display();
  memset(cBuff, NULL, sizeof(cBuff));
  char tb[6];
  itoa(aRPM, tb, 10);
  sprintf(cBuff, "%s RPM", tb);
  display.setTextSize(1);
//  display.setCursor(30, 46);
  display.setCursor(70, 46);

  display.println(cBuff);
//display.println("1234 RPM");

  // Write to display
  display.display();
}

void NANO_DISPLAY::setConfigData(float aTemp, int aPercent)
{
  char message[128];
  char ftemp[16];
  dtostrf(aTemp, 3, 1, ftemp);
  sprintf(message, "Temp: %s", ftemp);
  setMessage(message, 2, 2);

  itoa(aPercent, ftemp, 10);
  sprintf(message, "Percent: %s", ftemp);
  setMessage(message, 3, 2);
}

void NANO_DISPLAY::setDebugData(int AD[8])
{
  display.clearDisplay();
//  display.drawBitmap(0, 0, icon_Smile, 16, 16, 1);
//  display.drawBitmap(96, 0, icon_Home, 16, 16, 1);


  display.setTextSize(2);
  const int Y_SPACE = 16;
  int offset = 20;

  char cMsg[128];
  for (int i = 0; i < 3; i++) {
    sprintf(cMsg, "AD %d: %d", i, AD[i]);
    display.setCursor(0, offset);
    display.println(cMsg);
    offset += Y_SPACE;
  }

/*
  offset = 16;
  for (int i = 4; i < 8; i++) {
    sprintf(cMsg, "AD %d: %d", i, AD[0]);
    display.setCursor(64, offset);
    display.println(cMsg);
    offset += Y_SPACE;
  }
*/

  display.display();

}

void NANO_DISPLAY::setMessage(const char *aMessage, int aLine, int aSize)
{
  // Small size below yellow line
  display.setTextSize(aSize);
  switch (aLine)
  {
  case 0:
    display.setCursor(0, OFFSET_ZERO);
    break;
  case 1:
    display.setCursor(0, OFFSET_ONE);
    break;
  case 2:
    display.setCursor(0, OFFSET_TWO);
    break;
  case 3:
    display.setCursor(0, OFFSET_THREE);
    break;
  case 4:
    display.setCursor(0, OFFSET_FOUR);
    break;
  default:
    display.setCursor(0, OFFSET_ONE);
    break;
  }

  display.println(aMessage);
}

void NANO_DISPLAY::refreshDisplay() {
  display.display();
}

void NANO_DISPLAY::clearDisplay() {
  display.clearDisplay();
}

void NANO_DISPLAY::showArrowIcons()
{
  display.clearDisplay();
/*
  display.drawBitmap(0, 0, icon_LeftArrow, 16, 16, 1);
  display.drawBitmap(32, 0, icon_UpArrow, 16, 16, 1);
  display.drawBitmap(64, 0, icon_DownArrow, 16, 16, 1);
  display.drawBitmap(96, 0, icon_RightArrow, 16, 16, 1);
*/
  display.display();
/*
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
*/
}

void NANO_DISPLAY::test()
{
/*
  display.clearDisplay();

  display.drawBitmap(0, 0, icon_LeftArrow, 16, 16, 1);
  display.drawBitmap(32, 0, icon_UpArrow, 16, 16, 1);
  display.drawBitmap(64, 0, icon_DownArrow, 16, 16, 1);
  display.drawBitmap(96, 0, icon_RightArrow, 16, 16, 1);
    
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
*/
}