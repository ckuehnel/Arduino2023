/*
 * File: Heltec_Lora32_ttn_abp.ino
 * 
 * using https://github.com/rgot-org/TheThingsNetwork_esp32
 * 
 * 2021-06-27 Claus Kühnel info@ckuehnel.ch
 */
 
#include <TTN_esp32.h>
#include "arduino_secrets.h"

// OLED Library https://github.com/ThingPulse/esp8266-oled-ssd1306
#include <SSD1306.h>

//OLED pins to ESP32 GPIOs on Heltec WiFi LoRa 32:
//SDA & SCL pins used for ENV.II Unit, too
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

SSD1306  display(0x3c, 4, 15);

#define DEBUG 1 

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

const int pLED = 25;
const int pResetOLED = 16;

const long CYCLE = 5 * 60000; // Transmission cycle 5 minutes
unsigned long previousMillis = 0;

TTN_esp32 ttn ;

void message(const uint8_t* payload, size_t size, int rssi)
{
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(size) + " bytes RSSI= " + String(rssi) + "dB");
    for (int i = 0; i < size; i++)  Serial.print(" " + String(payload[i]));
    Serial.println();
}


void setup() 
{
  pinMode(pLED, OUTPUT);        // set pin to output
  digitalWrite(pLED, LOW);      // turn off on-board LED

  pinMode(pResetOLED, OUTPUT);
  digitalWrite(pResetOLED, LOW);         // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(pResetOLED, HIGH);        // while OLED is running, must set GPIO16 in high
  
  display.init(); // Initialising the UI will init the display too.
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  Serial.begin(115200);
    
  ttn.begin();
  ttn.onMessage(message); // declare callback function when is downlink from server
  ttn.personalize(devAddr, nwkSKey, appSKey);
  ttn.showStatus();
   
  display.drawStringMaxWidth(0, 0, 128, "Starting LoRaWAN Node..." );
  display.display();
  delay(2000);           // wait for connecting terminal
  Serial.println(F("Starting LoRaWAN Node..."));

  initSensor();
}

void loop()
{
  digitalWrite(pLED, HIGH);      // turn on on-board LED
  getValues();

  int16_t  temp = (int) (sht31Temperature * 10. + .5);
  uint8_t  humi = (int) (sht31Humidity + .5); 

  char buffer[5];
  String s = dtostrf(sht31Temperature, 3, 1, buffer);
  display.clear();
  display.drawString(0, 0, "Temp = ");
  display.drawString(72, 0, s);
  display.drawString(104, 0, " °C");

  s = dtostrf(sht31Humidity, 3, 0, buffer);
  display.drawString(0, 16, "Hum  = ");
  display.drawString(72, 16, s);
  display.drawString(104, 16, " %");
  display.display();

  byte payload[3];

  payload[0] = highByte(temp);
  payload[1] = lowByte(temp);
  payload[2] = humi;

  Serial.println(F("Message queued"));
  display.drawString(0, 32, "Message queued" );
  display.display();
    
  // Send message
  ttn.sendBytes(payload, sizeof(payload));
  Serial.println(F("Message sent.\n"));
  display.drawString(0, 48, "Message sent."); display.display();
  delay(2000);
  digitalWrite(pLED, LOW);      // turn off on-board LED
  previousMillis = millis();
  while(millis() - previousMillis < CYCLE);  // wait CYCLE milliseconds
}
