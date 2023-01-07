/*
 * File: M5Atom_AtomDTU_NB-IoT_HTTP.ino   
 * 
 * based on https://github.com/m5stack/ATOM_DTU_NB/blob/master/examples/HTTP/HTTP.ino
 * 
 * Description: 
 * Use ATOM DTU NB-IoT to connect to 1NCE. 
 * Useed Board: M5Stack-ATOM
 * Used Libraries:
 * - FastLED: https://github.com/FastLED/FastLED
 * - M5Atom: https://github.com/m5stack/M5Atom
 */

#include "ATOM_DTU_NB.h"
#include "M5Atom.h"

ATOM_DTU_NB DTU;

String response;
String content;

const int MaxByteArraySize = 250;
byte byteArray[MaxByteArraySize] = {0};
char buffer[250];

int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;

void setup()
{
  M5.begin(true, true, true);
  Serial.println("M5Atom DTU NB-IoT Initialization...");
  M5.dis.drawpix(0, GRB_COLOR_GREEN);
  delay(200);
  
  DTU.Init(&Serial2, 19, 22);  // SIM7020
  
  //Reset Module
  Serial.println("NB-IoT Module Reset...");
  DTU.sendMsg("AT+CRESET\r\n");
  response = DTU.waitMsg(1000);
  Serial.println(response);
  
  Serial.println("Initialization finished.");
  M5.dis.clear(); delay(200);
}

void loop()
{
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

  DTU.sendMsg("AT+CHTTPCREATE=\"http://www.ckuehnel.ch/\"\r\n");  // Create a HTTP/HTTPS client instance 
  response = DTU.waitMsg(5000);
  Serial.print(response);

  DTU.sendMsg("AT+CHTTPCON=0\r\n");  // Establish the HTTP/HTTPS Connection 
  response = DTU.waitMsg(5000);
  Serial.print(response);

  
  DTU.sendMsg("AT+CHTTPSEND=0,0,\"/text.txt\"\r\n");  //HTTP GET
  response = DTU.waitMsg(5000);
  Serial.print(response);

  int start = response.indexOf("+CHTTPNMIC");
  //Serial.println(start);
  String content = response.substring(start + 12);
  Serial.print("Received from host: ");
  Serial.println(content);
  
  start = content.indexOf(',');
  content = content.substring(start + 1);
  //Serial.println(content);
  start = content.indexOf(',');
  content = content.substring(start + 1);
  //Serial.println(content);
  start = content.indexOf(',');
  content = content.substring(start + 1);
  //Serial.println(content);
  start = content.indexOf(',');
  content = content.substring(start + 1);
  Serial.print("Separated content:  ");
  Serial.print(content);
  int cl = content.length()-2;
  Serial.print("Content length = ");
  Serial.println(cl);

  char Buf[100] = {0};
  content.toCharArray(Buf, 100);
  Serial.print("Char Array:         ");
  Serial.print(Buf);

  Serial.print("Decoded Array:      ");
  for(int i = 0; i < cl; i += 2)
  {
    int decimal = 0, val, len = 1;

    for(int j=0; j < 2; j++)
    {
      /* Find the decimal representation of Buf[i] */
      if(Buf[i+j]>='0' && Buf[i+j]<='9')       val = Buf[i+j] - 48;
      else if(Buf[i+j]>='a' && Buf[i+j]<='f')  val = Buf[i+j] - 97 + 10;
      else if(Buf[i+j]>='A' && Buf[i+j]<='F')  val = Buf[i+j] - 65 + 10;
      
      decimal += val * pow(16, len);
      len--;
    }
    Serial.print(char(decimal));
  }
  Serial.println();
  
/*  
  if(response.indexOf("OK") != -1)
  {
    M5.dis.drawpix(0, 0x0000ff);
    content = "";
    while(Serial2.available());
    {
      Serial.print(Serial2.read());
    }
  }
  else M5.dis.drawpix(0, 0x00ff00);
*/   
 
  DTU.sendMsg("AT+CHTTPDISCON=0\r\n");  // Close the HTTP/HTTPS Connection
  response = DTU.waitMsg(1000);
  Serial.print(response);
  
  DTU.sendMsg("AT+CHTTPDESTROY=0\r\n"); // Destroy the HTTP/HTTPS Client Instance
  response = DTU.waitMsg(1000);
  Serial.print(response);

  M5.dis.clear();
  delay(30000);
}
