

void displayData()
{
  char sOutput[18] = "";
  char sValue[10] = "";
 
  u8g2.clearBuffer();            // clear the internal memory
  u8g2.drawStr(0,10, "Room Cond. Monitor");
  strcat(sOutput, "Temp = "); 
  dtostrf(temp, 4, 1, sValue);
  strcat(sOutput, sValue);
  strcat(sOutput, " *C");
  u8g2.drawStr(0,25, sOutput);
  
  strcpy(sOutput, "");
  strcat(sOutput, "Humi = "); 
  dtostrf(humi, 4, 1, sValue);
  strcat(sOutput, sValue);
  strcat(sOutput, " %rH");
  u8g2.drawStr(0,35, sOutput);
  
  strcpy(sOutput, "");
  strcat(sOutput, "TVOC = "); 
  itoa(TVOC, sValue, 10);   // convert TVOC to string sValue
  strcat(sOutput, sValue);
  strcat(sOutput, " ppb");
  u8g2.drawStr(0,50, sOutput);

  strcpy(sOutput, "");
  strcat(sOutput, "eCO2 = "); 
  itoa(eCO2, sValue, 10);   // convert TVOC to string sValue
  strcat(sOutput, sValue);
  strcat(sOutput, " ppm");
  u8g2.drawStr(0,60, sOutput);
  
  u8g2.sendBuffer();
}


void showLED()
{
  if (eCO2 <800) pixels.setPixelColor(0, pixels.Color(0, 150, 0)); // Green
  if ((eCO2 >= 800) && (eCO2 <1200)) pixels.setPixelColor(0, pixels.Color(120, 100, 20)); //Yellow
  if (eCO2 >= 1200) pixels.setPixelColor(0, pixels.Color(150, 0, 0)); // Red
  pixels.show();   // Send the updated pixel colors to the hardware.
}
