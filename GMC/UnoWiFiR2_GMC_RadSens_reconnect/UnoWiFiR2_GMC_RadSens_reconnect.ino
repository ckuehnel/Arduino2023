/*
 * File: UnoWiFiR2_GMC_RadSens_reconnect.ino
 * 
 * Program for Geiger-Müller-Counter RaSens collects GMC pulses and calculates CPM and uSv/h
 * to send them to Thingspeak for visualization afterwards.
 *  
 * Board used is ESP32duino.
 * GMC interface is I2C. 
 * 
 * 2022-05-17 Claus Kühnel info@ckuehnel.ch
 */
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include <RadSensBoard.h>  // https://github.com/vurdalakov/radsensboard
#include "ThingSpeak.h"    // always include thingspeak header file after other header files and custom macros

RadSensBoard radSensBoard;

uint16_t oldCount = 0, newCount;
int rate;
float dose;

unsigned long previous_time = 0, last_measure = 0;
unsigned long time1, time2=0;
const unsigned long DELAY = 30000;  // 30 seconds delay
const unsigned long CYCLE = 60000;  // measuring cycle

int status = WL_IDLE_STATUS;                     // the Wifi radio's status

WiFiClient client;

void initWiFi(void);

void setup() 
{
  Serial.begin(9600);
  delay(2000);  // Wait for Serial Monitor
  Serial.println("\n\nReading RadSense GMC connected to Arduino Uno WiFi R2 by I2C");
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
  while (!ScanSSIDs()) initWiFi();
  last_measure = millis();
  oldCount = radSensBoard.getPulseCount();
} 
  
void loop() 
{
  time1 = millis();
  if ((time1 - time2) > 15000) //every 15 s
  {
    time2 = time1;    
    TestWiFiConnection(); //test connection, and reconnect if necessary
    long rssi=WiFi.RSSI();
    Serial.print("RSSI:"); Serial.print(rssi); Serial.println(" dBi");
  }

  unsigned long current_time = millis(); // number of milliseconds since the upload

  // checking GMC data
  if ((current_time - last_measure >= CYCLE))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    if (radSensBoard.readData())
    {
      dose = radSensBoard.getRadiationLevelDynamic();
      Serial.print("\nRadiation level (dynamic window): ");
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
  
    Serial.print("\nCount rate = "); Serial.print(rate); Serial.print(" CPM");
    Serial.print("\t\tDosis = "); Serial.print(dose * 10, 3); Serial.println(" uSv/h");
    
    dose *= 10.0;
    
    // set the fields with the values
    ThingSpeak.setField(1, rate);
    ThingSpeak.setField(2, dose);

    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200) Serial.println("Channel update successful.");
    else         Serial.println("Problem updating channel. HTTP error code " + String(x));

    connectServer();
    
    digitalWrite(LED_BUILTIN, LOW);
    last_measure = millis();   
  }
}
