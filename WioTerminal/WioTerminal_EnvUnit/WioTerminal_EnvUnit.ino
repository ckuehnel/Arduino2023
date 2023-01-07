/*
   WioTerminal_EnvUnit.ino

   Measuring temperature by ENV Unit 
   
   createtd 2020-06-20 by Claus Kühnel (info@ckuehnel.ch) 
 */
 
#include "TFT_eSPI.h"
#include "Free_Fonts.h"

#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

TFT_eSPI tft;
DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
Adafruit_BMP280 bme;

#define BAUD 115200

#define INFO 1
#define DEBUG 1

float temp, hum, pressure;

void setup() 
{  
  Serial.begin(BAUD);
  delay(2000);  // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("\nInitializing..."));
  tft.begin();
  tft.setRotation(3);
  tft.setFreeFont(FSS12);
  tft.fillScreen(TFT_NAVY); //NAVY background
  tft.setTextColor(TFT_WHITE);          //sets the text colour to black
//  tft.setTextSize(3);                   //sets the size of text
  tft.drawString("Initialize...", 10, 20); //prints strings from (0, 0)
  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
//    tft.drawString("Could not find a valid BMP280 sensor, check wiring!");
    while(1);
  }
  tft.fillScreen(TFT_NAVY); //NAVY background
  tft.drawString("Read ENV Unit...", 10, 20);
  tft.setFreeFont(FSS24);
  delay(500);
  if (DEBUG) Serial.println(F("Running..."));     //last line in setup()
}

void loop()
{
  temp = dht12.readTemperature();
  hum = dht12.readHumidity();
  pressure = bme.readPressure()/100;
  Serial.printf("Temperature: %2.1f *C  Humidity: %2.0f %%  Pressure: %5.0f hPa\r\n", temp, hum, pressure);
  tft.fillScreen(TFT_NAVY); //NAVY background
  String sValue = String(temp,1); sValue += " *C";
  tft.drawString(sValue, 80, 50);
  sValue = String(hum,0); sValue += " % rH";
  tft.drawString(sValue, 80, 100);
  sValue = String(pressure,0); sValue += " hPa";
  tft.drawString(sValue, 80, 150);
  delay(1 * 30000);           // 30 seconds
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
