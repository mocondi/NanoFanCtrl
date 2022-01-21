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

#define SCREEN_WIDTH    128 // OLED display width, in pixels
#define SCREEN_HEIGHT    64 // OLED display height, in pixels
#define OLED_RESET       -1
#define SCREEN_ADDRESS 0x3C

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


bool OLED_initDisplay() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
        return false;
    }
    delay(200);

    display.clearDisplay();

    return true;
}

void OLED_setTitle() {
    display.clearDisplay();
    display.setTextColor(WHITE);

    // Top title
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Temp   Fan");
//    display.display();
}

void OLED_setTempAndSpeed(float fTemp, int iFan, int iRPM)
{
    char cBuff[8];

    // Clear and set title
    OLED_setTitle();
    
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
    sprintf(cFan, "%d%%", iFan);
    display.println(cFan);

    // Fan RPM reading
//    display.setTextSize(2);
    display.setCursor(30, 51);
    sprintf(cFan, "%d RPM", iRPM);
    display.println(cFan);

    // Write to display
    display.display();
}

void OLED_debug(int line, char *message)
{
      
  display.setTextColor(WHITE);

  // Small size below yellow line
  display.setTextSize(1);
  switch(line)
  {
    case 0:
      display.setCursor(0,0);
      break;
    case 1:
      display.setCursor(0,20);
      break;
    case 2:
      display.setCursor(0,30);
      break;
    case 3:
      display.setCursor(0,40);
      break;
    case 4:
      display.setCursor(0,50);
      break;
    default:
      display.setCursor(0,20);
      break;
  }

  display.println(message);
}

void OLED_debugWrite()
{
  display.display();
}

void OLED_debugClear()
{
  display.clearDisplay();
}
