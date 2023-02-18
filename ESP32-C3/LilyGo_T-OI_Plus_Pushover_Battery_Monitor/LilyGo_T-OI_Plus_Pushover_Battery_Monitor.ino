 /* File: LilyGo_T-OI_Plus_Pushover_Battery_Monitor.ino
 *  
 * based on https://github.com/Xinyuan-LilyGO/LilyGo-T-OI-PLUS</blob/main/example/deepsleep/deepsleep.ino
 * Author: Pranav Cherukupalli <cherukupallip@gmail.com>
 * 
 * Visualization of battery voltage by Pushover
 * 
 * 2023-02-16/Claus Kuehnel info@ckuehnel.ch
 */
#include "Arduino.h"
#include <WiFi.h>
#include "esp_adc_cal.h"
#include "arduino_secrets.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 8;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 199);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Google DNS
IPAddress secondaryDNS(8, 8, 4, 4); // Google DNS

WiFiClient client;

#define DEVICE "VOLTAIC"

#define BAT_ADC   2  // Voltage divider for ADC Input A0 on LilyGo_T-OI_Plus
#define DEBUG     0  // 0 - disable serial communication

float Voltage = 0.0;
uint32_t readADC_Cal(int ADC_Raw);
int current_rssiValue;

#define uS_TO_S_FACTOR 1000000ULL   /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3600         /* Time ESP32 will go to sleep (in seconds) */

void setup()
{
  if (DEBUG) Serial.begin(115200);
  delay(1000); // Wait for Serial Monitor
  if (DEBUG) Serial.println("Starting LilyGo_T-OI_Plus_Voltaic_Battery_Monitor...");

  sensors.begin(); // Start the DS18B20 sensor

  if (DEBUG) 
  {
    print_wakeup_reason(); //Print the wakeup reason for ESP32
    print_adc_characteristics();
  }

  Voltage = (readADC_Cal(analogRead(BAT_ADC))) * 2;
  if (DEBUG) Serial.printf("Vbat = %.0f mV\n", Voltage);      // Print Voltage (in mV)

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  if (DEBUG) Serial.printf("Temp = %.1f ºC\n", temperatureC); // Print Temperature (in °C)

  initWiFi();

  current_rssiValue = WiFi.RSSI();
  if (DEBUG) Serial.printf("RSSI = %d dBm\n", current_rssiValue); 

  if (WiFi.status() != WL_CONNECTED) 
  {
    if (DEBUG) Serial.println("Connection lost - Reconnecting to WiFi...");
    ESP.restart();
  }
  if (DEBUG) Serial.println("Prepare message and send...");
  String message  = "Temperature = ";
         message += String(temperatureC, 1);
         message += " °C\n";
         message += "SuperCap Voltage = ";
         message += String(Voltage, 0);
         message += " mV\n";
         message += "RSSI = ";
         message += String(current_rssiValue);
         message += " dBm";

  int len = message.length();
  char charBuf[len+1];
  message.toCharArray(charBuf, len+1);
  if (DEBUG) Serial.println(charBuf);
  boolean ret = pushover(charBuf,0); // Priority 0
  if (DEBUG) Serial.println(ret);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  if (DEBUG) Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  /*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  /*
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  */
  if (DEBUG) Serial.println("Going to sleep...");
  if (DEBUG) Serial.flush();
  esp_deep_sleep_start();
  if (DEBUG) Serial.println("This will never be printed");
}

void loop()
{
  //This is not going to be called
}
