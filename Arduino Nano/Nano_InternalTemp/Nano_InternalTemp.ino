// Internal Temperature Sensor
// Example sketch for ATmega328 types.
// https://theorycircuit.com/arduino-internal-temperature-sensor/

String s = "";

void setup()
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  Serial.println(F("Internal Temperature Sensor"));
}

void loop()
{
  // Show the temperature in degrees Celcius.
  s = "ATmega328 Internal Temperatur: " + String(GetTemp(),1) + " *C";
  Serial.println(s);
  delay(1000);
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
