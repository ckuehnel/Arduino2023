/*
   File: StackyPi_Blink.ino

Test StackyPi environment by the standard blink program
Blue LED is connected to GPIO 25

Base: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink

2022-04-27 Claus Kühnel info@ckuehnel.ch
*/

#define LED_BUILTIN 25

void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on for short time
  delay(100);                        // wait for 100 ms
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off for longer time
  delay(900);                        // wait for 900 ms
}