/*
 * File: NodeMCU_MQTTClient.ino
 * 
 * based on the library example SimpleMQTTClient.ino
 * 
 * The purpose of this example is to illustrate a simple handling of MQTT and Wifi connection.
 * Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
 * It will also send a message delayed 5 seconds later.
 * A float variable is converted to string and is sent to the MQTT broker.
 */
 
#include "EspMQTTClient.h"
#include "arduino_secrets.h"

#define TOPIC01 "nRF24L01/Node01/temperature"

const unsigned long cycle = 10000; // 60 sec
unsigned long now;

float value = 12.34;    // simulated measuring value
char buffer[10];        // buffer for format conversion

void setup()
{
  Serial.begin(115200);

  dtostrf(value,3,2,buffer);

  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("nRF24L01_Gateway/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  
  // Publish a message to TOPIC
  client.publish(TOPIC01, buffer); // You can activate the retain flag by setting the third parameter to true
  now = millis();
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
  if (millis() - now > cycle)
  {
    now = millis();

    value += 0.01; dtostrf(value,3,2,buffer); 
    
    // Publish a message to TOPIC
    client.publish(TOPIC01, buffer); // You can activate the retain flag by setting the third parameter to true
  }
}
