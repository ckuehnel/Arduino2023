// Initialize ADC TLA2021 on Coral ESB

void initADC()
{
  // Try to initialize!
  if (!tla.begin(TLA2021_ADDR)) 
  {
    Serial.println("Failed to find TLA202x chip");
    while (1) { delay(10); }
  }
  Serial.println("TLA202x Found!");

  tla.setDataRate(TLA202x_RATE_920_SPS);
  Serial.print("Data rate set to: ");
  switch (tla.getDataRate()) {
    case TLA202x_RATE_128_SPS: Serial.println("128 SPS");break;
    case TLA202x_RATE_250_SPS: Serial.println("250 SPS");break;
    case TLA202x_RATE_490_SPS: Serial.println("490 SPS");break;
    case TLA202x_RATE_920_SPS: Serial.println("920 SPS");break;
    case TLA202x_RATE_1600_SPS: Serial.println("1600 SPS");break;
    case TLA202x_RATE_2400_SPS: Serial.println("2400 SPS");break;
    case TLA202x_RATE_3300_SPS: Serial.println("3300 SPS");break;
  }

  tla.setRange(TLA202x_RANGE_6_144_V);
  Serial.print("Measurement range set to: ");
  switch(tla.getRange()){
    case TLA202x_RANGE_6_144_V:
    Serial.println("+6.144 V to -6.144 V"); break;
    case TLA202x_RANGE_4_096_V:
    Serial.println("+4.096 V to -4.096 V"); break;
    case TLA202x_RANGE_2_048_V:
    Serial.println("+2.048 V to -2.048 V"); break;
    case TLA202x_RANGE_1_024_V:
    Serial.println("+1.024 V to -1.024 V"); break;
    case TLA202x_RANGE_0_512_V:
    Serial.println("+0.512 V to -0.512 V"); break;
    case TLA202x_RANGE_0_256_V:
    Serial.println("+0.256 V to -0.256 V"); break;
  }

  tla.setMux(TLA202x_MUX_AIN0_GND);
  Serial.print("Multiplexer set to: ");
  switch (tla.getMux()) {
    case TLA202x_MUX_AIN0_AIN1:
      Serial.println("AINp = AIN 0, AINn = AIN 1"); break;
    case TLA202x_MUX_AIN0_AIN3:
      Serial.println("AINp = AIN 0, AINn = AIN 3"); break;
    case TLA202x_MUX_AIN1_AIN3:
      Serial.println("AINp = AIN 1, AINn = AIN 3"); break;
    case TLA202x_MUX_AIN2_AIN3:
      Serial.println("AINp = AIN 2, AINn = AIN 3"); break;
    case TLA202x_MUX_AIN0_GND:
      Serial.println("AINp = AIN 0, AINn = GND"); break;
    case TLA202x_MUX_AIN1_GND:
      Serial.println("AINp = AIN 1, AINn = GND"); break;
    case TLA202x_MUX_AIN2_GND:
      Serial.println("AINp = AIN 2, AINn = GND"); break;
    case TLA202x_MUX_AIN3_GND:
      Serial.println("AINp = AIN 3, AINn = GND"); break;

  }
  tla.setMode(TLA202x_MODE_CONTINUOUS);
  Serial.print("Reading mode: ");
  switch(tla.getMode()){
    case TLA202x_MODE_ONE_SHOT: Serial.println("One-shot"); break;
    case TLA202x_MODE_CONTINUOUS: Serial.println("Continuous"); break;
  }
}
