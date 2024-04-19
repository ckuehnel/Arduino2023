/*
 * The access to SHTC3 Sensor Unit is organized here.
 * The SHTC3 Sensor measures temperature & humidity.
 * The results are saved in global float variable.
 * 
 * 2024-04-17 Claus Kühnel info@ckuehnel.ch
 */
 
#include <Wire.h>
#include <SHTSensor.h>
#include <arduino-sht.h>

SHTSensor sht;

void initSensor()
{
  if (sht.init()) Serial.print("init(): success\n");
  else            Serial.print("init(): failed\n");
}

void getValues()
{
  if (!sht.readSample()) 
  {
    Humidity = 55.555; //sht.getHumidity();
    Temperature = 33.333; //sht.getTemperature();
    Serial.printf("Temperature = %5.2f °C\tHumidity = %5.1f %%rH\n", Temperature, Humidity);
  } else Serial.printf("Error in readSample()\n");
}