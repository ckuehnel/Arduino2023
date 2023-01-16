// RAK14001

boolean initRAK14001(uint8_t ILED)
{
  if (rgb.begin()) 
    RAK14001_ready = true;
  else 
    Serial.println("RAK14001 not detected.");

  // set the current output level max, the range is 1 to 31
  rgb.setCurrent(ILED);
  
  return RAK14001_ready;
}

void setLED(void)
{
  if (RAK12047_ready)
  {
    if (iVOC < 10) rgb.setColor(0,0,0);      // OFF 
    else if ((iVOC >= 10) && (iVOC <= 200)) 
      rgb.setColor(0,255,0);   // GREEN   
    else if ((iVOC > 200) && (iVOC <= 400))
      rgb.setColor(255,255,0);                // YELLOW
    else
      rgb.setColor(255,0,0);                  // RED 
  }
  else rgb.setColor(0,0,0);                   // OFF 
}
