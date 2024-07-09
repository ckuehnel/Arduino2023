/*
 * File: NodeMCU_nRF24L01_Init.ino
 * 
 * Test Initialization of nRFL24L01 per Default & user defined
 * 
 * based on Arduino Wireless Communication Tutorial
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * Used Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(D1, D2); // CE, CSN

//const byte address[6] = "00001";
const byte channel = 4;

void setup() 
{
  Serial.begin(115200);
  printf_begin();
  delay(1000); // wait for Serial Monitor
  
  Serial.print("Default Initializing Radio by Rf24 Library...");
  radio.begin();
  Serial.println("done.\n");
  radio.printDetails();

  Serial.print("\nUser defined Initializing Radio...");
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
//  radio.openReadingPipe(1, address);
  Serial.println("done.\n");
  radio.printDetails();
  Serial.println("\nEnd."); 
}

void loop() {}
