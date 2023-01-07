/*    
 * File M5Stack_Xiaomi.ino    
 * 
 * Read temperature, humidity and battery level from Xiaomi MJ-TH-V1 BLE sensor w/ M5Stack Core2 via BLE   
 * based on http://educ8s.tv/esp32-xiaomi-hack  
 * 
 * adapted to M5Stack Core2
 * 2021-04-30 Claus Kühnel info@ckuehnel.ch
 */

// IMPORTANT
// You need to install this library as well else it won't work
// https://github.com/fguiet/ESP32_BLE_Arduino

#include <M5Core2.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_system.h"
#include <sstream>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SCAN_TIME  10 // seconds

boolean METRIC = true; //Set true for metric system; false for imperial

BLEScan *pBLEScan;

void IRAM_ATTR resetModule()
{
    ets_printf("reboot\n");
    esp_restart();
}

       
float current_humidity = -100;
float previous_humidity = -100;
float current_temperature = -100;
float previous_temperature = -100;
float previous_batlevel = -100;
float current_batlevel = -100;

int previous_RSSIvalue = 0;
int current_RSSIvalue = 0;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks 
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveName() && advertisedDevice.haveServiceData() && !advertisedDevice.getName().compare("MJ_HT_V1")) 
    {
      int serviceDataCount = advertisedDevice.getServiceDataCount();
      std::string strServiceData = advertisedDevice.getServiceData(0);

      uint8_t cServiceData[100];
      char charServiceData[100];

      strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);

      Serial.printf("\nAdvertised Device: %s", advertisedDevice.toString().c_str());

      current_RSSIvalue = advertisedDevice.getRSSI();
        
      Serial.printf("\nRSSI: %d dBm\n", current_RSSIvalue /*advertisedDevice.getRSSI()*/ );
      displayRSSI();

      for (int i=0;i<strServiceData.length();i++) sprintf(&charServiceData[i*2], "%02x", cServiceData[i]);

      std::stringstream ss;
      ss << "fe95" << charServiceData;
            
      Serial.print("Payload:");
      Serial.println(ss.str().c_str());

      char eventLog[256];
      unsigned long value, value2;
      char charValue[5] = {0,};
      switch (cServiceData[11]) 
      {
        case 0x04:
          sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
          value = strtol(charValue, 0, 16);
          if(METRIC) current_temperature = (float)value/10;
          else       current_temperature = CelsiusToFahrenheit((float)value/10);
          displayTemperature();  
          break;
        case 0x06:
          sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
          value = strtol(charValue, 0, 16);  
          current_humidity = (float)value/10;
          displayHumidity();                      
          Serial.printf("HUMIDITY_EVENT: %s, %d\n", charValue, value);
          break;
        case 0x0A:
          sprintf(charValue, "%02X", cServiceData[14]);
          value = strtol(charValue, 0, 16);  
          current_batlevel = (float)value;
          displayBatLevel();                  
          Serial.printf("BATTERY_EVENT: %s, %d\n", charValue, value);
          break;
        case 0x0D:
          sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
          value = strtol(charValue, 0, 16);      
          if(METRIC) current_temperature = (float)value/10;
          else       current_temperature = CelsiusToFahrenheit((float)value/10);
          displayTemperature();               
          Serial.printf("TEMPERATURE_EVENT: %s, %d\n", charValue, value);                    
          sprintf(charValue, "%02X%02X", cServiceData[17], cServiceData[16]);
          value2 = strtol(charValue, 0, 16);
          current_humidity = (float)value2/10;
          displayHumidity();                                        
          Serial.printf("HUMIDITY_EVENT: %s, %d\n", charValue, value2);
          break;
        }
     }
   }
};

void setup() 
{
  M5.begin();
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(80, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("M5Stack Core2");
  M5.Lcd.setCursor(40, 18);
  M5.Lcd.printf("Xiaomi MJ TH Display");
  M5.Lcd.setTextColor(WHITE);


  Serial.println("M5Stack Core2 XIAOMI Display");
 
  initBluetooth();
}

void loop() 
{
  char printLog[256];
  Serial.printf("\nStart BLE scan for %d seconds...", SCAN_TIME);
  BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
  int count = foundDevices.getCount();
  printf("Found device count : %d\n", count);
  delay(100);
}

void initBluetooth()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(0x50);
    pBLEScan->setWindow(0x30);
}

void displayTemperature()
{
   if(current_temperature != previous_temperature)
   {
      M5.Lcd.setCursor(50, 100);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("T: ");
      M5.Lcd.print(convertFloatToString(previous_temperature));
      
      M5.Lcd.setCursor(50, 100);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("T: ");
      M5.Lcd.print(convertFloatToString(current_temperature));
      
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(225, 95);
      M5.Lcd.print("o");
      M5.Lcd.setCursor(240, 100);
      M5.Lcd.setTextSize(4);
      
      if(METRIC) M5.Lcd.print("C");
      else       M5.Lcd.print("F");
      
      previous_temperature = current_temperature;
   }
}

void displayHumidity()
{
   if(current_humidity != previous_humidity)
   {
      M5.Lcd.setCursor(50, 160);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("H: ");
      M5.Lcd.print(convertFloatToString(previous_humidity));
      
      M5.Lcd.setCursor(50, 160);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("H: ");
      M5.Lcd.print(convertFloatToString(current_humidity));
      M5.Lcd.print(" %");
      
      previous_humidity = current_humidity;
   }
}

void displayBatLevel()
{
   if(current_batlevel != previous_batlevel)
   {
      M5.Lcd.setCursor(70, 40);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("BatLvl: ");
      M5.Lcd.print(convertFloatToString(previous_batlevel));
      
      M5.Lcd.setCursor(70, 40);
      M5.Lcd.setTextColor(DARKGREEN);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("BatLvl: ");
      M5.Lcd.print(convertFloatToString(current_batlevel));
      M5.Lcd.print(" %");
      
      previous_humidity = current_humidity;
   }
}

void displayRSSI()
{
   if(current_RSSIvalue != previous_RSSIvalue)
   {
      M5.Lcd.setCursor(94, 58);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("RSSI: ");
      M5.Lcd.print(String(previous_RSSIvalue));
      
      M5.Lcd.setCursor(94, 58);
      M5.Lcd.setTextColor(DARKGREEN);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("RSSI: ");
      M5.Lcd.print(String(current_RSSIvalue));
      M5.Lcd.print(" dBm");
      
      previous_RSSIvalue = current_RSSIvalue;
   }
}

String convertFloatToString(float f)
{
  String s = String(f,1);
  return s;
}

float CelsiusToFahrenheit(float Celsius)
{
 float Fahrenheit=0;
 Fahrenheit = Celsius * 9/5 + 32;
 return Fahrenheit;
}
