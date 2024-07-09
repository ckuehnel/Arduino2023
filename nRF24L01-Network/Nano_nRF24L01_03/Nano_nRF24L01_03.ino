/*
   File Nano_nRF24L01_03.ino

   Node03 in nRF24L01 network
   based on Arduino Wireless Network - Multiple NRF24L01 Tutorial
   by Dejan, www.HowToMechatronics.com
   used Libraries
   - nRF24/RF24, https://github.com/nRF24/RF24
   - nRF24/RF24Network, https://github.com/nRF24/RF24Network

   Claus Kühnel 2020-06-04 info@ckuehnel.ch
*/
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(7, 8);                 // CE, CSN
RF24Network network(radio);       // Include the radio in the network

const uint16_t this_node = 03;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00  = 00;    // Address of the other node in Octal format
const byte ch = 4;

const unsigned long interval = 10000;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?
unsigned long count = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.print("\nInitializing Radio...");
  SPI.begin();
  radio.begin();
  radio.setChannel(ch);
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  network.begin(this_node);  //(channel, node address)
  Serial.println("done.");
  Serial.print("Ready to transmit messages on channel ");
  Serial.println(radio.getChannel());
}

void loop()
{
  network.update();

  //===== Sending =====//
  unsigned long now = millis();

  if (now - last_sent >= interval)    // If it's time to send a data, send it!
  {
    last_sent = now;

    Serial.print("Transmit "); Serial.print(count); Serial.println(" to Base 00");
    RF24NetworkHeader header1(master00);   // (Address where the data is going)
    bool ok = network.write(header1, &count, sizeof(count)); // Send the data
    if (!ok) Serial.println("Transmission failed");
    count++;
  }
}
