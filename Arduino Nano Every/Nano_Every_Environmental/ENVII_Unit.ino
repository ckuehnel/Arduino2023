// ENVII Unit contains SHT30 and BMP280
// Pressure data are not of interest here.
// Therefore, only SHT30 data are used.

void initENVII()
{
  if (! sht31.begin(0x44)) 
  { // Set to 0x45 for alternate i2c addr
    if (DEBUG) Serial.println("No SHT30 found.");
    while (1) delay(1);
  }
  else u8g2.drawStr(0,25, "SHT30 connected.");
}

void getENVIIData()
{
  temp = sht31.readTemperature();
  humi = sht31.readHumidity();

  if (!isnan(temp))   // check if 'is not a number'
  {
    if (DEBUG) Serial.print("Temp = "); Serial.print(temp,1); Serial.print(" *C\t\t");
  }
  else  
    if (DEBUG) Serial.println("Failed to read temperature");
  
  if (!isnan(humi))   // check if 'is not a number'
  {
    if (DEBUG) Serial.print("Hum. = "); Serial.print(humi,1); Serial.println(" %rH");
  }
  else  
    if (DEBUG) Serial.println("Failed to read humidity");
}
