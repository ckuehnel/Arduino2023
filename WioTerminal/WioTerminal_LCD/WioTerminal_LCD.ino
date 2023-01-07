#include"TFT_eSPI.h"
TFT_eSPI tft;
 
void setup() {
  tft.begin();
  tft.setRotation(3);
 
  tft.fillScreen(TFT_BLUE); //Red background
 
  tft.setTextColor(TFT_WHITE);          //sets the text colour to black
  tft.setTextSize(2);                   //sets the size of text
  tft.drawString("Hello world!", 0, 0); //prints strings from (0, 0)
  tft.setTextSize(4);
  tft.drawString("Hello world!", 0, 40);
}
 
void loop() {}
