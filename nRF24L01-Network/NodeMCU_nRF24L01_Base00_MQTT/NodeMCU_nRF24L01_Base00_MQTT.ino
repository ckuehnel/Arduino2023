/*
 * File NodeMCU_nRF24L01_Base00_Thingspeak.ino
 * 
 * based on Arduino Wireless Network - Multiple NRF24L01 Tutorial
 * by Dejan, www.HowToMechatronics.com
 * used Libraries
 * - nRF24/RF24, https://github.com/nRF24/RF24
 * - nRF24/RF24Network, https://github.com/nRF24/RF24Network
 * - esp8266 by ESP8266 Community
 * - ThingSpeak by MathWorks
 * 
 * Claus Kühnel 2020-06-21 info@ckuehnel.ch
 */

#include "EspMQTTClient.h"
#include "arduino_secrets.h"

#define TOPIC01 "nRF24L01/Node01/temperature"
#define TOPIC02 "nRF24L01/Node02/temperature"
#define TOPIC013 "nRF24L01/Node013/temperature"
#define TOPIC023 "nRF24L01/Node023/temperature"

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(D1, D2);               // nRF24L01 (CE,CSN)
RF24Network network(radio);       // Include the radio in the network

const uint16_t this_node = 00;    // Address of this node in Octal format
const uint16_t node01    = 01;    // Address of the other nodes in Octal format
const uint16_t node02    = 02;
const uint16_t node03    = 03;
const uint16_t node04    = 04;    // for test purposes only
const uint16_t node013   = 013;
const uint16_t node023   = 023;

const uint8_t  ch = 4;            // Used channel number

const unsigned long interval = 60000;  // 1 min
unsigned long last_sent;            // When did we last send?

float valueN01, valueN02, valueN013, valueN023;
char buffer[10];        // buffer for format conversion

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nInitializing WiFi & MQTT...");
  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableLastWillMessage("nRF24L01_Gateway/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  Serial.print("Initializing Radio...");
  SPI.begin();
  radio.begin();
  radio.setChannel(ch); // set channel
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  network.begin(ch, this_node);  //(channel, node address)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("done.");
  Serial.print("Ready to receive messages on channel "); 
  Serial.println(radio.getChannel());
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "NODE/wildcardtest/#" and display received message to Serial
  client.subscribe("nRF24L01/#", [](const String & topic, const String & payload) 
  {
    Serial.println(topic + ": " + payload);
  });
}

void loop() 
{
  client.loop();
  network.update();
  //===== Receiving =====//
  while ( network.available() )   // Is there any incoming data?
  {
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    //Serial.print("Message from Node "); Serial.println(header.from_node);
    if (header.from_node == node01) 
    {    // If data comes from Node 01
      valueN01 = (float) incomingData / 10.;
      Serial.print("Received from Node 01: "); Serial.println(valueN01,1);
    }
    if (header.from_node == node02) 
    {    // If data comes from Node 02
      valueN02 = (float) incomingData / 10.;
      Serial.print("Received from Node 02: "); Serial.println(valueN02,1);
    }
    if (header.from_node == node03) 
    {    // If data comes from Node 03
      Serial.print("Received from Node 03: "); Serial.println(incomingData);
    }
    if (header.from_node == node04) 
    {    // If data comes from Node 04
      Serial.print("Received from Node 04: "); Serial.println(incomingData);
    }
    if (header.from_node == node013) 
    {    // If data comes from Node 013
      valueN013 = (float) incomingData / 10.;
      Serial.print("Received from Node 013: "); Serial.println(valueN013,1);
    }
    if (header.from_node == node023) 
    {    // If data comes from Node 023
      valueN023 = (float) incomingData / 10.;
      Serial.print("Received from Node 023: "); Serial.println(valueN023,1);
    }
  }
  unsigned long now = millis();
  if (now - last_sent >= interval)    // If it's time to send a data, send it!
  {
    last_sent = now;
    dtostrf(valueN01,3,1,buffer); valueN01 = 99;
    client.publish(TOPIC01, buffer);
    dtostrf(valueN02,3,1,buffer); valueN02 = 99;
    client.publish(TOPIC02, buffer);
    dtostrf(valueN013,3,1,buffer); valueN013 = 99;
    client.publish(TOPIC013, buffer);
    dtostrf(valueN023,3,1,buffer); valueN023 = 99;
    client.publish(TOPIC023, buffer); 
  }
}
