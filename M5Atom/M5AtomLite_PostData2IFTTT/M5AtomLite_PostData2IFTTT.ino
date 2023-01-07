/*
 * File: M5AtomLite_PostData2IFTTT.ino
 * 
 * Post data of evironmental sensors to Google Sheets for later evaluation & visualization
 * based on https://RandomNerdTutorials.com/esp32-http-post-ifttt-thingspeak-arduino/ by Rui Santos
 * 
 * 2021-01-30 Claus Kühnel info@ckuehnel.ch
 */
#include <Arduino.h>
#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "arduino_secrets.h"

// set not used sensors to 0
#define SGP30_Sensor 1
#define SCD30_Sensor 1
#define SCD41_Sensor 0
#define K30_Sensor 0
#define BME680_Sensor 0 // will give an error when BSEC not supported

#define DEBUG 1

#include <Adafruit_SGP30.h>
Adafruit_SGP30 sgp;

#include <SparkFun_SCD30_Arduino_Library.h>
SCD30 scd;

#include <SensirionI2CScd4x.h>
SensirionI2CScd4x scd4x;

#include <K30_I2C.h>
K30_I2C k30_i2c = K30_I2C(0x68);

#include <bsec.h>
Bsec iaqSensor;

// Function prototypes
void initSGP30(void);  void readSGP30(void);
void initSCD30(void);  void readSCD30(void);
void initSCD41(void);  void readSCD41(void);
void initK30(void);    void readK30(void);
void initBME680(void); void readBME680(void);
void initWiFi(void);

float temperature, humidity, BME680eCO2;
uint16_t TVOC, eCO2, CO2;
int K30CO2, BME680iAQ;
uint16_t SCD41CO2;

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned

// Set timer to 1 minutes (60000)
unsigned long timerDelay = 60000;
// Timer set to 10 seconds (10000)
//unsigned long timerDelay = 10000;

unsigned long lastTime = millis() + timerDelay + 10;

void setup() 
{
  M5.begin(true, true, true); // Serial, I2C, Display
  delay(50);
  Serial.println("\nM5Atom Lite posts data to IFTTT...");
  Wire.begin(26,32);  // Grove port @ M5Atom

  if (SGP30_Sensor) initSGP30();
  if (SCD30_Sensor) initSCD30();
  if (SCD41_Sensor) initSCD41();
  if (K30_Sensor) initK30(); 
  if (BME680_Sensor) initBME680();

  Serial.println("Initial State Blue");
  M5.dis.drawpix(0, 0x0000f0);
  initWiFi();
}

void loop() 
{
  //Send an HTTP POST request every 60 seconds
  if ((millis() - lastTime) > timerDelay) 
  { 
    // Read TVOC & eCO2 from SGP30 sensor
    if (SGP30_Sensor) readSGP30();
    
    // Read Temperature, Humidity & CO2 from SCD30 snsor
    if (SCD30_Sensor) readSCD30();
    
    // Read CO2 from K30 sensor
    if (K30_Sensor) readK30();

    // Read CO2 from SCD41 sensor
    if (SCD41_Sensor) readSCD41();

    // Read data from BME680
     if (BME680_Sensor) readBME680();

    int wifi_retry = 0;
    
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      // Prepare data string for the columns saved in Google sheet
      String httpRequestData = "value1=" + String(eCO2) + "&value2=" + String(CO2)+ "&value3=" + String(BME680iAQ);           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);   
      Serial.print("\nHTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println();
      if (httpResponseCode == 200)
      {
        M5.dis.drawpix(0, 0xf00000);
        delay(1000);
        M5.dis.drawpix(0, 0x0000f0);
      }
      if (httpResponseCode < 100)
      {
        Serial.println("\nReboot");
        ESP.restart();
      }
        
      // Free resources
      http.end();
    }
    else // Reconnecting WiFi
    {
      if (wifi_retry < 5 ) 
      {
        wifi_retry++;
        Serial.println("WiFi not connected. Try to reconnect");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        delay(100);
      }
      if(wifi_retry >= 5) 
      {
        Serial.println("\nReboot");
        ESP.restart();
      }
    }
    lastTime = millis(); 
  }
}
