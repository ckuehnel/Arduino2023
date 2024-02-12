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
#include "pins.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>        // V.7.0.2
#include <UrlEncode.h>
#include "arduino_secrets.h"


// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

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

float temperature, humidity, pressure, wind_speed;
char timeStringBuff[25]; //25 chars should be enough

// MQTT client
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// HTTP Client  
HTTPClient http;

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
  mqttClient.setId("_Oxocard_");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("cksensorhub@ttn", "NNSXS.W4DZ6DNZWZ3T6BLRL54UR334L5VJDVSOZEHA3RI.MKJJWLV6REV3MMJMASA2AK6N2TCF2RRYZDPW2FG6IBC4GIGAQMRQ");

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
  pinMode(LCD_LED, OUTPUT); digitalWrite(LCD_LED, HIGH); // LCD background light on
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN);
  tft.setTextDatum(TC_DATUM);     // Centre text on x,y position
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("Oxocard Connect", 120, 10);
  tft.drawString("SensorHub Data", 120, 27);
  tft.drawString("to Weather", 120, 44);
  tft.drawString("Platforms", 120, 61);
	tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeMonoBold18pt7b);
  tft.drawString("waiting...", 120, 120);

  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.println("Oxocard MQTT Client for SensorHub Data");
 
  connectToWiFi();

  initTime("UTC");
/*
  Serial.println("Get NTP Time...");
  struct tm local;
  configTzTime(TZ_INFO, NTP_SERVER); // NTP Synchronization
  getLocalTime(&local, 10000);       // for 10 s */
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

    Serial.println("Change timezone to Berlin");
    setTimezone(WET); // Western European Time
    printLocalTime();
    //tm local;
    //getLocalTime(&local);
    //Serial.print(&local, "Date: %d.%m.%y  Time: %H:%M:%S: "); // formatted output

    // use the Stream interface to print the contents
    uint16_t i = 0;
    while (mqttClient.available()) 
    {
      input[i++] = (char) mqttClient.read();
    }
    Serial.print(input);
    Serial.println();

    // The filter: it contains "true" for each value we want to keep
    JsonDocument filter;
    filter["uplink_message"]["decoded_payload"]["temperature_3"] = true;
    filter["uplink_message"]["decoded_payload"]["humidity_prec_4"] = true;
    filter["uplink_message"]["decoded_payload"]["barometer_5"] = true;
    filter["uplink_message"]["decoded_payload"]["wind_speed_1"] = true;
    filter["uplink_message"]["decoded_payload"]["probe_sn_0"] = true;

    DeserializationError error = deserializeJson(doc, input, DeserializationOption::Filter(filter));
    if (error) 
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // Print the result
    Serial.println("Filtered result:");
    serializeJsonPretty(doc, Serial);
    Serial.println();
    //printLocalTime();

    JsonObject uplink_message_decoded_payload = doc["uplink_message"]["decoded_payload"];
    temperature = uplink_message_decoded_payload["temperature_3"];
    humidity = uplink_message_decoded_payload["humidity_prec_4"]; 
    pressure = uplink_message_decoded_payload["barometer_5"];
    wind_speed = uplink_message_decoded_payload["wind_speed_1"]; 
    const char* probe_sn_0 = uplink_message_decoded_payload["probe_sn_0"];

    if (strcmp(probe_sn_0, "01ff") == 0) // Message contains the expected data
    {
      Serial.printf("Temperature %5.1f °C, Humidity %3.0f %%, Pressure %4.1f hPa, Wind Speed %4.2f m/s, \n", temperature, humidity, pressure, wind_speed);
      tft.fillRect(0, 90, 240, 150, TFT_BLACK);
      tft.drawString(String(temperature,1) + " °C", 120, 90);
      tft.drawString(String(humidity,0) + " %", 120, 120);
      tft.drawString(String(pressure,1) + " hPa", 120, 150);
      tft.drawString(String(wind_speed,1) + " m/s", 120, 180);

      sendData2Weathercloud();
      sendData2Meteologix();
    }
    else Serial.println("Not requested data");
  }
  
}
