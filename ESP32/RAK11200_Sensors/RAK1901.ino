// RAK1901

boolean initRAK1901(void)
{
  if (sht.init()) 
  {
       //Serial.print("init(): success\n");
       sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
       RAK1901_ready = true;
  } 
  else Serial.println("RAK1901 not detected");

  return RAK1901_ready;
}

void readRAK1901(void)
{
  if (sht.readSample()) 
  {
    temp = sht.getTemperature();
    humi = sht.getHumidity();
   
    Serial.printf("RAK1901 : Temp = %3.1f °C, Humi = %3.0f %%rH\n", temp, humi);
  } 
  else Serial.println("Error in readSample()");
}
