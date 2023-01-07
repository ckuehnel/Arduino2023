/*
 * BME680 application program on WioTerminal using 
 * Bosch Sensortec Environmental Cluster (BSEC) Software v1.4.8.0 released on July 8th, 2020
 * The BSEC fusion library has been conceptualized to provide a higher-level signal processing and fusion for the BME680. 
 * The library receives compensated sensor values from the sensor API. 
 * It processes the BME680 signals to provide the requested sensor outputs.
 */

// Helper functions declarations
void checkIaqSensorStatus(void);
String output;

void initBME680()
{
  Wire.begin();
  
  //iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire); // SDO = CS = HIGH
  
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = 
  {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  if (DEBUG)
  {
    // Print the header
    output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
    Serial.println(output);
  }
}

void readBME680()
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run())   // If new data is available
  {
    output = String(time_trigger);
    output += ", " + String(iaqSensor.rawTemperature);
    output += ", " + String(iaqSensor.pressure);
    output += ", " + String(iaqSensor.rawHumidity);
    output += ", " + String(iaqSensor.gasResistance);
    output += ", " + String(iaqSensor.iaq);
    output += ", " + String(iaqSensor.iaqAccuracy);
    output += ", " + String(iaqSensor.temperature);
    output += ", " + String(iaqSensor.humidity);
    output += ", " + String(BME680iAQ=iaqSensor.staticIaq);
    output += ", " + String(BME680eCO2=iaqSensor.co2Equivalent);
    output += ", " + String(iaqSensor.breathVocEquivalent);
    if (DEBUG) Serial.println(output);
  } else checkIaqSensorStatus();
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      while(1);
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      while(1);
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}
