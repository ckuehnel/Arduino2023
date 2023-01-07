

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
    if (DEBUG) Serial.println("No SGP30 found.");
    while(1);
  }
  else u8g2.drawStr(0,35, "SGP30 connected.");
}

void getSGP30Data()
{
  sgp.setHumidity(getAbsoluteHumidity(temp, humi));
  
  if (!sgp.IAQmeasure()) 
  {
    if (DEBUG) Serial.println("Measurement failed");
    return;
  }
  else
  {
    TVOC = sgp.TVOC;
    eCO2 = sgp.eCO2; 
  }
  if (DEBUG)
  {
    Serial.print("TVOC = "); Serial.print(TVOC); Serial.print(" ppb\t\t");
    Serial.print("eCO2 = "); Serial.print(eCO2); Serial.println(" ppm");
  }
}
