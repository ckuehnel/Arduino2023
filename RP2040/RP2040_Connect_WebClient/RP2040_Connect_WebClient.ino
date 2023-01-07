/*
 * RP2040_Connect_WebClient.ino
 * 
 * This sketch connects to my website (http://ckuehnel.ch) using the WiFi module.
 * This example is written for a network using WPA encryption. For 
 * WEP or WPA, change the Wifi.begin() call accordingly.
 * 
 * Circuit: Arduino Nano RP2040 Connect
 * 
 * based on WiFiWebClient.ino by Tom Igoe
 * 
 * 2022-01-28 Claus Kühnel info@ckuehnel.ch
 */
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;

char server[] = "www.ckuehnel.ch";    // name address for ckuehnel.ch (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

WiFiClient client;

void setup() 
{
  Serial.begin(9600);
  delay(2000);  // wait for serial monitor
  Serial.println("Arduino Nano RP2040 Connect WebClient");

  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  Serial.print("Actual Firmware Version: "); Serial.println(fv);
  Serial.print("Latest Firmware Version: "); Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    Serial.println("Please upgrade the firmware");

  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID);

    status = WiFi.begin(SSID, PASS);
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) 
  {
    Serial.println("connected to server");

    // Make a HTTP request:
    client.println("GET /text.txt HTTP/1.1");
    client.println("Host: www.ckuehnel.ch");
    client.println("Connection: close");
    client.println();
  }
}

void loop() 
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    while (true);
  }
}

void printWifiStatus() 
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
