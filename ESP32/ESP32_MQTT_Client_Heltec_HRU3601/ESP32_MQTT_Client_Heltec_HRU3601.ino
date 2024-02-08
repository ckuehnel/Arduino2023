/*
 * File: Oxocard_MQTT_Client_Kachelmann.ino
 *
 * Read the uplink content from TTN MQTT server to monitor
 * filtered content using Oxoxcard Connect as display unit.
 * The complete upload is visualized graphically by an Datacake dashboard.
 * Measuring values are transfered to weather platforms.
 * 
 * 2024-02-01 Claus Kühnel info@ckuehnel.ch
 */

#include "SPI.h"

#include <WiFi.h>

#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>        // V.7.0.2

#include "arduino_secrets.h"


#define MAX_INPUT_LENGTH 3200
char input[MAX_INPUT_LENGTH];

// Deserialize the document
JsonDocument doc;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

#define NTP_SERVER "de.pool.ntp.org"
#define WET "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // Western European Time

unsigned long t;

float temperature, humidity;
char timeStringBuff[25]; //25 chars should be enough

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
  mqttClient.setId("_Heltec_HRU-3601A_");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("ck-heltec@ttn", "NNSXS.LHJ3VON3HRICXVHFZMEUWWX7SWRVIHJXIN6PXYY.OQWYQ5JTBBALIW4WV4IHWPOF5VA6PQ36ETJ2Z6L7BRAWWZMJM62Q");

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
  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.println("ESP32 MQTT Client for Geltec HRU3601A Data");
 
  connectToWiFi();

  initTime(WET);
  Serial.println("NTP synchronized.");

  setupMQTT();
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
      if (mqttClient.subscribe("#")) 
        Serial.println("Topic subscribed.");
    }   
  }
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

    // use the Stream interface to print the contents
    uint16_t i = 0;
    while (mqttClient.available()) 
    {
      input[i++] = (char) mqttClient.read();
    }
    Serial.print(input);
    Serial.println();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, input, MAX_INPUT_LENGTH);

    if (error) 
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    for (JsonObject uplink_message_decoded_payload_0_sensor_item : doc["uplink_message"]["decoded_payload"]["0"]["sensor"].as<JsonArray>()) 
    {
      double item_data = uplink_message_decoded_payload_0_sensor_item["data"];
      int item_pk_id = uplink_message_decoded_payload_0_sensor_item["pk_id"];
      //Serial.printf("ID %d; value %5.1f\n", item_pk_id, item_data);
      if (item_pk_id == 0) 
      {
        printLocalTime();
        Serial.printf(" > Temperature = %5.1f °C\t", item_data);
      }
      else Serial.printf("Humidity = %5.1f %%rH\n", item_data);
    }

  }

}

