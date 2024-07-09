/*
 * File: Nano_nRF24L01_Rx.ino
 * 
 * nRF24L01 Peer-to-Peer Test
 * based on Arduino Wireless Communication Tutorial
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * Used Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN  // Arduino Nano
//RF24 radio(D1, D2); // CE, CSN  // NodeMCU

const byte address[6] = "00001";
const byte channel = 4;

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.print("\nInitializing Radio...");
  radio.begin();
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  radio.openReadingPipe(1, address);
  radio.startListening();
  Serial.println("done.");
  Serial.print("Ready to receive messages on channel "); 
  Serial.println(radio.getChannel());
}

void loop() 
{
  if (radio.available()) 
  {
    char text[32] = ""; // max payload
    radio.read(&text, sizeof(text));
    Serial.print(text);

    bool goodSignal = radio.testRPD();
    Serial.println(goodSignal ? "\t\tStrong signal > 64dBm" : "\t\tWeak signal < 64dBm" );
    radio.read(0,0);
  }
}
