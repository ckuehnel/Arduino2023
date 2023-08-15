/*
 * File: TTGO_GPS_MQTT_WiFi.ino
 * 
 * WiFi Node using Mobil Phone Hotspot for Internet connection
 * 
 * Source: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G
 * Board: ESP32Dev
 *
 * 2023-07-29 Claus Kühnel info@ckuehnel.ch 
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>  // TinyGSM v0.11.7

#include <ArduinoJson.h>


// Set serial for AT commands (to the module)
#define SerialAT Serial1

TinyGsm       modem(SerialAT);
//TinyGsmClient client(modem);

WiFiClient    espClient;
PubSubClient  mqtt(espClient);

#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12
#define BAT_ADC     35

#define DEBUG       1

// GPS variables
float lat,  lon, speed, alt;
int usat;

// ENV.II variables
float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;
bool sensorActive = true;
bool GPSActive = true;

// MQTT variables
char msg[256];
size_t len = 0; 

uint32_t lastReconnectAttempt = 0;
uint32_t lastMeasure = 0;
boolean firstRun;

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.write(payload, len);
    Serial.println();

    // Only proceed if incoming message's topic matches
    if (String(topic) == topicSensor) 
    {
      Serial.print("sensor: ");
      sensorActive = payload[0] - 0x30;  // payload 0 or 1
      Serial.println(sensorActive);
    }
    if (String(topic) == topicGPS) 
    {
      Serial.print("GPS: ");
      GPSActive = payload[0] - 0x30;  // payload 0 or 1
      Serial.println(GPSActive);
    }
}

boolean mqttConnect()
{
    Serial.print("Connecting to ");
    Serial.print(broker);

    // Connect to MQTT Broker
    boolean status = mqtt.connect("TTGO");

    if (status == false) 
    {
        Serial.println(" fail");
        return false;
    }
    Serial.println(" success");
    mqtt.publish(topicUpdate, msg);
    mqtt.subscribe(topicSensor);
    mqtt.subscribe(topicGPS);
    blink(5);
    return mqtt.connected();
}


void setup()
{
    // Set console baud rate
    Serial.begin(115200);
    delay(1000);  // wait for serial monitor

    Serial.println("\nStarting TTGO-GPS-WiFi-MQTT Client...");

    // Set LED OFF
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);

    Serial.println("Initializing...");
    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    initWiFi();

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.restart()) Serial.println("Failed to restart modem, attempting to continue without restarting");

    // MQTT Broker setup
    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);

    initSensor();   // Initialization of M5Stack ENV.II unit
    firstRun = true;
    Serial.println("Initialization finished.");
}

void loop()
{
  uint32_t measure = millis();
  if ((measure - lastMeasure > 300000L) || firstRun) // measuring cycle is 5 min
  {
    firstRun = false;
    lastMeasure = measure;

    blink(2);

    float Vbat = readBattery(BAT_ADC);
    bool isUSBpowered = false;
  
    Serial.printf("Vbat  = %5.0f mV\n", Vbat);

    if (sensorActive)
    {
      getValues();  // Read measuring values from M5Stck ENV.II unit
    }
    
    if (Vbat < 0.1) 
    {
      isUSBpowered = true;   // USB powered device
      Serial.printf("USB powered device\n");
    }

    if (GPSActive) readGPS();
 
    StaticJsonDocument<256> doc;
    doc["node"] = "TTGO-LTE";

    if (GPSActive)
    {
      String sLat = String(lat,6); doc["lat"] = sLat;   // conversion float to string
      String sLon = String(lon,6); doc["lon"] = sLon;
      String sAlt = String(alt,1); doc["alt"] = sAlt;
      String sSpd = String(speed,1); doc["spd"] = sSpd;
      String sUsat = String(usat); doc["usat"] = sUsat;
    }
    if (sensorActive)
    {
      String sTemp = String(sht31Temperature,1); doc["temp"] = sTemp;
      String sHumi = String(sht31Humidity,0); doc["humi"] = sHumi;
    }
    if (isUSBpowered) doc["vbat"] = "USB powered";
    else 
    {
      String sVbat = String(Vbat,0); doc["vbat"] = sVbat;
    }

    // Generate the minified JSON and send it to the Serial port.
    serializeJsonPretty(doc, Serial);
    Serial.println();

    // Prepare the message to send by MQTT
    serializeJsonPretty(doc, msg);
    len = strlen(msg);
    Serial.printf("Message length = %d\n", len);

    uint16_t interval = 10000;
    unsigned long previousMillis = millis(), currentMillis = millis();
    // if WiFi is down, try reconnecting
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) 
    {
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
      blink(10);
      previousMillis = currentMillis;
    }

    //mqtt.disconnect();
    if (!mqtt.connected()) 
    {
        Serial.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 10000L) 
        {
            lastReconnectAttempt = t;
            if (mqttConnect()) lastReconnectAttempt = 0;
        }
        delay(100);
        return;
    }
    else mqttConnect();
  }
  mqtt.loop();
}
