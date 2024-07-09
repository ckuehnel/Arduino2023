/*
 * File: Heltec_Lora32_ttn_abp.ino
 * 
 * using https://github.com/rgot-org/TheThingsNetwork_esp32
 * 
 * 2021-07-29 Claus Kühnel info@ckuehnel.ch
 */
 
#include <TTN_esp32.h>
//#include "TTN_CayenneLPP.h"
#include <CayenneLPP.h>
#include "arduino_secrets.h"

#define DEBUG 1

// OLED Library https://github.com/ThingPulse/esp8266-oled-ssd1306
#include <SSD1306.h>

//OLED pins to ESP32 GPIOs on Heltec WiFi LoRa 32:
//SDA & SCL pins used for ENV.II Unit, too
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

SSD1306  display(0x3c, 4, 15);

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

const int pLED = 25;
const int pResetOLED = 16;

const long CYCLE = 5 * 60000; // Transmission cycle 5 minutes
unsigned long previousMillis = 0;

TTN_esp32 ttn ;
CayenneLPP lpp(51); // maximim payload

void message(const uint8_t* payload, size_t size, int rssi)
{
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(size) + " bytes RSSI=" + String(rssi) + "db");
    for (int i = 0; i < size; i++)
    {
        Serial.print(" " + String(payload[i]));
        // Serial.write(payload[i]);
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting");
    ttn.begin();
    ttn.onMessage(message); // Declare callback function for handling downlink
                            // messages from server
    ttn.join(devEui, appEui, appKey);
    Serial.print("Joining TTN ");
    while (!ttn.isJoined())
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\njoined !");
    ttn.showStatus();
}

void loop()
{
  static float nb = 18.2;
  nb += 0.1;
  
  lpp.reset();
  lpp.addTemperature(1, nb);

  if (DEBUG)
  { 
    Serial.print("Payload length =  ");
    Serial.println(lpp.getSize());

    uint8_t *payload = lpp.getBuffer();

    for (uint8_t i = 0; i < lpp.getSize(); i++)
    {
      Serial.print("0x");
      Serial.print(payload[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize()))
  {
    Serial.printf("Temp: %f TTN_CayenneLPP: %d %x %02X%02X\n", nb, lpp.getBuffer()[0], lpp.getBuffer()[1], lpp.getBuffer()[2], lpp.getBuffer()[3]);
  }
  delay(10000);
}
