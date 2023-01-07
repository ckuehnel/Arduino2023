// Read data from K30 sensor

void initK30()
{
  int co2 = 0, rc  = 1;
  
  rc = k30_i2c.readCO2(co2);
  if (rc == 0) Serial.println("K30 sensor operates");
  else 
  {
    Serial.print("Failure to read K30 sensor -  Error: ");
    Serial.println(rc);  
  } 
}

void readK30()
{
  int co2 = 0, rc  = 1;

  rc = k30_i2c.readCO2(co2);
  if (rc == 0)
  {
    Serial.print("K30 value:\t");
    Serial.print("          \t");
    Serial.print("CO2  "); Serial.print(co2); Serial.print(" ppm\n");
    K30CO2 = co2;
  } 
  else 
  {
    Serial.print("Failure to read sensor -  Error: ");
    Serial.println(rc);  
  }
}
