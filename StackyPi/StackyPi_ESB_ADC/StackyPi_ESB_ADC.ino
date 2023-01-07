/*
 * File: StackyPi_ESB_ADC.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * ADC TLA2021 connected to I2C0.
 * 
 * based on:
 * https://github.com/adafruit/Adafruit_TLA202x/blob/master/examples/read_single/read_single.ino
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_TLA202x.h> // https://github.com/adafruit/Adafruit_TLA202x

// Coral ESB I2C Addr Definition
const uint8_t TLA2021_ADDR = 0x49;

Adafruit_TLA202x  tla;

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  Serial.printf("done\n");
  Serial.printf("ADC TLA2021 initialization...\n");
  initADC();
  Serial.printf("Initialization done.\n");

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("The ADC TLA2021 measures voltages between AIN0 and GND.");
}

void loop()
{
  Serial.printf("Channel 0: %3.3f V\n", tla.readOnce(TLA202x_CHANNEL_0));
  delay(2000);
}
