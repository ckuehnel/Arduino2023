/*
 * File: Nano_HC05_IntTemp.ino
 * 
 * Bluething sends internal temperature via BT
 * 
 */
#include <SoftwareSerial.h>
SoftwareSerial HC05(12, 11); // RX, TX
String s = "";

void flash(uint8_t count);

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println(F("\nBluething can be connected with DeviceName 'Bluething104'"));
  Serial.println(F("and sends internal temperature value every five seconds"));
  Serial.println(F("to console and via bluetooth.\n"));
  
  // set the data rate for the SoftwareSerial port
  HC05.begin(38400);
  // set to default 
  //HC05.println("AT+ORGL"); delay(500);
  // clear pairing list
  //HC05.println("AT+RMAAD"); delay(500);
  //set device name
  //HC05.println("AT+NAME=myBluething"); delay(500);
  // Setze Pin auf 4321
  //HC05.println("AT+PSWD=4321"); delay(500);
  // reset HC05
  //HC05.println("AT+RESET"); delay(1000);
  // initialize SPP profile library
  HC05.println("AT+INIT"); delay(500);
  HC05.println("AT+DISC"); delay(500);
  // setup finished
  flash(5);   
}

void loop() 
{
  // Show the temperature in degrees Celcius.
  flash(2);
  s = "ATmega328 Internal Temperatur: " + String(GetTemp(),1) + " *C";
  Serial.println(s);
  HC05.println(s);
  delay(5000);
}

void flash(uint8_t count)
{
  for (uint8_t i = 0; i<count; i++)
  {
    digitalWrite(LED_BUILTIN, 1);
    delay(50);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);  
  }
}

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used with the internal reference of 1.1V.
  // Channel 8 can not be selected with the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}
