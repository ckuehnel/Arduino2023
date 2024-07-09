/*
 * File: ESP32_MQTT_Meshtastic.ino
 *
 * Read the Meshtastic upload content from Hivemq Public Broker 
 * and decode for console output.
 * 
 * 2024-02-12 Claus Kühnel info@ckuehnel.ch
 */
#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>        // V.7.0.2
#include <TimeLib.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

#define MAX_INPUT_LENGTH 1000
char input[MAX_INPUT_LENGTH];

// Deserialize the document
JsonDocument doc;

// MQTT client
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void connectToWiFi() 
{
  Serial.print("Connecting to ");
 
  WiFi.begin(ssid, pass);
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected."); 
} 

void reconnect() 
{
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) 
  {
    Serial.println("Reconnecting to MQTT Broker..");
           
    if (mqttClient.connect(broker, port)) 
    {
      Serial.println("MQTT Connected.");
      // subscribe to topic
      if (mqttClient.subscribe("msh/#")) 
        Serial.println("Topic subscribed.");
    }   
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  tm local;
  getLocalTime(&local);
  Serial.println(&local, "Date: %d.%m.%y  Time: %H:%M:%S"); // formatted output
  
  Serial.println("Message received:");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setupMQTT() 
{
  // You can provide a unique client ID. Each client must have a unique client ID.
  mqttClient.setId("_Meshtastic_");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("", "");

  Serial.print("Attempting to connect to the MQTT broker: "); Serial.println(broker);

  if (!mqttClient.connect(broker, port)) 
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");

    // subscribe to a topic
  Serial.print("Subscribing to topic: "); Serial.println(topic);
  mqttClient.subscribe(topic);

  Serial.print("Waiting for messages on topic: ");   Serial.println(topic);
  //Serial.println();
}

void setup() 
{
  Serial.begin(9600);
  delay(2000); // wait for serial monitor
  Serial.println("ESP32 Client for Meshtastic Data");
  connectToWiFi();
  setupMQTT();
}

void loop() 
{
  if (!mqttClient.connected()) reconnect();

  int messageSize = mqttClient.parseMessage();
  if (messageSize) 
  {
    // we received a message, print out the topic and contents
    Serial.print("\nReceived a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    uint16_t i = 0;
    while (mqttClient.available()) 
    {
      input[i++] = (char) mqttClient.read();
    }
    //Serial.print(input);
    //Serial.println();

    DeserializationError error = deserializeJson(doc, input, MAX_INPUT_LENGTH);

    if (error) 
    {
      //Serial.print("deserializeJson() failed: ");
      //Serial.println(error.c_str());
      Serial.println("No valid JSON message");
      return;
    }
    else
    {
      int channel = doc["channel"]; // 1
      long long from = doc["from"]; // 2053978516
      long id = doc["id"]; // 1643399270

      JsonObject payload = doc["payload"];
      double payload_air_util_tx = payload["air_util_tx"]; // 0.579777777194977
      int payload_battery_level = payload["battery_level"]; // 42
      float payload_channel_utilization = payload["channel_utilization"]; // 0
      double payload_voltage = payload["voltage"]; // 3.73399996757507
      float payload_temperature = payload["temperature"];
      float payload_relative_humidity = payload["relative_humidity"];
      const char* payload_longname = payload["longname"];
      double payload_latitude = payload["latitude_i"];
      double payload_longitude = payload["longitude_i"];
      int payload_sats_in_view = payload["sats_in_view"];

      int rssi = doc["rssi"]; // -126
      const char* sender = doc["sender"]; // "!fa66367c"
      float snr = doc["snr"]; // -12
      long timestamp = doc["timestamp"]; // 1707691897
      long long to = doc["to"]; // 4294967295
      const char* type = doc["type"]; // "telemetry"
                  
      Serial.printf("Channel %d\n", channel);
      Serial.printf("Sent from %u\n", from);
      if (rssi != 0 && snr != 0)
        Serial.printf("RSSI = %d dBm, SNR = %3.2f dBm\n", rssi, snr);
      Serial.printf("UNIX Time Stamp %ld\n", timestamp);
      Serial.printf("UTC Date: %4d-%02d-%02d %02d:%02d:%02d\n", year(timestamp), month(timestamp), day(timestamp), hour(timestamp), minute(timestamp), second(timestamp));
      Serial.printf("Type is %s\n", type);
      if (strcmp(type, "telemetry") == 0)
      {
        if (payload_air_util_tx > 0)
          Serial.printf("Air_Util_TX %3.3f %%\n", payload_air_util_tx);
        if (payload_channel_utilization > 0)
          Serial.printf("Channel Utilization %3.3f %%\n", payload_channel_utilization);
        if (payload_battery_level != 0 && payload_voltage != 0)
          Serial.printf("Battery Level %d %%  Voltage %3.3f V\n", payload_battery_level, payload_voltage);
        if (payload_temperature != 0 && payload_relative_humidity != 0)
          Serial.printf("Temperature %3.1f °C   Relative Humidity %3.0f %%rH\n", payload_temperature, payload_relative_humidity);
      }
      if (strcmp(type, "nodeinfo") == 0)
      {
        Serial.printf("Longname of node is %s\n", payload_longname);
      }
      if (strcmp(type, "position") == 0)
      {
        if (payload_sats_in_view > 2)
        {
          Serial.printf("Latitude  %3.7f\n", payload_latitude/1e7);
          Serial.printf("Longitude %3.7f\n", payload_longitude/1e7);
          Serial.printf("%d Satellites in View\n", payload_sats_in_view);
        }
        else Serial.printf("Not enough satellites in view\n");
      }

    }
  }
  
}
