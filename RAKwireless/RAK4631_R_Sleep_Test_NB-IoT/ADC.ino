// ADC routines for RAK4631

float ref = 3.3;        // for RAK4631

void initADC(void)
{
  //analogReference(UDRV_ADC_MODE_DEFAULT); // for RAK4631 reference is 3.6 V
  analogReadResolution(12);
}

float readADC(uint8_t pin)
{
  uint16_t adc_value = analogRead(pin);
  float adc_voltage = ref*(((float)adc_value)/4096.)*2.5/1.5;

  return adc_voltage;
}