/*
 * File: StackyPi_ESB_IO.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * On-board LED ist connected to GPIO15.
 * Button is connected to GPIO27
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <arduino-timer.h>            // https://github.com/contrem/arduino-timer

auto timer = timer_create_default(); // create a timer with default settings

// Coral ESB Pin Definitions
#define ESB_LED 15
#define ESB_Button 27

boolean toggle_led(void *) 
{
  digitalWrite(ESB_LED, !digitalRead(ESB_LED));
  return true; 
}

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);
  
  pinMode(ESB_LED, OUTPUT); 
  pinMode(ESB_Button, INPUT_PULLUP);

  // call the toggle_led function every 1000 millis (1 second)
  timer.every(1000, toggle_led);

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("The on-board LED of ESB blinks ones per second.");
  Serial.println("Press the on-board button for digital input.");

  for (int i = 0; i < 10; i++)
  {
    Serial.println(digitalRead(ESB_Button));
    delay(1000);
  }
  Serial.println("Restart to repeat");
}

void loop() 
{
  timer.tick(); // tick the timer
}
