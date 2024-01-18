/*
    note: need add library Adafruit_BMP280 from library manager
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/
#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

DHT12 dht12; 
Adafruit_BMP280 bme;

#define RedLED 10

void setup() 
{
  pinMode(RedLED, OUTPUT);    // Red LED
  digitalWrite(RedLED, HIGH);

  M5.begin();
  Wire.begin(0,26);
  M5.Lcd.setRotation(4);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.println("  M5Stick-C   ENV Hat");
  
  if (!bme.begin(0x76))
  {  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
}

void loop() 
{
  digitalWrite(RedLED, LOW);
  delay(20);
  digitalWrite(RedLED, HIGH);

  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();

  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.fillRect(0, 46, 160, 16, BLACK);
  M5.Lcd.setCursor(0, 46, 4);
  if (tmp < 10 && tmp >= 0) M5.Lcd.printf("  ");
  M5.Lcd.printf("%2.1f*C", tmp);

  M5.Lcd.fillRect(0, 74, 160, 16, BLACK);
  M5.Lcd.setCursor(0, 74, 4);
  if (hum < 10) M5.Lcd.printf("  ");
  M5.Lcd.printf("   %2.0f %%", hum);
  
  float pressure = bme.readPressure();
  M5.Lcd.fillRect(0, 104, 160, 10, BLACK);
  M5.Lcd.setCursor(0, 104, 2);
  M5.Lcd.printf("  %4.1f hPa", pressure/100);

  M5.Lcd.fillRect(0, 130, 160, 10, BLACK);
  M5.Lcd.setCursor(0,130, 1);
  float vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  if (vbat < 3.5) M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.printf("Vbat: %5.3f V   \n", vbat);
  int charge = M5.Axp.GetBatCurrent();
  M5.Lcd.printf("Ichrg: %2d mA ", charge);
  //Serial.printf("Vbat: %5.3f V  Icharge: %3d mA\r\n",vbat, charge);
  
  if (!bme.begin(0x76))
  {  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
  
  delay(5000);
}
