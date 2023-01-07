// Read data from SCD30 sensor

void initSCD30()
{
  if (!scd.begin())
  {
    Serial.println("No SCD30 sensor found :(");
    while (1);
  }
  else Serial.println("Found SCD30");
  
  //The SCD30 has data ready every two seconds
}

void readSCD30()
{
  if (scd.dataAvailable())
  {
    CO2 = scd.getCO2();
    temperature = scd.getTemperature();
    humidity = scd.getHumidity();
    
    Serial.print("SCD30 values:\t");
    Serial.print("        \t");
    Serial.print("CO2  "); Serial.print(CO2); Serial.print(" ppm\t");
    Serial.print("Temp "); Serial.print(temperature,1); Serial.print(" *C\t");
    Serial.print("rHum "); Serial.print(humidity,0); Serial.println(" %rH");
  }
  else Serial.println("Waiting for new data");
}
