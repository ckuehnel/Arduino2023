#include <Arduino.h>
#include <U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
 
#define BOARD "Arduino Nano Every"
#define CONSOLE 0
 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
 
void setup(void) 
{
  if (CONSOLE)
  {
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB.
  
    Serial.print(F("\nHello World from ")); 
    Serial.println(BOARD);              // used board
    Serial.print(F("Arduino SW Version "));
    Serial.println(ARDUINO);           // Arduino SW version
    Serial.print(F("Clock frequency "));
    Serial.print(F_CPU / 1000000);     // Oscillator frequency
    Serial.println(F(" MHz"));
    Serial.println();
  }
  
  u8g2.begin();
  u8g2.clearBuffer();            // clear the internal memory
  u8g2.setFont(u8g2_font_7x13_tf); // choose a suitable font
  u8g2.drawStr(0, 10, "Hello World from ");  // write something to the internal memory
  u8g2.drawStr(0, 20, BOARD);
  u8g2.drawStr(0, 40, "Arduino SW");
  uint16_t vers = ARDUINO; char svers[5];
  itoa(vers, svers, 10); // convert vers to string svers
  u8g2.drawStr(80, 40, svers);
  u8g2.drawStr(0, 55, "Clock");
  uint16_t clk = F_CPU / 1000000; char sclk[5];
  itoa(clk, sclk, 10); // convert vers to string svers
  u8g2.drawStr(40, 55, sclk);
  u8g2.drawStr(60, 55, "MHz");
  u8g2.sendBuffer();

  pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop(void) 
{
  if (CONSOLE) Serial.println(F("Blink LED"));
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(950);                       // wait for a second
}
