/*
 * File: LilyGo_EPD102_SensorNode.ino
 * 
 * 
 */
#define LILYGO_EPD_DISPLAY_102  // T-Dispay E-paper 1.02" @ https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include "boards.h"
#include <GxEPD.h>
#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

#define SLEEP (1000 * 1000 * 60)  // Sleeptime 60 sec
#define ID 1      // this ID must be unique for the connected sensors

float Temperature, Humidity, volt;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct EspNowMessage 
{
    uint8_t  id;
    uint16_t battery;
    float    Temp;
    uint8_t  Humi;
} EspNowMessage;

// Create a struct_message called message
EspNowMessage message;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t gatewayAddress[] = {0xF0, 0x08, 0xD1, 0x80, 0xE2, 0xCC};

// Create peer interface
esp_now_peer_info_t peerInfo;

/*
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
*/


void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.printf("\nLilyGo PD102 SensorNode\n");
  Serial.printf("Sensor used Sensirion SHTC3\n");

  Serial.print("ESP Board MAC Address = ");
  Serial.println(WiFi.macAddress());

  initDisplay();
  
  //initSensor(); // initialize SHTC3 sensor

  volt = analogRead(34) * 6.6 /4096;
  Serial.printf("Battery = %5.3f V\t", volt);
 
  getValues(); // get measuring results of SHTC3 sensor
  displayValues();

  message.id = ID;   
  message.battery = (uint16_t) (volt * 1000 + .5);
  message.Temp = Temperature;
  message.Humi = (uint8_t) (Humidity + .5);


  // Set device as a WiFi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
//  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, gatewayAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(gatewayAddress, (uint8_t *) &message, sizeof(message));
   
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  btStop();
  //adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  Serial.printf("Going to sleep for %d seconds\n", SLEEP/1000/1000);

  esp_sleep_enable_timer_wakeup(SLEEP);
  esp_deep_sleep_start();
}


void loop()
{
  delay(1000);
}