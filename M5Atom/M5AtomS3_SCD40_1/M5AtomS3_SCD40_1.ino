/*
 * File: M5AtomS3_SCD40_1.ino
 * Reading environmental data of Sensirion SCD40 sensor
 * 
 * Include this line 
 *   #define TFT_RGB_ORDER 0	// to get right colors on M5AtomS3 LCD
 * after line #35 in file ..\libraries\M5AtomS3-main\src\utility\In_eSPI_Drivers\ST7789_Init.h
 * 
 * 2023-01-15  Claus Kühnel info@ckuehnel.ch
 */ 
 #include <M5AtomS3.h>

const int16_t SCD_ADDRESS = 0x62;

void setup(void)
{
  int error;
  
  M5.begin(true, true, true, false);  // Init AtomS3 (Initialize LCD, serial portm I2C).
  Wire1.begin(2, 1);  // Detects external I2C
  
    // wait until sensors are ready, > 1000 ms according to datasheet
  delay(1000);

  // start scd measurement in periodic mode, will update every 5 s
  Wire1.beginTransmission(SCD_ADDRESS);
  Wire1.write(0x21);
  Wire1.write(0xb1);
  error = Wire1.endTransmission();
  
  M5.Lcd.setRotation(2);
  M5.Lcd.setTextSize(2);
  
  // wait for first measurement to be finished
  delay(5000);
}

void loop() 
{
  float co2, temperature, humidity;
  uint8_t data[12], counter;
  int error;  

  // send read data command
  Wire1.beginTransmission(SCD_ADDRESS);
  Wire1.write(0xec);
  Wire1.write(0x05);
  error = Wire1.endTransmission();
 
  // read measurement data: 2 bytes co2, 1 byte CRC,
  // 2 bytes T, 1 byte CRC, 2 bytes RH, 1 byte CRC,
  // 2 bytes sensor status, 1 byte CRC
  // stop reading after 12 bytes (not used)
  // other data like  ASC not included
  Wire1.requestFrom(SCD_ADDRESS, 12);
  counter = 0;
  while (Wire1.available()) {
    data[counter++] = Wire1.read();
  }

  // floating point conversion according to datasheet
  co2 = (float)((uint16_t)data[0] << 8 | data[1]);
  // convert T in degC
  temperature = -45 + 175 * (float)((uint16_t)data[3] << 8 | data[4]) / 65536;
  // convert RH in %
  humidity = 100 * (float)((uint16_t)data[6] << 8 | data[7]) / 65536;

  if (co2 < 1000) 
  {
    M5.Lcd.fillScreen(GREEN);    //Fill the screen with green
    m5.Lcd.setTextColor(BLACK,GREEN);
  }
  else if (co2 > 2000) 
  {
    M5.Lcd.fillScreen(RED); //Fill the screen with red
    M5.Lcd.setTextColor(WHITE,RED);
  }
  else 
  {
    M5.Lcd.fillScreen(YELLOW); //Fill the screen with yellow
    M5.Lcd.setTextColor(BLACK,YELLOW);        
  }
  M5.Lcd.setCursor(5,15);
  M5.Lcd.printf("CO2:\n %5.0f ppm", co2);
  M5.Lcd.setCursor(5,55);
  M5.Lcd.printf("Temp:\n  %5.1f *C", temperature); 
  M5.Lcd.setCursor(5,95);
  M5.Lcd.printf("Humi:\n %5.0f %%rH", humidity); 

  // wait 5 s for next measurement
  delay(5000);
}
