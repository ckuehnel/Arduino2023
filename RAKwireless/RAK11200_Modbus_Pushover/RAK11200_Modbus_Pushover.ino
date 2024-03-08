/*
 * File: RAK11200_Modbus_Pushover.ino
 * 
 * Program for reading Modbus sensor and sending pushover messages
 * 
 * 2023-07-16 Claus Kühnel info@ckuehnel.ch
 */
#include <WiFi.h>
#include <ModbusRTU.h>        // https://github.com/emelianov/modbus-esp8266
#include "arduino_secrets.h"

WiFiClient client;
ModbusRTU mb;

#define SLAVE_ID 1      // Modbus device address
#define FIRST_REG 0
#define REG_COUNT 2

float temperature, humidity = 0.;

unsigned long previousMillis = 0;       
unsigned long interval = 15 * 60000;       //use 3600000 for one hour
boolean first = true;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) 
{
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void setup() 
{
  // WisBLOCK RAK5802 Power On
  pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);

	Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.printf("\nRAK11200 reads Modbus sensor and sends Pushover messages to mobile phone...\n");
  initModbus();
  initWiFi();
  pinMode(LED_BLUE, OUTPUT); digitalWrite(LED_BLUE, LOW);
} 
  
void loop() 
{ 
  unsigned long currentMillis = millis();
  
  if ((currentMillis - previousMillis >= interval) || first) 
  {
    previousMillis = currentMillis;
    first = false;

    digitalWrite(LED_BLUE, HIGH);
    readModbus();
    sendMessage();
    digitalWrite(LED_BLUE, LOW);
  }
}

