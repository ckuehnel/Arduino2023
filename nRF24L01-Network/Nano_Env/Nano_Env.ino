/*
 * File Nano_Env.ino
 * 
 * M5Stack ENV Unit connected to Arduino Nano via Grove I2C Connector
 * 
 * Claus Kühnel 2020-06-03 info@ckuehnel.ch
 */

#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

#define INFO 1
#define DEBUG 1
#define BAUDRATE 115200

DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
Adafruit_BMP280 bme;

void setup() 
{
  //pinMode(RedLED, OUTPUT);    // Red LED
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(BAUDRATE);
  delay(2000);              // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("Initializing..."));

  Serial.println("ENV Unit test...");
  
  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
  
  Wire.begin();
  if (!bme.begin(0x76))
  {  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }  
}

void loop() 
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  
  float tmp = dht12.readTemperature();
  //float tmp = random(-40.0, 60.0); // for simulation 
  float hum = dht12.readHumidity();
  float pressure = bme.readPressure();
  Serial.print("Temperature: \t"); Serial.print(tmp, 1); Serial.println(" *C");
  Serial.print("Humidity: \t"); Serial.print(hum, 0); Serial.println(" %RH");
  Serial.print("Pressure: \t"); Serial.print(pressure, 0); Serial.println(" Pa\n");

  delay(2500);
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;

  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
