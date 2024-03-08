// Quectel BG77 Support

// Read the return value of BG77.
void BG77_read(time_t timeout)
{
  time_t timeStamp = millis();
  while((millis() - timeStamp) < timeout)
  {
    if(Serial1.available()>0) 
    {
      Serial.print((char)Serial1.read());
      delay(1);
    }
  }
}

// Write commnads to BG77.
void BG77_write(const char *command) 
{
  while(*command)
  {
    Serial1.write(*command);
    command++;
  }
  Serial1.println();
}

void modemOn(void)
{
    // Module slept, wake it up
    pinMode(BG77_POWER_KEY, OUTPUT);
    digitalWrite(BG77_POWER_KEY, 0);
    delay(1000);
    digitalWrite(BG77_POWER_KEY, 1);
    delay(2000);
    digitalWrite(BG77_POWER_KEY, 0);
    delay(1000);
}

void modemOff(void)
{
     // Module active, switch off
  command = "AT+QPOWD";
	BG77_write(command.c_str());
  BG77_read(300);
}

uint8_t getRSSI(void)
{
  uint8_t signalStrength;
  do
  {
    // Send the "AT+CSQ" command to the modem
    command = "AT+CSQ";
	  BG77_write(command.c_str());
    BG77_read(1000);

    // Read the response from the modem
    String response = "";
    while (Serial1.available()) 
    {
      char c = Serial1.read();
      response += c;
    }
    Serial.println(response);

    // Check if the response contains "+CSQ:"
    if (response.indexOf("+CSQ:") != -1) 
    {
      // Extract the signal strength value from the response
      signalStrength = response.substring(response.indexOf(":") + 1).toInt();

      // Check if the signal strength is not equal to 99
      if (signalStrength != 99) {
      Serial.println("Signal strength is not equal to 99. Exiting loop.");
      break;
    } else {
      Serial.println("Signal strength is still 99. Will query again.");
    }
    } else Serial.println("Error: No +CSQ response found.");

    delay(1000); // Wait for a few seconds before the next query
  } while (signalStrength == 90);
  return signalStrength;
}

void modemConnect(void)
{
  command = "AT+CGDCONT=1,\"IP\",\"emnify\"";
	BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CFUN=1,0\r";
  BG77_write(command.c_str());
  BG77_read(15000);

  command = "AT+CGATT=1\r";
  BG77_write(command.c_str());
  BG77_read(2000);              //response in 140 s maximum

  command = "AT+CEREG?\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  command = "AT+CPIN?\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  command = "AT+QNWINFO\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  command = "AT+QCSQ\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  getRSSI();

  command = "AT+CSQ\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTOPEN=?\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTOPEN=0,\"broker.hivemq.com\",1883\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTOPEN?\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTCONN=0,\"RAK5860_MQTT\"\r";
  BG77_write(command.c_str());
  BG77_read(2000);  
}