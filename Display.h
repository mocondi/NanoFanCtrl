#pragma once

#define OFFSET_ZERO       0
#define OFFSET_ONE       20
#define OFFSET_TWO       30
#define OFFSET_THREE     40
#define OFFSET_FOUR      50
#define OFFSET_FIVE      60

#define MAX_TABLE         6
#define Y_OFFSET         22
//*
#define DISP_TOP_SIZE     2
#define DISP_TEMP_SIZE    3
#define DISP_FAN_SIZE     2
#define DISP_RPM_SIZE     2
//*/
/*
#define DISP_TOP_SIZE     1
#define DISP_TEMP_SIZE    1
#define DISP_FAN_SIZE     1
#define DISP_RPM_SIZE     1
*/
#define SCREEN_WIDTH    128 // OLED display width, in pixels
#define SCREEN_HEIGHT    64 // OLED display height, in pixels
#define OLED_RESET       -1
#define SCREEN_ADDRESS 0x3C

namespace NANO_DISPLAY
{
  bool initDisplay();
  void setTempAndSpeed(float aTemp, int aFan, int aRPM);
  void setConfigData(float aTemp, int aPercent);
  void setDebugData(int AD[8]);
  void setMessage(const char *aMessage, int aLine = 1, int aSize = 1);
  void refreshDisplay();
  void clearDisplay();
  void showArrowIcons();
  void test();
}

typedef struct _PAIR {
  float temp;
  int percent;
} PAIR;


/*  
  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
*/  
/*
//  int a = displayTest(2);
//  char tout[256];
//  printf(tout, "%d", a);
//  Serial.println(F(tout);
  //Serial.println(F("displayTest() :")); 

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Eat my ASS!");
  display.display(); 
  delay(1000);

  	// Display Inverted Text
	display.setTextColor(BLACK, WHITE); // 'inverted' text
	display.setCursor(0,28);
	display.println("Hello world!");
	display.display();
	delay(2000);
	display.clearDisplay();

  display.setTextColor(WHITE);

  // Top title
  display.setTextSize(2);
	display.setCursor(0,0);
	display.println("Temp   Fan");

  // Data Temp
  display.setTextSize(3);
	display.setCursor(0,20);
	display.println("95.3");

  // Data Fan speed
//	display.setCursor(68,20);
//	display.setCursor(60,20);
//	display.println("100%");

  //Desc
  display.setTextSize(1);
	display.setCursor(0,36);
	display.println("Degrees");

  display.display();

*/
//display.startscrolldiagleft(0x00, 0x07);