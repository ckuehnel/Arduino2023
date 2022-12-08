/*
 * File: ESP32_GMC_RadSens.ino
 * 
 * Program for Geiger-Müller-Counter RaSens collects GMC pulses and calculates CPM and uSv/h
 * to send them to Thingspeak for visualization afterwards.
 *  
 * Board used is ESP32duino.
 * GMC interface is I2C. 
 * 
 * 2022-05-17 Claus Kühnel info@ckuehnel.ch
 */
#include <WiFi.h>
#include "arduino_secrets.h"
#include <RadSensBoard.h>  // https://github.com/vurdalakov/radsensboard
#include "ThingSpeak.h"    // always include thingspeak header file after other header files and custom macros

RadSensBoard radSensBoard;

const uint16_t LED_BUILTIN = 2;

uint16_t oldCount = 0, newCount, rate;
float dose;

unsigned long previous_time = 0, last_measure = 0;
const unsigned long DELAY = 30000;  // 30 seconds delay
const unsigned long CYCLE = 60000;  // measuring cycle

WiFiClient client;

void initWiFi(void);

void setup() 
{
  Serial.begin(115200);
  delay(2000);  // Wait for Serial Monitor
  Serial.printf("\n\nReading RadSense GMC connected to ESP32 by I2C\n");
  if (!radSensBoard.init()) Serial.println("Device not found");
  Serial.print("Firmware version: ");
  Serial.println(radSensBoard.getFirmwareVersion());
  Serial.print("Calibration value: ");
  Serial.println(radSensBoard.getCalibrationValue());
  Serial.print("High-voltage generator: ");
  Serial.println(radSensBoard.getHighVoltageGeneratorState());
  Serial.print("LED Indication state: ");
  Serial.println(radSensBoard.getLedIndicationState());
  initWiFi();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, LOW);
  last_measure = millis();
  oldCount = radSensBoard.getPulseCount();
} 
  
void loop() 
{
  unsigned long current_time = millis(); // number of milliseconds since the upload

  // checking for WIFI connection
  if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= DELAY)) 
  {
    Serial.print(millis());
    Serial.println(" - Reconnecting to WIFI network");
    WiFi.disconnect();
    WiFi.reconnect();
    previous_time = current_time;
  }
  // checking GMC data
  if ((current_time - last_measure >= CYCLE))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    if (radSensBoard.readData())
    {
      dose = radSensBoard.getRadiationLevelDynamic();
      Serial.print("Radiation level (dynamic window): ");
      Serial.print(dose, 3);
      Serial.println(" mR/h");

      Serial.print("Radiation level (static window):  ");
      Serial.print(radSensBoard.getRadiationLevelStatic(), 3);
      Serial.println(" mR/h");

      newCount = radSensBoard.getPulseCount();
      rate = newCount - oldCount; oldCount = newCount;

      Serial.print("Pulse count:                      ");
      Serial.println(rate);
    }
    else Serial.println("Error reading data from the board");
  
    Serial.printf("\nCount rate = %d CPM", rate);
    Serial.printf("\t\tDosis = %5.3f uSv/h\n", dose * 10);

    // set the fields with the values
    ThingSpeak.setField(1, rate);
    ThingSpeak.setField(2, dose * 10);

    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200) Serial.println("Channel update successful.");
    else         Serial.println("Problem updating channel. HTTP error code " + String(x));
    digitalWrite(LED_BUILTIN, LOW);
    last_measure = millis();   
  }
}
