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

#define SCREEN_WIDTH    128 // OLED display width, in pixels
#define SCREEN_HEIGHT    64 // OLED display height, in pixels
#define OLED_RESET       -1
#define SCREEN_ADDRESS 0x3C

const int OLED_POWER = 4;

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


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


bool NANO_DISPLAY::initDisplay() {
  // Enable LCD power
  pinMode(OLED_POWER, OUTPUT);
  digitalWrite(OLED_POWER, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
      return false;
  }
  delay(200);

  display.clearDisplay();
  display.setTextColor(WHITE);


    display.clearDisplay();
 
  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }
 
  delay(1000);

  return true;
}

void NANO_DISPLAY::setTempAndSpeed(float aTemp, int aFan, int aRPM)
{
    char cBuff[8];

    // Clear and set title
    clearDisplay();
    setMessage("Temp   Fan", 0, 2);
    
    const int yoffset = 22;

    // Temperature
    display.setTextSize(3);
    display.setCursor(0, yoffset);
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
    display.setTextSize(2);
    display.setCursor(80, yoffset);
    memset(cBuff, NULL, sizeof(cBuff));
    //    itoa(aFan, cBuff, 10);
    char cFan[8];
    sprintf(cFan, "%3d%%", aFan);
    display.println(cFan);

    // Fan RPM reading
//    display.setTextSize(2);
    display.setCursor(30, 51);
    sprintf(cFan, "%4d RPM", aRPM);
    display.println(cFan);

    // Write to display
    display.display();
}

void NANO_DISPLAY::setConfigData(float aTemp, int aPercent)
{

}

void NANO_DISPLAY::setMessage(char *aMessage, int aLine, int aSize)
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

  display.drawBitmap(0, 0, icon_LeftArrow, 16, 16, 1);
  display.drawBitmap(32, 0, icon_UpArrow, 16, 16, 1);
  display.drawBitmap(64, 0, icon_DownArrow, 16, 16, 1);
  display.drawBitmap(96, 0, icon_RightArrow, 16, 16, 1);

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