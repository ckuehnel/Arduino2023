// Read ADC value for battery voltage and charge.
// Raed SHTC3 for temperature and humidity inside enclosure

uint16_t batteryVoltage;
uint8_t  batteryCharge;
uint16_t insideTemperature;
uint8_t  insideHumidity;

#define PIN_VBAT WB_A0

uint32_t vbat_pin = PIN_VBAT;

#define VBAT_MV_PER_LSB (0.73242188F) // 3.0V ADC range and 12 - bit ADC resolution = 3000mV / 4096
#define VBAT_DIVIDER_COMP (1.73)      // Compensation factor for the VBAT divider, depend on the board

#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

// Get RAW Battery Voltage
uint16_t readVBAT(void)
{
  float raw;

  // Get the raw 12-bit, 0..3000mV ADC value
  raw = analogRead(vbat_pin);

  return (uint16_t) raw * REAL_VBAT_MV_PER_LSB;
}

// Convert from raw mv to percentage

uint8_t mvToPercent(float mvolts)
{
  if (mvolts < 3300) return 0;
  if (mvolts < 3600)
  {
    mvolts -= 3300;
    return mvolts / 30;
  }

  mvolts -= 3600;
  return 10 + (mvolts * 0.15F); // thats mvolts /6.66666666
}

void init_adc(void)
{
  // Set the analog reference to 3.0V (default = 3.6V)
  analogReference(AR_INTERNAL_3_0);

  // Set the resolution to 12-bit (0..4095)
  analogReadResolution(12); // Can be 8, 10, 12 or 14

  // Let the ADC settle
  delay(1);

  // Get a single ADC sample and throw it away
  readVBAT();
}

void get_battery_voltage(void)
{
  // Get a raw ADC reading
  uint16_t vbat_mv = readVBAT();

  // Convert from raw mv to percentage (based on LIPO chemistry)
  uint8_t vbat_per = mvToPercent(vbat_mv);

  // Display the results
#ifndef MAX_SAVE
  Serial.printf("LiPo Voltage =  %d mV\n", vbat_mv);
  Serial.printf("LiPo Charge  =  %d %%\n", vbat_per);
#endif

batteryVoltage = vbat_mv;
batteryCharge = vbat_per;
}

// SHTC3

void shtc3_read_data(void)
{
  float Temperature = 0;
  float Humidity = 0;
  
  g_shtc3.update();
  if (g_shtc3.lastStatus == SHTC3_Status_Nominal) // You can also assess the status of the last command by checking the ".lastStatus" member of the object
  {

    Temperature = g_shtc3.toDegC();               // Packing LoRa data
    Humidity = g_shtc3.toPercent();

  // Display the results
#ifndef MAX_SAVE
    Serial.printf("Temperature  =  %4.1f °C\n", Temperature);
    Serial.printf("Humidity     = %3.0f %% rH\n", Humidity);
#endif

    insideTemperature = (uint16_t) (Temperature * 10 + .5);
    insideHumidity = (uint8_t) (Humidity + .5);
  }
}
