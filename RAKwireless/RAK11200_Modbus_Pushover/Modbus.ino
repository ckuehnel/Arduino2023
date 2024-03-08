// Modbus Initialization

void initModbus(void)
{
  Serial1.begin(9600,SERIAL_8N1);    //Data bit 8, check bit None, Stop bit 1
  while (!Serial1);
  mb.begin(&Serial1);
  mb.master();
  Serial.println("MODBUS RTU-Master Init Succeed.");
}

void readModbus(void)
{
  uint16_t res[REG_COUNT];
  
  if (!mb.slave())  // Check if no transaction in progress
  {    
    mb.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cbWrite); // Send Read HOLDING reg from Modbus Server
    while(mb.slave()) // Check if transaction is active
    { 
      mb.task();
      delay(10);
    }
    Serial.printf("Humidity    %5.1f %%rH\n",res[0]/10.);
    Serial.printf("Temperature %5.1f °C\n",res[1]/10.);
    humidity = res[0]/10.;
    temperature = res[1]/10.;
  }
}