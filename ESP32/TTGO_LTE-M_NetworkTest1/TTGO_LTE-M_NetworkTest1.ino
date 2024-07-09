/*
 * File: TTGO_LTE-M_NetworkTest.ino
 * 
 * based on https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/examples/Arduino_NetworkTest/Arduino_NetworkTest.ino
 */

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
//#define DUMP_AT_COMMANDS

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "iot.1nce.net";     //SET TO YOUR APN
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define uS_TO_S_FACTOR      1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP       60          // Time ESP32 will go to sleep (in seconds)

#define UART_BAUD           9600
#define PIN_DTR             25
#define PIN_TX              27
#define PIN_RX              26
#define PWR_PIN             4

#define SD_MISO             2
#define SD_MOSI             15
#define SD_SCLK             14
#define SD_CS               13
#define LED_PIN             12
#define BAT_ADC             35

uint16_t count = 0;  // Counter variable

void enableGPS(void)
{
    // Set SIM7000G GPIO4 LOW ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem.sendAT("+SGPIO=0,4,1,1");
    if (modem.waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,1 false ");
    }
}

void disableGPS(void)
{
    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem.sendAT("+SGPIO=0,4,1,0");
    if (modem.waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,0 false ");
    }
}

void modemPowerOn()
{
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, LOW);
    delay(1000);    //Datasheet Ton mintues = 1S
    digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff()
{
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, LOW);
    delay(1500);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PWR_PIN, HIGH);
}


void modemRestart()
{
    modemPowerOff();
    delay(1000);
    modemPowerOn();
}

void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);
    delay(1000); // wait for serial monitor
    Serial.println("\nLTE-M Network Test for LilyGO-T-SIM7000G w/ ESP32-Wrover");

    // Set LED OFF
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    modemPowerOn();

    // no card during program upload from Arduino IDE
    Serial.println("SDCard Detection...");
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS)) {
        Serial.println("SDCard MOUNT FAIL");
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "SDCard Size: " + String(cardSize) + "MB";
        Serial.println(str);
    }
    Serial.println();
    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
    delay(2000);
}

void loop()
{
  String res;

  Serial.println("Modem Initialization...");
  if (!modem.init()) 
  {
    modemRestart();
    delay(2000);
    Serial.println("Failed to restart modem, attempting to continue without restarting");
    return;
  }

  modem.sendAT("+SIMCOMATI");
  modem.waitResponse(1000L, res);
  res.replace(GSM_NL "OK" GSM_NL, "");
  Serial.println(res);
 
  res = "";
  Serial.println();
  Serial.print("Preferred mode selection...");
  modem.sendAT("+CNMP?");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  
  res = "";
  Serial.print("Preferred selection between CAT-M and NB-IoT...");
  modem.sendAT("+CMNB?");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  String name = modem.getModemName();
  Serial.println("Modem Name: " + name);

  String modemInfo = modem.getModemInfo();
  Serial.println("Modem Info: " + modemInfo);

  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) modem.simUnlock(GSM_PIN);

  /*
    2 Automatic
    13 GSM only
    38 LTE only
    51 GSM and LTE only
  */
  Serial.println("Set Network Mode to LTE-M");
  res = modem.setNetworkMode(38);

  /*
    1 CAT-M
    2 NB-Iot
    3 CAT-M and NB-IoT
  */
  Serial.println("Set Preffered Mode to Cat-M1");
  res = modem.setPreferredMode(1);

  digitalWrite(LED_PIN, LOW);
  bool isConnected = false;
  int tryCount = 60;
  while (tryCount--) 
  {
    int16_t signal =  modem.getSignalQuality();
    Serial.print("Signal: ");
    Serial.println(signal);
    Serial.print("Network connected? ");
    isConnected = modem.isNetworkConnected();
    Serial.println( isConnected ? "Connected" : "Not Connected");
    if (isConnected) break;
  }
  digitalWrite(LED_PIN, HIGH);

  Serial.println();
  Serial.println("Device is connected.");
  Serial.println();

  res = "";
  Serial.println("Inquiring UE system information...");
  modem.sendAT("+CPSI?");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Open wireless connection...");
  modem.sendAT("+CNACT=1,\"iot.1nce.net\"");
  if (modem.waitResponse(5000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Get local IP...");
  modem.sendAT("+CNACT?");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Set up server URL...");
  modem.sendAT("+SMCONF=\"URL\",\"broker.hivemq.com\"");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Set up ClientID...");
  modem.sendAT("+SMCONF=\"CLIENTID\",\"LTE\"");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  
  res = "";
  Serial.println("Set up QoS...");
  modem.sendAT("+SMCONF=\"QOS\",1");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Read MQTT Parameter...");
  modem.sendAT("+SMCONF?");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Connecting MQTT...");
  modem.sendAT("+SMCONN");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  char str[10]; sprintf(str, "%5d", count);
  char msg[30] = "Count = "; strcat(msg,str);
  Serial.printf("Count = %d\n", count);

  float Vbat = readBattery(BAT_ADC);
  Serial.printf("Vbat  = %5.0f mV\n", Vbat);
  strcat(msg,"; Vbat = ");
  sprintf(str, "%5.0f", Vbat); strcat(msg,str);
  strcat(msg, " mV");
  Serial.printf("%s\n", msg);
  
  size_t len = strlen(msg);
  Serial.printf("Message length = %d\n", len);
  
  res = "";
  Serial.println("Send packet...");
  modem.sendAT("+SMPUB=\"update\",\"30\",1,1");
  delay(10);
  SerialAT.println(msg); 
  if (modem.waitResponse(2000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
   
  res = "";
  Serial.println("Disconnect MQTT...");
  modem.sendAT("+SMDISC");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  res = "";
  Serial.println("Disconnect wireless...");
  modem.sendAT("+CNACT=0");
  if (modem.waitResponse(1000L, res) == 1) 
  {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  digitalWrite(LED_PIN, HIGH);
  count++;
  delay(10000);
}
