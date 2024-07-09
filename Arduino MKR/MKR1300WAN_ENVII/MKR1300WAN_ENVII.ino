/*
 * File: MKR1300WAN_ENVII.ino
 * 
 * Sending data sampled by ENV.II Unit w/ The Things Uno board to TTN
 * 
 * 2021-06-26 Claus Kühnel info@ckuehnel.ch
 */

#include <MKRWAN.h>
#include "arduino_secrets.h" 

#define DEBUG 1   // set to 0 to avoid display of measuring results on serial monitor

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

const long CYCLE = 15 * 60000; // Transmission cycle 15 minutes
unsigned long previousMillis = 0;

// Select your region (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = EU868;

LoRaModem modem(Serial1);

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
 
  if (!modem.begin(region)) 
  {
    Serial.println("Failed to start module");
    while (1) {}
  };

  Serial.println("\nSensornode w/ Arduino MKR 1300 WAN & M5Stack ENV.II Sensor"); // Change this if you want
  Serial.println("SHT31 @ 0x44 & BMP280 @ 0x76");
  Serial.println("Initialize sensors...");
  initSensor(); // initialize ENV.II sensor
  Serial.println("Initialization done.");
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  Serial.println("Successfully joined the network!");
  Serial.println("Enabling ADR and setting low spreading factor");
  modem.setADR(true);  // ADR true
  modem.dataRate(5);   // DR5 means SF7
}

void loop() 
{
  getValues(); // get measuring results of ENV.II sensor

  // prepare the measuring results for use in payload
  // you can enhance payload w/ BMP280 data, if you want
  int16_t temp = (int16_t) (sht31Temperature * 100. + .5);
  int16_t humi = (int16_t) (sht31Humidity * 10. + .5);

  String payload = String(temp) + String(humi);

  digitalWrite(LED_BUILTIN, HIGH);
  
  modem.beginPacket();
  modem.print(payload);
  
  int err = modem.endPacket(false);

  if (err > 0) Serial.println("Transmission ok");
  else         Serial.println("Transmission Error");

  digitalWrite(LED_BUILTIN, LOW);

  previousMillis = millis();
  while(millis() - previousMillis < CYCLE);  // wait CYCLE milliseconds
}
