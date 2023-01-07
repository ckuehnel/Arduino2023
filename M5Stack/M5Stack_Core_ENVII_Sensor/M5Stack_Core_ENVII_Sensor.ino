/*
 * File: M5Stack_Core_ENVII_Sensor.ino
 * 
 * Please install the < Adafruit BMP280 Library > （https://github.com/adafruit/Adafruit_BMP280_Library）
 * and Adafruit Unified Sensor Library and SHT3X Library
 * from the library manager before use. 
 * This code will display the temperature, humidity and air pressure information on the screen
 */

#include <M5Stack.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include <WEMOS_SHT3X.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch

SHT3X sht30(0x44);
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() 
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setFreeFont(FSS12);
  M5.Lcd.setBrightness(50);
  M5.Lcd.setTextSize(1);
  Serial.println(F("ENVII Unit(SHT30 and BMP280) test..."));

  while (!bme.begin(0x76))
  {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    //M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  
  M5.Lcd.println("\nTest of ENVII Unit...");
}

void loop() 
{
  pressure = bme.readPressure();
  if(sht30.get()==0)
  {
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("Temperature: %2.1f *C  Humidity: %2.0f %%rH  Pressure: %2.1f hPa\r\n", tmp, hum, pressure/100);
  //M5.Lcd.fillRect(0, 80, 320, 110, BLACK);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.fillRect(0, 80, 320, 25, BLACK);
  M5.Lcd.printf("Temperature %2.1f *C\r\n", tmp);
  M5.Lcd.fillRect(0, 110, 320, 25, BLACK);
  M5.Lcd.printf("Humidity        %2.0f %% rH\r\n", hum);
  M5.Lcd.fillRect(0, 140, 320, 25, BLACK);
  M5.Lcd.printf("Pressure       %2.1f hPa", pressure/100);
  delay(2500);
}
