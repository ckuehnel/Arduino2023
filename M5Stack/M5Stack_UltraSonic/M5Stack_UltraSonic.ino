/*
 * File: M5Stack_UltraSonic.ino
 * 
 * Ultrasonic distance mesuring by M5Stack Ultrasonic Range Sensor Unit
 * connected via I2C (PortA) to M5Stack Basic Core
 * 
 * 2021-01-18 Claus Kuehnel info@ckuehnel.ch
 */
 
#include <M5Stack.h>

TFT_eSprite Terminalbuff = TFT_eSprite(&M5.Lcd);

float newvalue = 0;
String s = "";

void setup() 
{
    M5.begin();
    Wire.begin(21,22);
    
    Terminalbuff.createSprite(320, 80);
    Terminalbuff.fillRect(0,0,320,80,BLACK);
    Terminalbuff.pushSprite(0,100);
    Terminalbuff.setTextFont(4);
    
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.fillRect(0, 0, 320, 50, BLUE);
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.drawString("Ultrasonic Distance", 160, 5, 4);
    M5.Lcd.drawString("Measurement", 160, 25,4);

    Terminalbuff.setTextSize(2);
    Terminalbuff.setTextDatum(MC_DATUM);
}

float readEUS()
{
    uint32_t data;
    Wire.beginTransmission(0x57);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(120);
    Wire.requestFrom(0x57,3);
    data  = Wire.read();data <<= 8;
    data |= Wire.read();data <<= 8;
    data |= Wire.read();
    return float(data) / 1000;
    
}

void loop() 
{
  newvalue = readEUS();

  Terminalbuff.fillRect(0,0,320,80,BLACK);
  
  if(( newvalue < 1500 )&&( newvalue > 20 ))
    s = String(newvalue/10.,2) + " cm";
  else 
    s = "Out of range";
  Terminalbuff.drawString(s, 160, 40);
  Terminalbuff.pushSprite(0,100);
  delay(100);
}
