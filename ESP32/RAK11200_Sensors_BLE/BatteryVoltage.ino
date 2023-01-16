// Read battery voltage

#define VBAT_MV_PER_LSB (0.73242188F) // 3.0V ADC range and 12 - bit ADC resolution = 3000mV / 4096
#define VBAT_DIVIDER (0.6F)        // 1.5M + 1M voltage divider on VBAT = (1.5M / (1M + 1.5M))
#define VBAT_DIVIDER_COMP (1.45F)   // Compensation factor for the VBAT divider, depend on the board
#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

void print_adc_characteristics()
{
  Serial.println("Is ADC reference voltage have been burned to the eFuse?");
  if (ESP_OK == esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF)) 
    Serial.println("Calibration mode is supported in eFuse");
  else 
    Serial.println("Error, eFuse values are not burned");

  Serial.println("Are Two Point values have been burned to the eFuse?");
  if (ESP_OK == esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP)) 
    Serial.println("Calibration mode is supported in eFuse");
  else 
    Serial.println("Error, eFuse values are not burned");

  Serial.println("Which type of calibration value is used in characterization?");
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t calValue = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  switch (calValue) 
  {
    case 0: Serial.println("Characterization based on reference voltage stored in eFuse");
            break;
    case 1: Serial.println("Characterization based on Two Point values stored in eFuse");
            break;
    case 2: Serial.println("Characterization based on default reference voltage");
            break;
    default: Serial.println("Type not specified");
  }
}

uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars); 
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
