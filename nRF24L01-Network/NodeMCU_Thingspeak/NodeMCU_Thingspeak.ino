/*
 * File: NodeMCU_Thingspeak.ino
 * 
 * Sending values to a ThingSpeak channel
 * using the ThingSpeak API (https://www.mathworks.com/help/thingspeak).
 * Used libraries:
 * - esp8266 by ESP8266 Community
 * - ThingSpeak by MathWorks
 
 ThingSpeak Setup:
 
   * Sign Up for New User Account - https://thingspeak.com/users/sign_up
   * Create a new Channel by selecting Channels, My Channels, and then New Channel
   * Enable one field
   * Enter SECRET_CH_ID in "secrets.h"
   * Enter SECRET_WRITE_APIKEY in "secrets.h"

 Setup Wi-Fi:
  * Enter SECRET_SSID in "secrets.h"
  * Enter SECRET_PASS in "secrets.h"
  
 Tutorial: http://nothans.com/measure-wi-fi-signal-levels-with-the-esp8266-and-thingspeak
   
 Created: Feb 3, 2017 by Hans Scharler (http://nothans.com)
*/

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "arduino_secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;



char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() 
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) 
  { 
    WiFi.begin(ssid, pass);
    Serial.print("\nAttempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println("\nConnected.");
  }

  // Measure Analog Input (A0)
  int valueA0 = (int) random(100, 110); //analogRead(A0);
  Serial.print("Send value: "); Serial.println(valueA0);

  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, valueA0, myWriteAPIKey);

  if (httpCode == 200) Serial.println("Channel write successful.");
  else                 Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));

  // Wait 60 seconds to update the channel again
  delay(60000);
}
