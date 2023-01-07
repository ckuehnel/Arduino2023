/*
 * File: WioTerminal_BME680.ino
 * 
 * Measuring Indoor Air Quality Index by BME680 Sensor from Bosch 
 * 
 * createtd 2021-02-21 by Claus Kühnel (info@ckuehnel.ch) 
 */
 
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include <Wire.h> // BME680 uses I2C comunication.

TFT_eSPI tft;

#define BAUD 115200

#define INFO 1
#define DEBUG 1

float temp, hum, pressure, eCO2, bVOC;
int iAQ, Acc;

void displayData(void)
{
  tft.fillScreen(TFT_NAVY); //NAVY background
  String sValue = "T: " + String(temp,1) + " *C";
  tft.drawString(sValue, 160, 50);
  sValue = "H: "+ String(hum, 0) + " % rH";
  tft.drawString(sValue, 160, 90);
  if (Acc > 0) sValue = "iAQ: " + String(iAQ);
  else  sValue = " * iAQ: " + String(iAQ) + " *";
  tft.drawString(sValue, 160, 140); 
  sValue = "eCO2: " + String(eCO2, 0) + " ppm";
  tft.drawString(sValue, 160, 180);
  sValue = "bVOC: " + String(bVOC) + " ppm";
  tft.drawString(sValue, 160, 220);
}

void setup() 
{  
  Serial.begin(BAUD);
  while(!Serial);
  //delay(2000);  // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("\nInitializing..."));
  tft.begin();
  tft.setRotation(3);
  tft.setFreeFont(FSS12);
  tft.fillScreen(TFT_NAVY); //NAVY background
  tft.setTextColor(TFT_WHITE);          //sets the text colour to black
//  tft.setTextSize(3);                   //sets the size of text
  tft.drawString("Initialize...", 10, 20); //prints strings from (0, 0)
  tft.fillScreen(TFT_NAVY); //NAVY background
  tft.drawString("Read BME680 Sensor...", 10, 20);
  tft.setFreeFont(FSS18);
  tft.setTextDatum(BC_DATUM);
  initBME680();
  delay(500);
  if (DEBUG) Serial.println(F("Running..."));     //last line in setup()
}

void loop()
{
  readBME680();
  //delay(1 * 15000);           // 15 seconds
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;
  
  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
