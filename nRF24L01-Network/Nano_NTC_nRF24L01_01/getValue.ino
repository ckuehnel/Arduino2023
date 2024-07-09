// Read temperatur sensor

long getValue(void)
{
  int a = analogRead(pinTempSensor);
 
  float R = 1023.0/a-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  temperature = temperature * 10. + .5;
  return (long) temperature;
}
