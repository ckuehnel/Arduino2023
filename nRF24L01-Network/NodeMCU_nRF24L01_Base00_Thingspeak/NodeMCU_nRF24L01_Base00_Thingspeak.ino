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

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

WiFiClient  client;

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

const unsigned long interval = 10 * 60000;  // 10 min
unsigned long last_sent;            // When did we last send?

float valueN01, valueN02, valueN013, valueN023;

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nInitializing WiFi...");
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
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

void loop() 
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    Serial.print("\nAttempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      
      Serial.print(".");
      delay(500);
    }
    Serial.println("\nConnected.");
  }
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
    ThingSpeak.setField(1, valueN01); valueN01 = 99;
    ThingSpeak.setField(2, valueN02); valueN02 = 99;
    ThingSpeak.setField(3, valueN013); valueN013 = 99;
    ThingSpeak.setField(4, valueN023); valueN023 = 99;
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (httpCode == 200) Serial.println("Channel write successful.");
    else                 Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
}
