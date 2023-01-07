/*
 * File: Tiny2040_RGB.ino
 * 
 * This example shows how to control the onboard LEDs at Tiny2040  
 * 2021-04-06 Claus Kühnel info@ckuehnel.ch
 */

// Pins for RGB LED @ Tiny2040
const int LEDR = 18;
const int LEDG = 19;
const int LEDB = 20;

const int TIME = 1000;

void setup() 
{
  // declare LED pins to be an output:
  pinMode(LEDR, OUTPUT); digitalWrite(LEDR, 1);
  pinMode(LEDG, OUTPUT); digitalWrite(LEDG, 1);
  pinMode(LEDB, OUTPUT); digitalWrite(LEDB, 1);
}

void loop() 
{
  analogWrite(LEDR, 0);
  delay(TIME);
  analogWrite(LEDR, 255);
  analogWrite(LEDG, 175);
  delay(TIME);
  analogWrite(LEDG, 255);
  analogWrite(LEDB, 0);
  delay(TIME);
  analogWrite(LEDB, 255);
}
