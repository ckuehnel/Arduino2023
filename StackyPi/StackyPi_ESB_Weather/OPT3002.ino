// Initialize OPT3002 sensor

void initOPT3002()
{
  opt3002.begin(OPT3002_ADDRESS);
  Serial.print("OPT3002 Manufacturer ID:");
  Serial.println(opt3002.readManufacturerID());
  Serial.print("Device ID:");
  Serial.println(opt3002.readDeviceID());

  configureSensor();
  printResult("High-Limit", opt3002.readHighLimit());
  printResult("Low-Limit", opt3002.readLowLimit());
}

void configureSensor() 
{
  OPT3002_Config newConfig;

  newConfig.RangeNumber = B1100;
  newConfig.ConvertionTime = B0;
  newConfig.Latch = B1;
  newConfig.ModeOfConversionOperation = B11;

  OPT3002_ErrorCode errorConfig = opt3002.writeConfig(newConfig);
  if (errorConfig != NO_ERROR)
    printError("OPT3002 configuration", errorConfig);
  else {
    OPT3002_Config sensorConfig = opt3002.readConfig();
    Serial.println("OPT3002 Current Config:");
    Serial.println("------------------------------");

    Serial.print("Conversion ready (R):");
    Serial.println(sensorConfig.ConversionReady, HEX);

    Serial.print("Conversion time (R/W):");
    Serial.println(sensorConfig.ConvertionTime, HEX);

    Serial.print("Fault count field (R/W):");
    Serial.println(sensorConfig.FaultCount, HEX);

    Serial.print("Flag high field (R-only):");
    Serial.println(sensorConfig.FlagHigh, HEX);

    Serial.print("Flag low field (R-only):");
    Serial.println(sensorConfig.FlagLow, HEX);

    Serial.print("Latch field (R/W):");
    Serial.println(sensorConfig.Latch, HEX);

    Serial.print("Mask exponent field (R/W):");
    Serial.println(sensorConfig.MaskExponent, HEX);

    Serial.print("Mode of conversion operation (R/W):");
    Serial.println(sensorConfig.ModeOfConversionOperation, HEX);

    Serial.print("Polarity field (R/W):");
    Serial.println(sensorConfig.Polarity, HEX);

    Serial.print("Overflow flag (R-only):");
    Serial.println(sensorConfig.OverflowFlag, HEX);

    Serial.print("Range number (R/W):");
    Serial.println(sensorConfig.RangeNumber, HEX);

    Serial.println("------------------------------");
  }
}

void printResult(String text, OPT3002 result) 
{
  if (result.error == NO_ERROR) 
  {
    Serial.print(text);
    Serial.print(": ");
    Serial.print(result.lux);
    Serial.println(" nW/cm2");
  }
  else printError(text, result.error);
}

void printError(String text, OPT3002_ErrorCode error) 
{
  Serial.print(text);
  Serial.print(": [ERROR] Code #");
  Serial.println(error);
}
