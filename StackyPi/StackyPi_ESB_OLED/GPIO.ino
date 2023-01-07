// Initialize on-board GPIO

void initGPIO(void)
{
  pinMode(LED_BUILTIN, OUTPUT);     // initialize digital pin LED_BUILTIN as an output.
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off

  //I2C0 
  Wire.setSDA(20);    // Header 3
  Wire.setSCL(21);    // Header 5
  Wire.begin();

  // SPI1 for SD Card
  SPI1.setRX(12);      // Header 33
  SPI1.setTX(11);      // Header 31
  SPI1.setSCK(10);     // Header 29
  SPI1.setCS(9);       // Header 15
  SPI1.begin(false);

  // SPI0 
  SPI.setRX(4);       // Header 21
  SPI.setTX(3);       // Header 19
  SPI.setSCK(2);      // Header 23
  SPI.setCS(5);       // Header 24
  SPI.begin(false);
}
