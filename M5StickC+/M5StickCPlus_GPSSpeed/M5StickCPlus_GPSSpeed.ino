/*
 * File: M5StickCPlus_GPSSpeed.ino
 * 
 * GPS based speed measuring by M5Stack GPS Unit.
 * UART2 is redirected to Grove Port for serial communication w/ GPS Unit @ 9600 bps
 * 
 * 2021-01-18 Claus Kühnel info@ckuehnel.ch
 */
 
#include <M5StickCPlus.h>
#include <FuGPS.h>        // https://github.com/fu-hsi/FuGPS

HardwareSerial GPS(2);    // UART2 for GPS

FuGPS fuGPS(GPS);
bool gpsAlive = false;
String s = "";

void setup() 
{
  M5.begin();
  M5.Lcd.setRotation(1);
  GPS.begin(9600, SERIAL_8N1, 33, 32);  // GPS init for M5StickC

  Serial.println("\nReading GPSUnit connected to M5StickC Plus");

  M5.Lcd.fillRect(0,0,240,30, BLUE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_ORANGE, BLUE);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("M5StickC+ GPS Data");
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(BC_DATUM);
} 
  
void loop() 
{
  byte quality, satellites;
  float _speed;
  
  // Valid NMEA message
  if (fuGPS.read())
  {
    // We don't know, which message was came first (GGA or RMC).
    // Thats why some fields may be empty.
    gpsAlive = true;
    
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print("Qual:");
    M5.Lcd.setCursor(80, 40);
    M5.Lcd.print(quality, BLACK);
    quality = fuGPS.Quality;
    M5.Lcd.setCursor(80, 40);
    M5.Lcd.print(quality);
        
    M5.Lcd.setCursor(120, 40);
    M5.Lcd.print("#Sat:");
    M5.Lcd.setCursor(190, 40);
    M5.Lcd.print(satellites, BLACK);
    satellites = fuGPS.Satellites;
    M5.Lcd.setCursor(190, 40);
    M5.Lcd.print(satellites);       

    if (fuGPS.hasFix() == true)
    {
      // Data from GGA message
      Serial.print("Accuracy (HDOP): ");
      Serial.println(fuGPS.Accuracy);

      Serial.print("Altitude (above sea level): ");
      Serial.println(fuGPS.Altitude);

      // Data from GGA or RMC
      Serial.print("Location (decimal degrees): ");
      Serial.println("https://www.google.com/maps/search/?api=1&query=" + String(fuGPS.Latitude, 6) + "," + String(fuGPS.Longitude, 6));

      M5.Lcd.setCursor(0, 110);
      M5.Lcd.print("Speed: ");
      M5.Lcd.setCursor(190, 110);
      M5.Lcd.print("km/h");

      //M5.Lcd.fillRect(80, 100, 100, 35, BLACK);
      M5.Lcd.setTextColor(BLACK, BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.drawString(s, 130, 130);
      _speed = fuGPS.Speed * 1.852;
      s = String(_speed,0);
      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.drawString(s, 130, 130);
      M5.Lcd.setTextSize(2);
    }

    // Default is 10 seconds
    if (fuGPS.isAlive() == false)
    {
      if (gpsAlive == true)
      {
        gpsAlive = false;
        Serial.println("GPS module not responding with valid data.");
        Serial.println("Check wiring or restart.");
      }
    }
  }
}
