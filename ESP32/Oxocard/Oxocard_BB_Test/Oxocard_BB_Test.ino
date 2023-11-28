// Oxocard Connect 

#include "pins.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include <Wire.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

#define EEPROM_ADDRESS 0b1010000 //0b1010(A2 A1 A0)

ExternalEEPROM myEEPROM;

void setup() 
{
  pinMode(USR_BTN1, INPUT);
  pinMode(USR_BTN2, INPUT);
  pinMode(USR_BTN3, INPUT);
  pinMode(USR_BTN4, INPUT);
  pinMode(USR_BTN5, INPUT);

  pinMode(IO20, OUTPUT); digitalWrite(IO20, LOW);
  pinMode(IO25, OUTPUT); digitalWrite(IO25, LOW);
  pinMode(IO26, OUTPUT); digitalWrite(IO26, LOW);
  pinMode(IO32, OUTPUT); digitalWrite(IO32, LOW);
  pinMode(IO33, OUTPUT); digitalWrite(IO33, LOW);

  pinMode(LCD_LED, OUTPUT); digitalWrite(LCD_LED, HIGH); // LCD background light on
  tft.init();
  tft.setRotation(1);
 

  tft.fillScreen(TFT_BLACK);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Oxocard Connect Breadboard Test");

  Wire.begin();

  myEEPROM.setMemoryType(512); // Valid types: 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1025, 2048

  if (myEEPROM.begin(EEPROM_ADDRESS, Wire) == false) 
  {
    Serial.println("No memory detected. Freezing.");
    while (true);
  }
  Serial.println("Memory detected!");  
}

void loop() 
{
	tft.setTextColor(TFT_GREEN);
  tft.setTextDatum(TC_DATUM);     // Centre text on x,y position
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("Oxocard Connect", 120, 10);
	tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeMono12pt7b);
  tft.drawString("Test of", 120, 50);
  tft.drawString("Oxocard Connect", 120, 70);
  tft.drawString("features", 120, 90);
  tft.drawString("programmed by", 120, 110);
  tft.drawString("Arduino IDE.", 120, 130);
  tft.drawString("Follow the", 120, 160);
  tft.drawString("console output.",120,180);

  Serial.println("\nKey Test");
  Serial.println("Press Button");
  while(digitalRead(USR_BTN5)==0);
  Serial.println("> Button pressed.");
  Serial.println("Press Left");
  while(digitalRead(USR_BTN1)==0);
  Serial.println("> Left pressed.");
  Serial.println("Press Right");
  while(digitalRead(USR_BTN2)==0);
  Serial.println("> Right pressed.");
  Serial.println("Press Down");
  while(digitalRead(USR_BTN3)==0);
  Serial.println("> Down pressed.");
  Serial.println("Press Up");
  while(digitalRead(USR_BTN4)==0);
  Serial.println("> Up pressed.");

  Serial.println("\nBlink LED Line");
  digitalWrite(IO20, !digitalRead(IO20)); delay(50);
  digitalWrite(IO25, !digitalRead(IO25)); delay(50);
  digitalWrite(IO26, !digitalRead(IO26)); delay(50);
  digitalWrite(IO32, !digitalRead(IO32)); delay(50);
  digitalWrite(IO33, !digitalRead(IO33)); delay(50);
  
  Serial.println("\nRead EEPROM");
    uint32_t eepromSizeBytes = myEEPROM.detectMemorySizeBytes();
  Serial.print("Detected EEPROM size (bytes): ");
  Serial.print(eepromSizeBytes);
  Serial.print(" bytes / ");
  if (eepromSizeBytes < 128)
  {
    Serial.print(eepromSizeBytes * 8);
    Serial.print(" Bits");
  }
  else
  {
    Serial.print(eepromSizeBytes * 8 / 1024);
    Serial.print(" kBits");
  }
  Serial.print(" - 24XX");
  if (eepromSizeBytes == 16)
    Serial.print("00");
  else
  {
    if ((eepromSizeBytes * 8 / 1024) < 10) Serial.print("0");
    Serial.print(eepromSizeBytes * 8 / 1024);
  }
  Serial.println();

  Serial.print("Memory Contents:");
  for (uint16_t x = 0; x < 32 * 4; x++)
  {
    if (x % 16 == 0) Serial.println();
    Serial.print(" 0x");
    if (myEEPROM.read(x) < 0x10) Serial.print("0");
    Serial.print(myEEPROM.read(x), HEX);
  }
  Serial.println();

  for (uint16_t x = 14; x < 32 * 4; x++)
  {
    char c = myEEPROM.read(x);
    if (c == 0) 
    {
      Serial.println();
      return;
    }
    else Serial.print(c);
  }
  

  delay(2000);
}
