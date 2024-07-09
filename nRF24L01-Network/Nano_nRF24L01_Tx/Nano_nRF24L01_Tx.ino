/*
 * File: Nano_nRF24L01_Tx.ino
 * 
 * nRF24L01 Peer-to-Peer Test
 * based on Arduino Wireless Communication Tutorial
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * Used Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
const byte channel = 4;
const uint16_t Periode = 2000;
unsigned long previousMillis;
unsigned int count = 0;
char buffer[10];

void setup() 
{ 
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.print("Initializing Radio...");
  pinMode(LED_BUILTIN, OUTPUT);
  radio.begin();
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  radio.openWritingPipe(address);
  Serial.println("done.");
  Serial.print("Ready to transmit messages on channel "); 
  Serial.println(radio.getChannel());
}

void loop() 
{
  if (millis() - previousMillis > Periode)
  {
    previousMillis = millis();
    char text[] = "Count: ";
    
    dtostrf(count++, 8, 0, buffer);
    strcat (text, buffer);
    Serial.print("Send message...\t");
    Serial.println(text); 
    digitalWrite(LED_BUILTIN, HIGH); delay(500);
    
    radio.write(&text, strlen(text));
    digitalWrite(LED_BUILTIN, LOW);
  }
}
