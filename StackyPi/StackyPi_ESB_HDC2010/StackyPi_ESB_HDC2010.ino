

/*
 * File: StackyPi_ESB_HDC2010.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * Temperature & Humidity Sensor HDC2010 connected to I2C0.
 * 
 * based on:
 * https://github.com/closedcube/ClosedCube_OPT3002_Arduino/blob/master/examples/opt3002demo/opt3002demo.ino
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <SPI.h>
#include <Wire.h>
#include <sSense-HDC2010.h>  // https://itbrainpower.net/downloadables/sSense-HDC2010.zip

// Coral ESB Pin Definitions

HDC2010 ssenseHDC2010(HDC2010_I2C_ADDR);

float temperature = 0, humidity = 0;

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  Serial.printf("done\n");

  ssenseHDC2010.begin();  // Initialize HDC2010(THS) I2C communication  
  ssenseHDC2010.reset();  // Begin with a HDC2010(THS) reset

  // Set up HDC2010(THS) temperature offset, if required
  //ssenseHDC2010.setTemperatureOffset(0b11010111);    //-6.64 degrees Celsius - determine and set your, see definitions and HDC2010 datasheet
 
  // Configure Measurements
  ssenseHDC2010.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
  ssenseHDC2010.setRate(ONE_HZ);                     // Set measurement frequency to 1 Hz
  ssenseHDC2010.setTempRes(FOURTEEN_BIT);
  ssenseHDC2010.setHumidRes(FOURTEEN_BIT);

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("The temperatur & humidity sensor HDC2010 is connected via I2C0..");

  delay(1000);

  ssenseHDC2010.triggerMeasurement();  //begin HDC2010 sensor measuring
}



void loop()
{
  temperature = ssenseHDC2010.readTemp();
  humidity = ssenseHDC2010.readHumidity();

  Serial.printf("Temp = %3.1f °C\tHum = %3.0f %%rH\n", temperature, humidity);

  delay(2000);
}
