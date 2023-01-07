// Initialize HDC2010 sensor

void initHDC2010(void)
{
  ssenseHDC2010.begin();  // Initialize HDC2010(THS) I2C communication  
  ssenseHDC2010.reset();  // Begin with a HDC2010(THS) reset

  // Set up HDC2010(THS) temperature offset, if required
  //ssenseHDC2010.setTemperatureOffset(0b11010111);    //-6.64 degrees Celsius - determine and set your, see definitions and HDC2010 datasheet
 
  // Configure Measurements
  ssenseHDC2010.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
  ssenseHDC2010.setRate(ONE_HZ);                     // Set measurement frequency to 1 Hz
  ssenseHDC2010.setTempRes(FOURTEEN_BIT);
  ssenseHDC2010.setHumidRes(FOURTEEN_BIT);
  delay(1000);
  ssenseHDC2010.triggerMeasurement();  //begin HDC2010 sensor measuring
  delay(100);
  HDC_temp = ssenseHDC2010.readTemp();  // read first result and throw away
  HDC_humi = ssenseHDC2010.readHumidity();
}
