/*
 * File Nano_NTC_nRF24L01_013.ino
 * 
 * Node01 w/ NTC on A2 in nRF24L01 network
 * based on Arduino Wireless Network - Multiple NRF24L01 Tutorial
 * by Dejan, www.HowToMechatronics.com
 * used Libraries
 * - nRF24/RF24, https://github.com/nRF24/RF24
 * - nRF24/RF24Network, https://github.com/nRF24/RF24Network
 * 
 * Claus Kühnel 2020-06-17 info@ckuehnel.ch
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <math.h>

RF24 radio(7, 8);                 // nRF24L01 (CE,CSN)
RF24Network network(radio);       // Include the radio in the network

const uint16_t this_node = 013;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00  = 00;    // Address of the other node in Octal format
const byte channel = 4;

const unsigned long interval = 2000;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A2;     // Grove - Temperature Sensor connect to A0

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.print("\nInitializing Radio...");
  SPI.begin();
  radio.begin();
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  network.begin(channel, this_node);  //(channel, node address)
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
    long value = getValue();
    Serial.print("Transmit "); Serial.print(value); Serial.println(" to Base 00");
    RF24NetworkHeader header(master00);   // (Address where the data is going)
    bool ok = network.write(header, &value, sizeof(value)); // Send the data
    if (!ok) Serial.println("Transmission failed");
  }
}
