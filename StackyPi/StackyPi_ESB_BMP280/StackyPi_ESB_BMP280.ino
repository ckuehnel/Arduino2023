/*
 * File: StackyPi_ESB_BMP280.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * Barometric Pressure Sensor BMP280 connected to I2C0.
 * 
 * based on:
 * https://bitbucket.org/christandlg/bmx280mi/src/master/examples/BMx280_I2C/BMx280_I2C.ino
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <SPI.h>
#include <Wire.h>
#include <BMx280I2C.h>  // https://bitbucket.org/christandlg/bmx280mi/src/master/

// Coral ESB Pin Definitions

#define I2C_ADDRESS 0x76

//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(I2C_ADDRESS);

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  Serial.printf("done\n");

  //begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
  //and reads compensation parameters.
  if (!bmx280.begin())
  {
    Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
    while (1);
  }

  if (bmx280.isBME280())
    Serial.println("Sensor is a BME280");
  else
    Serial.println("Sensor is a BMP280");

  //reset sensor to default parameters.
  bmx280.resetToDefaults();

  //by default sensing is disabled and must be enabled by setting a non-zero
  //oversampling setting.
  //set an oversampling setting for pressure and temperature measurements. 
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  //if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280()) bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("The barometric pressure sensor BMP280 is connected via I2C0..");
}



void loop()
{
  //start a measurement
  if (!bmx280.measure())
  {
    Serial.println("could not start measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish
  do
  {
    delay(100);
  } while (!bmx280.hasValue());

  Serial.printf("Pressure: %3.0f hPa\t", bmx280.getPressure()/100);
  Serial.printf("Pressure (64 bit): %6.1f hPa\t", bmx280.getPressure64()/100);
  Serial.printf("Temp: %3.1f °C\t", bmx280.getTemperature());

  //important: measurement data is read from the sensor in function hasValue() only. 
  //make sure to call get*() functions only after hasValue() has returned true. 
  if (bmx280.isBME280())
  {
    Serial.printf("Hum: %3.0f", bmx280.getHumidity());
  }
  Serial.printf("\n");

  delay(2000);
}
