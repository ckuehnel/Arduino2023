/*
 * File: LoRaWAN_ENVII_Template.ino
 * 
 * This file gives a frame for sending ensor data via LoRaWAN
 * via WisBlock Core RAK11310 (RP2040 & SX1262) or RAK4631 (nRF52840 & SX1262)
 * and RAK1920 WisBlock Sensor Adapter Module w/ Grove, QwiiC & mikroE interfaces
 * 2022-03-01 Claus Kühnel info@ckuehnel.ch
 */
#include <Arduino.h>

#define DEBUG 1   // set to 0 to avoid display of measuring results on serial monitor

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

void setup() 
{
  time_t timeout = millis();  
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000) 
      delay(100);
    else break;
  } 

  Serial.println("\nM5Stack ENV.II Sensor Test"); // Change this if you want
  Serial.println("SHT31 @ 0x44 & BMP280 @ 0x76");
  
  initSensor(); // initialize ENV.II sensor
}

void loop() 
{
  getValues(); // get measuring results of ENV.II sensor

  // prepare the measuring results for use in payload
  int16_t temp = (int16_t) (sht31Temperature * 100. + .5);
  uint8_t tempHi = highByte(temp);
  uint8_t tempLo = lowByte(temp);

  //Serial.print(temp); Serial.print("\t");
  //Serial.print(temp,HEX); Serial.print("\t"); Serial.print(tempHi,HEX); Serial.print(tempLo,HEX);
  Serial.println();

  // prepare LoRaWAN message and send it to TTN
  
  delay(5000);
}
