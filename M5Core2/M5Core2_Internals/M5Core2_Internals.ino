/*
 * File: M5Core_Internals.ino
 * 
 * 2021-01-17 info@ckuehnel.ch
 */
#include <M5Core2.h>

const uint16_t CYCLE = 5000; // ms
unsigned long now = 0;
String msg = "";

void setup() 
{
  M5.begin();
  Serial.println("\nM5Core2 Internals");
  //M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.setCursor(0, 10, 4);
  M5.Lcd.println("M5Core2 Internals");
  M5.Lcd.setTextColor(WHITE, BLACK);
} 

void loop() 
{
  now = millis();

  M5.Lcd.fillRect(0, 40, 320, 180, BLACK);
  M5.Lcd.setCursor(0, 50, 4);
  
  msg = "Battery Current = " + String(M5.Axp.GetBatCurrent(),0) + " mA";
  M5.Lcd.println(msg); Serial.println(msg);

  msg = "Bus Voltage = " + String(M5.Axp.GetVBusVoltage(),1) + " V";
  M5.Lcd.println(msg); Serial.println(msg);

  msg = "Bus Current = " + String(M5.Axp.GetBatCurrent(),0) + " mA";
  M5.Lcd.println(msg); Serial.println(msg);

  msg = "AXP192 Chip Temp. = " + String(M5.Axp.GetTempInAXP192(),0) + " *C";
  M5.Lcd.println(msg); Serial.println(msg);

  msg = "Battery Power = " + String(M5.Axp.GetBatPower(),0) + " mAh";
  M5.Lcd.println(msg); Serial.println(msg);
  
  msg = "Bat. Charg. Cur. = " + String(M5.Axp.GetBatChargeCurrent(),0) + " mA";
  M5.Lcd.println(msg); Serial.println(msg);

  while(millis() < now + CYCLE);
}
