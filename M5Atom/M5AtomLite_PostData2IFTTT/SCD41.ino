// Read data from SCD41 sensor
// Based on https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/examples/exampleUsage/exampleUsage.ino

void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("SCD41 Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}



void initSCD41()
{
  uint16_t error;
  char errorMessage[256];

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
      Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error) {
      Serial.print("Error trying to execute getSerialNumber(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
  } else {
      printSerialNumber(serial0, serial1, serial2);
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
}


void readSCD41()
{
  uint16_t error;
  char errorMessage[256];

  // Read Measurement
  uint16_t co2;
  uint16_t temperature;
  uint16_t humidity;
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else if (co2 == 0) {
    Serial.println("Invalid sample detected, skipping.");
  } else {
    Serial.print("SCD41 values:\t");
    Serial.print("        \t");
    Serial.print("CO2  "); Serial.print(co2); Serial.print(" ppm\t");
    Serial.print("Temp "); Serial.print(temperature * 175.0 / 65536.0 - 45.0,1); Serial.print(" *C\t");
    Serial.print("rHum "); Serial.print(humidity * 100.0 / 65536.0,0); Serial.println(" %rH");
    SCD41CO2 = co2;
  } 
}
