/*
 * File: StackyPi_ESB_OPT3002.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * Light sensor OPT3002 is connected to I2C0.
 * 
 * based on:
 * https://github.com/closedcube/ClosedCube_OPT3002_Arduino/blob/master/examples/opt3002demo/opt3002demo.ino
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <SPI.h>
#include <Wire.h>


// Coral ESB Pin Definitions
;

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  Serial.printf("done\n");

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("Scanning both i2C busses.");
}

void scan(uint8_t bus)
{
  byte error, address;
  int nDevices;

  Serial.print("Scanning ");
  if (bus == 0) Serial.print(0);
  else          Serial.print(1);
  Serial.println(" ...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    if (bus == 0)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
    }
    else
    {
      Wire1.beginTransmission(address);
      error = Wire1.endTransmission();
    }

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
      Serial.print("0");
      Serial.println(address,HEX);

      nDevices++;
    }
/*    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }*/
  }
  if (nDevices == 0)
  Serial.println("No I2C devices found\n");
  else
  Serial.println("done\n");
}

void loop()
{
  scan(0);
  delay(2000);
  scan(1);
  delay(5000);
}
