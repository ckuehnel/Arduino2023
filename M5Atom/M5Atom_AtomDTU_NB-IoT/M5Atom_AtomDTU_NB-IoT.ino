/*
 * File: M5Atom_AtomDTU_NB-IoT.ino   
 * 
 * based on https://github.com/m5stack/ATOM_DTU_NB/blob/master/examples/HTTP/HTTP.ino
 * 
 * Description: 
 * Use ATOM DTU NB-IoT to connect to 1NCE. 
 * Used board: M5Stick-C
 * Used libraries:
 * - FastLED: https://github.com/FastLED/FastLED
 * - M5Atom: https://github.com/m5stack/M5Atom
 */
#define DEBUG 1

#include "ATOM_DTU_NB.h"
#include "M5Atom.h"

ATOM_DTU_NB DTU;

String response;

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

const long CYCLE = 15 * 60000; // Transmission cycle 15 minutes
unsigned long previousMillis = 0;

char payload[] = "";
char buffer[2];


void setup()
{
  //M5.begin(true, true, true);
  M5.begin();
  Serial.println("Initialization...");
  Wire.begin(26, 32, 100000);    // Atom Lite I2C GPIO Pin is 26 and 32 
//  initSensor();          // ENV.II UNit
  
  //SIM7020
  DTU.Init(&Serial2, 19, 22);
  //Reset Module
  Serial.print("Module Reset.....");
  DTU.sendMsg("AT+CRESET\r\n");
  response = DTU.waitMsg(1000);
  Serial.print(response);
  M5.dis.drawpix(0, 0xff0000);
}

void loop()
{
//  prepareMessage();
  //char buffer[] = "4d35535441434b";
  int len = sizeof(buffer);
  

  delay(5000);
}

void prepareMessage()
{
  //getValues(); // get measuring results of ENV.II sensor

  // prepare the measuring results for use in payload
  // you can enhance payload w/ BMP280 data, if you want
  int16_t temp = 2220; //(int16_t) (sht31Temperature * 100. + .5);
  uint8_t temp_hi = highByte(temp); uint8_t temp_lo = lowByte(temp);
  uint8_t humi = 50; //(uint8_t) (sht31Humidity + .5);
  itoa(temp_hi, buffer, 16); strcat(payload, buffer);
  itoa(temp_lo, buffer, 16); strcat(payload, buffer);
  itoa(humi, buffer, 16); strcat(payload, buffer);
  
}
