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

/*
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

//defining House Icon
byte housechar1[8] = { B00000, B00001, B00011, B00011, //Row 0, Col 0
                  B00111, B01111, B01111, B11111, };
byte housechar2[8] = { B11111, B11111, B11100, B11100, //Row 1, Col 0
                  B11100, B11100, B11100, B11100, };
byte housechar3[8] = { B00000, B10010, B11010, B11010, //ROW 0, Col 1
                  B11110, B11110, B11110, B11111, };
byte housechar4[8] = { B11111, B11111, B11111, B10001, //Row 1, Col 1
                    B10001, B10001, B11111, B11111, };
//Definining Termometer Icon
byte tempchar1[8] = { B00000, B00001, B00010, B00100, //Row 0, Col 2
                  B00100, B00100, B00100, B00111, };
byte tempchar2[8] = { B00111, B00111, B00111, B01111, //Row 1, Col 2
                  B11111, B11111, B01111, B00011, };
byte tempchar3[8] = { B00000, B10000, B01011, B00100, //ROW 0, Col 3
                  B00111, B00100, B00111, B11100, };
byte tempchar4[8] = { B11111, B11100, B11100, B11110, //Row 1, Col 3
                  B11111, B11111, B11110, B11000, };

//Defining Humidity Icon
byte humchar1[8] = { B00000, B00001, B00011, B00011,
                  B00111, B01111, B01111, B11111, };
byte humchar2[8] = { B11111, B11111, B11111, B01111,
                  B00011, B00000, B00000, B00000, };
byte humchar3[8] = { B00000, B10000, B11000, B11000,
                  B11100, B11110, B11110, B11111, };
byte humchar4[8] = { B11111, B11111, B11111, B11110,
                  B11100, B00000, B00000, B00000, };

uint16_t wifiHotspot_icon_W = 16;
uint16_t wifiHotspot_icon_H = 16;
static const unsigned char PROGMEM wifiHotspot_icon[] = {
B00000000, B00000000,
B00000000, B00000000,
B00000000, B00000000,
B00100000, B00000100,
B01001000, B00010010,
B01010010, B01001010,
B01010100, B00101010,
B01010101, B10101010,
B01010101, B10101010,
B01010100, B00101010,
B01010010, B01001010,
B01001000, B00010010,
B00100000, B00000100,
B00000000, B00000000,
B00000000, B00000000,
B00000000, B00000000
};

static const uint8_t image_data_Down[32] = {
    0xfa, 0xaf,
    0xf7, 0xef,
    0xff, 0x4f,
    0xf6, 0xef,
    0xff, 0x5f,
    0xf6, 0xcf,
    0xff, 0x6f,
    0x55, 0x52,
    0x7d, 0xa8,
    0xb6, 0x95,
    0xcd, 0x53,
    0xf6, 0xaf,
    0xf2, 0x8f,
    0xfd, 0x5f,
    0xfd, 0xbf,
    0xfe, 0x7f
};
const

typedef struct {
  const uint8_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t dataSize;
} tImage;
*/


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

void NANO_DISPLAY::setTempAndSpeed(float fTemp, int iFan, int iRPM)
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
    if (fTemp >= 100.0F) {
        itoa((int)fTemp, cBuff, 10);
    }
    // Float if 100 < with 1 decimal point
    else {
        dtostrf(fTemp, 3, 1, cBuff);
    }
    display.println(cBuff);

    // Fan control percent
    display.setTextSize(2);
    display.setCursor(80, yoffset);
    memset(cBuff, NULL, sizeof(cBuff));
    //    itoa(iFan, cBuff, 10);
    char cFan[8];
    sprintf(cFan, "%3d%%", iFan);
    display.println(cFan);

    // Fan RPM reading
//    display.setTextSize(2);
    display.setCursor(30, 51);
    sprintf(cFan, "%4d RPM", iRPM);
    display.println(cFan);

    // Write to display
    display.display();
}

void NANO_DISPLAY::setConfigData(void *data)
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

void NANO_DISPLAY::test()
{
  display.clearDisplay();

  display.drawBitmap(0, 0, icon_LeftArrow, 16, 16, 1);
  display.drawBitmap(32, 0, icon_UpArrow, 16, 16, 1);
  display.drawBitmap(64, 0, icon_DownArrow, 16, 16, 1);
  display.drawBitmap(96, 0, icon_RightArrow, 16, 16, 1);

  display.display();
}