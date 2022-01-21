#pragma once

namespace NANO_DISPLAY
{
  bool initDisplay();
  void setTitle();
  void setTempAndSpeed(float fTemp, int iFan, int iRPM);
  void debug(int line, char *message);
  void debugWrite();
  void debugClear();
}


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