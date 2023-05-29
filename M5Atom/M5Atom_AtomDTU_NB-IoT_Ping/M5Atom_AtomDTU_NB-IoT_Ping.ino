/*
 * File: M5Atom_AtomDTU_NB-IoT_Ping.ino   
 * 
 * based on https://github.com/m5stack/ATOM_DTU_NB/blob/master/examples/HTTP/HTTP.ino
 * 
 * Description: 
 * Use ATOM DTU NB-IoT to connect to 1NCE. 
 * Useed Board: M5Stack-ATOM
 * Used Libraries:
 * - FastLED: https://github.com/FastLED/FastLED
 * - M5Atom: https://github.com/m5stack/M5Atom
 * 
 * 2022-01-12 Claus Kühnel info@ckuehnel.ch
 */

#include "ATOM_DTU_NB.h"
#include <M5Atom.h>

ATOM_DTU_NB DTU;

String response;

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
  
  DTU.Init(&Serial2, 19, 22);  // Initialize SIM7020
  
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


  DTU.sendMsg("AT+CGACT=1\r\n");  // Activate PDP context
  response = DTU.waitMsg(1000);
  Serial.print(response);

  DTU.sendMsg("AT+CIPPING=\"www.google.com\"\r\n"); // Ping Google server
  response = DTU.waitMsg(15000);
  Serial.print(response);

  M5.dis.clear();
  delay(30000);
}
