/*
 * File: Nano_Every_Environmental.ino
 * 
 * Measuring of environmental data by M5Stack ENV II and SGP30 Units
 * connected to Arduino Nano Every by Grove Shield for Arduino Nano & 
 * M5Stack Grove Hub
 * 
 * 2021-01-26 Claus Kühnel info@ckuehnel.ch
 */
#include <U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Adafruit_SHT31.h>
#include <Adafruit_SGP30.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG 0  // set 1 for serial monitor

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
 
Adafruit_SHT31 sht31 = Adafruit_SHT31(); // works for SHT30 too
Adafruit_SGP30 sgp;

#define PIN        6  
#define NUMPIXELS  1 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

float temp = 0.0;
float humi = 0.0;
uint16_t TVOC, eCO2;

void setup()
{
  if (DEBUG)
  {
    Serial.begin(115200);
    while (!Serial);        // wait for serial port to connect. Needed for native USB.
    Serial.println("\nMeasuring environmental data by M5Stack ENVII & SGP30 Units");

  }

  u8g2.begin();
  u8g2.clearBuffer();            // clear the internal memory
  u8g2.setFont(u8g2_font_7x13_tf); // choose a suitable font
  u8g2.drawStr(0,10, "Room Cond. Monitor");
  initENVII();
  initSGP30();
  u8g2.sendBuffer();
  pixels.begin(); pixels.clear();
  delay(1000);
}

void loop()
{
  getENVIIData();
  getSGP30Data();
  displayData();
  showLED();
  delay(5000);
}
