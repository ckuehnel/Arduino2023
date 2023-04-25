#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <esp_adc_cal.h>

//WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wm;

#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34
#define BUTTON_1            35
#define BUTTON_2            0

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

char buff[512];
int vref = 1100;

void setup() 
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0, 2);
  tft.setTextDatum(MC_DATUM);

  Serial.begin(115200);
  Serial.println("Start T-Display...");

  tft.println("Connect to AP");
  tft.println("by 192.168.4.1");
  tft.println("to configure");
  tft.println("Internet access.");

/*
  ADC_EN is the ADC detection enable port
  If the USB port is used for power supply, it is turned on by default.
  If it is powered by battery, it needs to be set to high level
 */
  pinMode(ADC_EN, OUTPUT);
  digitalWrite(ADC_EN, HIGH);

  bool res = wm.autoConnect("AutoConnectAP","cksz"); // password protected ap

  if(!res) 
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  } 
  else 
  {
    Serial.println("...connected to Internet now.");
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(2);
    tft.println("T-Display");
    tft.setTextSize(1);
    tft.println("with ESP32");
    tft.println("connected to");
    tft.println("Internet.");
  }

}

void loop() 
{
  Serial.print("Local IP : ");
  Serial.println(WiFi.localIP());  
  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI()); Serial.println(" dBm"); 
  
  tft.fillRect(0, 100, 135, 64, TFT_BLACK);
  tft.setCursor(0, 100, 2);
  tft.println("Local IP :");
  tft.println(WiFi.localIP());
  tft.print("RSSI : ");
  tft.print(WiFi.RSSI());
  tft.println(" dBm");
  uint16_t v = analogRead(ADC_PIN);
  float battery_voltage = ((float)v / 4096.0 * 2.0 * 3.3 * vref);
  String voltage = "Vbat : " + String(battery_voltage) + " mV";
  Serial.println(voltage);
  tft.println(voltage);

  delay(5000);

}