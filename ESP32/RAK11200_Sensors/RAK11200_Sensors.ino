/*
 * File: RAK11200_Sensors.ino
 * 
 * Environmwental Sensor based on
 * - RAK19007 WisBlock Base Board 2nd Gen
 * - RAK11200 is a WisBlock Core module based on Espressif ESP32-WROVER
 * - RAK1901 WisBlock Sensor based on Sensirion SHTC3 temperature and humidity sensor
 * - RAK12047 WisBlock VOC Sensor based on MOx-based Sensirion Gas Sensor SGP40
 * - RAK14001 WisBlock RGB LED Module
 * send WA message if ventilation is required.
 * 
 * 2022-11-16 Claus Kuehnel info@ckuehnel.ch
 */

#include <Wire.h>
#include "SHTSensor.h"                      // http://librarymanager/All#arduino-sht By:Johannes Winkelmann
#include "SparkFun_SGP40_Arduino_Library.h" // http://librarymanager/All#SparkFun_SGP40
#include <NCP5623.h>                        // http://librarymanager/All#NCP5623 By:RAKWireless
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>                      // http://librarymanager/All#URLEncode
#include "esp_adc_cal.h"
#include "arduino_secrets.h"

SHTSensor sht;
SGP40 mySGP40; 
NCP5623 rgb;

const uint16_t PERIOD = 5000;
boolean ventilation = false;
boolean RAK1901_ready = false;  // true, if RAK1901 was detected
boolean RAK12047_ready = false;  // true, if RAK1901 was detected
boolean RAK14001_ready = false;  // true, if RAK14001 was detected

float temp, humi;   // Temp & Humi of SHTC3 (RAK1901)
int32_t iVOC;       // VOC Index of SGP40 (RAK12047)

#define PIN_VBAT WB_A0
uint32_t vbat_pin = PIN_VBAT;

float Voltage = 0.0;
uint32_t readADC_Cal(int ADC_Raw);

void setup() 
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH); // set 3V3_S

  pinMode(WB_IO6, OUTPUT);
  digitalWrite(WB_IO6, HIGH); // enable RAK14001
  
  Wire.begin();
  Serial.begin(115200);
  delay(1000); // wait for serial monitor
  Serial.println("RAKwireless Sensor Node");
  
  if (initRAK1901()) Serial.println("RAK1901 connected");
  if (initRAK12047()) Serial.println("RAK12047 connected");
  if (initRAK14001(15)) Serial.println("RAK14001 connected"); // ILED = 15 mA

  print_adc_characteristics();

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Send Message to WhatsApp
  sendMessage("RAK AirQuality Monitoring started...");
}

void loop() 
{
  if (RAK1901_ready)  readRAK1901();
  if (RAK12047_ready) readRAK12047();
  if (RAK14001_ready) setLED();
  Voltage = (readADC_Cal(analogRead(PIN_VBAT))) * 2;
  Serial.printf("Battery : Vbat = %.0f mV\n", Voltage); // Print Voltage (in mV)

  // https://sensirion.com/media/documents/02232963/6294E043/Info_Note_VOC_Index.pdf
  if ((iVOC > 200) && (!ventilation))
  {
    ventilation = true;
    sendMessage("iVOC too high - ventilation required");
  }
  if ((iVOC < 100) && (ventilation))
  {
    ventilation = false;
    sendMessage("iVOC in good condition - no ventilation required");
  }
  delay(PERIOD);
}
