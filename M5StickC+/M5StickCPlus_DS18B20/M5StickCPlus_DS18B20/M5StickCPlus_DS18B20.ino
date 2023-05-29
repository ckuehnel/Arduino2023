/*
   M5StickCPlus_DS18B20.ino
   
   Temperature Measurement w/ DS18B20 sensor via header on top
   Used Board:   M5Stick-C-Plus
   Upload Speed: 115200
       
   Based on M5StickC_DS18B20, created 2019-05-20 Claus Kühnel info@ckuehnel.ch
*/
#include <M5StickCPlus.h>
#include <Wire.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define INFO 0
#define DEBUG 1

#define RedLED 10

// Data wire is plugged into pin G26 on the ESP32
#define ONE_WIRE_BUS 26
 
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// array to hold device address
DeviceAddress Thermometer;

WiFiClient client;

unsigned long previousMillis = 0;     // will store last time
const unsigned long interval = 5000;  // interval in milliseconds
float tmp, vbat = 0.0;
int length;

void setup() 
{
  M5.begin();
  delay(2000);              // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("Initializing..."));

  pinMode(RedLED, OUTPUT);    // Red LED
  digitalWrite(RedLED, HIGH);

  M5.Beep.setVolume(0);  
  
  sensors.begin();

  if (DEBUG)
  {
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");

    // report parasite power requirements
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
    if (!sensors.getAddress(Thermometer, 0)) Serial.println("Unable to find address for Device 0");

    // show the addresses we found on the bus
    Serial.print("Device 0 Address: ");
    printAddress(Thermometer);
    Serial.println();

    // set the resolution to 11 bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(Thermometer, 11);
 
    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(Thermometer), DEC); 
    Serial.println();
  }

  M5.Lcd.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(GREEN);  
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen( BLACK );
   
  delay(3000);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("DS18B20 Temperature");
 
  Serial.println(F("Temperature Measurement..."));

  Wire.begin(); // internal I2C Bus

  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
  
    digitalWrite(RedLED, LOW);
    delay(20);
    digitalWrite(RedLED, HIGH);
  
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print(" DONE\t\t");
  
    //float tmp = sensors.getTempCByIndex(0); // Why "byIndex"?
    // You can have more than one IC on the same bus.
    // 0 refers to the first IC on the wire
    tmp = sensors.getTempC(Thermometer);
    Serial.printf("Temperature: %4.1f *C \n", tmp);

    M5.Lcd.setCursor(0,38);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("T: %4.1f *C  \n", tmp);
  
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0,66);
    vbat = M5.Axp.GetBatVoltage();
    if (vbat < 3.5) M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.printf("\r\nVbat: %5.3f V   \r\n", vbat);
    int charge = M5.Axp.GetBatCurrent();
    M5.Lcd.printf("Icharge: %2d mA   \r\n", charge);
    Serial.printf("Vbat: %5.3f V  Icharge: %3d mA\r\n",vbat, charge);
  }    
        
  M5.update();  // Read the press state of the key.  
  if (M5.BtnA.wasPressed()) sendMessage();
  
  //delay(5000);
}

/*--------------------------------------------------------------*/
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

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
