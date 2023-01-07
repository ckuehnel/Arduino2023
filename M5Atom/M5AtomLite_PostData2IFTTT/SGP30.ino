// Read data from SGP30 sensor

/* return absolute humidity [mg/m^3] with approximation formula
 * @param temperature [°C]
 * @param humidity [%RH]
 */
uint32_t getAbsoluteHumidity(float temperature, float humidity) 
{
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}

void initSGP30()
{
  if (!sgp.begin())
  {
    Serial.println("No SGP30 sensor found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
}

void readSGP30()
{
  // temperature & humidity from SCD30
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
  
  if (!sgp.IAQmeasure()) 
  {
    Serial.println("Measurement failed");
    return;
  }
  TVOC = sgp.TVOC;
  eCO2 = sgp.eCO2;
  
  Serial.print("SGP30 values:\t");
  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
   Serial.print("eCO2 "); Serial.print(eCO2); Serial.print(" ppm\n");
  
  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  // value are for information only and will not be used for compare
  //Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  //Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
}
