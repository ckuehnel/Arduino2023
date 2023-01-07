/*
 * File: M5Stack_AtomDTU_NB-IoT_MQTT.ino
 * 
 * Based on: https://github.com/m5stack/ATOM_DTU_NB/tree/master/examples/MQTT
 * 
 * Use ATOM DTU NB to connect to the MQTT server, and implement subscription and publishing messages.
 * Check the status through Serial. When the MQTT connection is successful, ENV.II data will be published.
 * Please install library before compiling:  
 * - FastLED: https://github.com/FastLED/FastLED
 * - M5Atom: https://github.com/m5stack/M5Atom
 */


#include "ATOM_DTU_NB.h"
#include "M5Atom.h"

#define DEBUG 1

ATOM_DTU_NB DTU;

int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;

String response;

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

unsigned int i, j;
String msg, msglen, publish;

void setup()
{
  M5.begin(true, true, true);
  Wire.begin(26,32);
  Serial.println("M5Atom DTU NB-IoT Initialization...");
  M5.dis.drawpix(0, GRB_COLOR_GREEN);
  delay(200);
  
  DTU.Init(&Serial2, 19, 22);  // SIM7020
  
  //Reset Module
  Serial.println("NB-IoT Module Reset...");
  DTU.sendMsg("AT+CRESET\r\n");
  response = DTU.waitMsg(1000);
  Serial.println(response);

  initSensor();
  
  Serial.println("Initialization finished.");
  M5.dis.clear(); delay(200);

  DTU.sendMsg("AT+CSMINS?\r\n"); // SIM Inserted Status Reporting available?
  response = DTU.waitMsg(1000);
  Serial.print(response);

  while(1)
  {
    M5.dis.drawpix(0, GRB_COLOR_BLUE);
    DTU.sendMsg("AT+CSQ\r\n"); //  Signal Quality Report
    response = DTU.waitMsg(1000);
    Serial.print(response);
    if(response.indexOf("0,0") == -1 && response.indexOf("99") == -1 )
    {
      break;
    }
    M5.dis.clear(); delay(200);
  }

  M5.dis.drawpix(0, GRB_COLOR_BLUE);
  DTU.sendMsg("AT+CREG?\r\n");  // Network Registration
  response = DTU.waitMsg(1000);
  Serial.print(response);

  DTU.sendMsg("AT+COPS?\r\n");  // Operator Selection 
  response = DTU.waitMsg(1000);
  Serial.print(response);

  //Create MQTT connection
  //If succeed, MQTT id will return.
  DTU.sendMsg("AT+CMQNEW=\"broker.mqttdashboard.com\",\"1883\",12000,1024\r\n");
  response = DTU.waitMsg(5000);
  Serial.print(response);

  DTU.sendMsg("AT+CMQCON=0,3,\"myclient\",600,1,0\r\n");
  response = DTU.waitMsg(5000);
  Serial.print(response);

  DTU.sendMsg("AT+CREVHEX=0\r\n");
  response = DTU.waitMsg(1000);
  Serial.print(response);  
  M5.dis.clear();
}

void loop()
{
  M5.dis.drawpix(0, GRB_COLOR_BLUE);
  
  getValues();

  i = j++ % 3;
  
  switch (i) 
  {
    case 0:
      msg = String(sht31Temperature,1); msglen = String(msg.length());
      publish = String("AT+CMQPUB=0,\"atomdtu/sht31Temperature\",1,0,0,");
      publish += msglen + String(",\"") + msg + String("\"\r\n");
      break;
    case 1:
      msg = String(sht31Humidity,0); msglen = String(msg.length());
      publish = String("AT+CMQPUB=0,\"atomdtu/sht31Humidity\",1,0,0,");
      publish += msglen + String(",\"") + msg + String("\"\r\n");
      break;
    case 2:
      msg = String(bmp280Pressure,0); msglen = String(msg.length());
      publish = String("AT+CMQPUB=0,\"atomdtu/bmp280Pressure\",1,0,0,");
      publish += msglen + String(",\"") + msg + String("\"\r\n");
      break;
    default:
      msg = String(sht31Temperature,1); msglen = String(msg.length());
      publish = String("AT+CMQPUB=0,\"atomdtu/sht31Temperature\",1,0,0,");
      publish += msglen + String(",\"") + msg + String("\"\r\n");
      break;
  }
  DTU.sendMsg(publish);
  response = DTU.waitMsg(5000);
  Serial.print(response);
  if(response.indexOf("ERR") !=-1) ESP.restart();
 
  M5.dis.clear();
  delay(55000); // resulting cycle time approx. 1 minute
}
