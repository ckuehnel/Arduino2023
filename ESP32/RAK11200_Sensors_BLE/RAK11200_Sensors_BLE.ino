/*
 * File: RAK11200_Sensors_BLE.ino
 * 
 * Environmwental Sensor based on
 * - RAK19007 WisBlock Base Board 2nd Gen
 * - RAK11200 is a WisBlock Core module based on Espressif ESP32-WROVER
 * - RAK1901 WisBlock Sensor based on Sensirion SHTC3 temperature and humidity sensor
 * - RAK12047 WisBlock VOC Sensor based on MOx-based Sensirion Gas Sensor SGP40
 * - RAK14001 WisBlock RGB LED Module
 * send BLE Advertising for Sensirion App MyAmbience on Android
 * 
 * 2022-11-19 Claus Kuehnel info@ckuehnel.ch
 */

#include <Wire.h>
#include "SHTSensor.h"                      // http://librarymanager/All#arduino-sht By:Johannes Winkelmann
#include "SparkFun_SGP40_Arduino_Library.h" // http://librarymanager/All#SparkFun_SGP40
#include <NCP5623.h>                        // http://librarymanager/All#NCP5623 By:RAKWireless
#include <WiFi.h>    
#include "esp_adc_cal.h"
#include "Sensirion_Gadget_BLE.h"
// NimBLE-Arduino library must be installed too

SHTSensor sht;
SGP40 mySGP40; 
NCP5623 rgb;
NimBLELibraryWrapper lib;
//DataProvider provider(lib, DataType::T_RH_CO2_ALT);
DataProvider provider(lib, DataType::T_RH_VOC);

boolean RAK1901_ready = false;  // true, if RAK1901 was detected
boolean RAK12047_ready = false;  // true, if RAK1901 was detected
boolean RAK14001_ready = false;  // true, if RAK14001 was detected

float temp, humi;   // Temp & Humi of SHTC3 (RAK1901)
int32_t iVOC;       // VOC Index of SGP40 (RAK12047)
uint16_t batteryLevel = 100;

#define PIN_VBAT WB_A0
uint32_t vbat_pin = PIN_VBAT;

float Voltage = 0.0;
uint32_t readADC_Cal(int ADC_Raw);

static int64_t lastMeasurementTimeMs = 0;
static int measurementIntervalMs = 1000;

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

  // Initialize the GadgetBle Library
  provider.begin();
  Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
  Serial.println(provider.getDeviceIdString());

  // Set initial battery level
  provider.setBatteryLevel(batteryLevel);
}

void loop() 
{
  if (millis() - lastMeasurementTimeMs >= measurementIntervalMs)
  {
    if (RAK1901_ready)  readRAK1901();
    if (RAK12047_ready) readRAK12047();
    if (RAK14001_ready) setLED();
    Voltage = (readADC_Cal(analogRead(PIN_VBAT))) * 2;
    Serial.printf("Battery : Vbat = %.0f mV\n", Voltage); // Print Voltage (in mV)
    
    // getBatteryLevel toDo yet
    provider.setBatteryLevel(batteryLevel);
    Serial.printf("Battery Level: %d %%\n",batteryLevel);   // from Sensirion library

    provider.writeValueToCurrentSample(temp, Unit::T);
    provider.writeValueToCurrentSample(humi, Unit::RH);
    provider.writeValueToCurrentSample(iVOC, Unit::VOC);
    
    provider.commitSample();
    lastMeasurementTimeMs = millis();
  }
  provider.handleDownload();
  delay(3);
}
