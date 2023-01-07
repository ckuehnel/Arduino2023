/*
 * File: StackyPi_ESB_Weather.ino
 *
 * This is a simple weather station based on Coral Environmental Board and StackyPi
 * 
 * (c) 2022-05-09 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <SPI.h>
#include <Wire.h>
#include <sSense-HDC2010.h>  // https://itbrainpower.net/downloadables/sSense-HDC2010.zip
#include <BMx280I2C.h>  // https://bitbucket.org/christandlg/bmx280mi/src/master/
#include <ClosedCube_OPT3002.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Coral ESB Pin Definitions
#define DC 26
#define RST 22
#define CS 5
#define MOSI 3
#define CLK 2

#define BMP280_I2C_ADDRESS 0x76
#define OPT3002_ADDRESS 0x45

HDC2010 ssenseHDC2010(HDC2010_I2C_ADDR);
BMx280I2C bmx280(BMP280_I2C_ADDRESS);
ClosedCube_OPT3002 opt3002;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, MOSI, CLK, DC, RST, CS);

float HDC_temp = 0, HDC_humi = 0;
float BMP_press = 0, BMP_press64 = 0, BMP_temp = 0;
float lux = 0;
uint8_t idx = 0;

// Function prototypes
void initGPIO(void);
void initHDC2010(void);
void initBMP280(void);
void initOPT3002(void);
void initOLED(void);


void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  Serial.printf("done\n");

  initHDC2010();
  initBMP280();
  initOPT3002();
  initOLED();

  Serial.printf("\nSimple weather station w/ Coral Environmental Board and StackyPi\n");
}



void loop()
{
  HDC_temp = ssenseHDC2010.readTemp();
  HDC_humi = ssenseHDC2010.readHumidity();

  Serial.printf("HDC2010\tTemp = %3.1f °C\tHum = %3.0f %%rH\n", HDC_temp, HDC_humi);

  readBMP280();
  Serial.printf("BMP280\tTemp = %3.1f °C\tPress = %6.1f hPa\n", BMP_temp, BMP_press64);

  OPT3002 result = opt3002.readResult();
  if (result.error == NO_ERROR) 
  { 
    lux = result.lux;
    Serial.printf("OPT3002\tLight = %6.1f nW/cm2\n", lux);
  }
  Serial.printf("\n");
  display.clearDisplay();
  display.setCursor(0, 16); 
  
  switch(idx)
  {
    case 1: 
      display.println(String(HDC_temp, 1) + " *C");
      break;
    case 2:
      display.println(String(HDC_humi, 0) + " %rH");
      break;
    case 3:
      display.println(String(BMP_press64, 0) + " hPa");
      break;
    case 4:
      display.println(String(lux/1000, 1) + " mW/cm2");
      break;
    default:
      display.println("CORAL ESB");
      break;
  }
  display.display();
  idx++; if (idx == 5) idx = 0;
  // Serial.println(idx);
  
  delay(2000);
}
