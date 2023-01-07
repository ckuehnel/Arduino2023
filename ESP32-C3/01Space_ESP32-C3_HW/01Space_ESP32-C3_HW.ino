/*
 * File: 01Space_ESP32-C3_HW.ino
 * 
 * based on https://github.com/01Space/ESP32-C3-0.42LCD/blob/main/HelloWorld/HelloWorld.ino
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define SDA_PIN 5
#define SCL_PIN 6
#define RGB_PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.42" OLED

void setup(void) 
{
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
  pixel.begin();
  pixel.setBrightness(50);
  pixel.show(); // Initialize all pixels to 'off'
}

void loop(void) 
{
  pixel.setPixelColor(0, 127, 127, 127); // White
  pixel.show(); delay(20);
  pixel.setPixelColor(0, 0, 0, 0);      // Blank
  pixel.show();
  u8g2.clearBuffer();               // clear the internal memory
  u8g2.sendBuffer();                // transfer internal memory to the display
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  delay(1000);
  u8g2.drawStr(0,10,"Hello World"); // write something to the internal memory
  u8g2.sendBuffer();                // transfer internal memory to the display
  delay(1000);
  u8g2.drawStr(0,25,"by ESP32-C3");    // write something to the internal memory
  u8g2.sendBuffer();                // transfer internal memory to the display
  delay(1000);  
  u8g2.drawStr(0,40,"RISC-V MCU");  // write something to the internal memory
  u8g2.sendBuffer();                // transfer internal memory to the display
  delay(1000);
}
