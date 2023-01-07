// RAK12047

boolean initRAK12047(void)
{
  if (mySGP40.begin()) 
    RAK12047_ready = true;
  else 
    Serial.println("RAK12047 not detected.");
  
  return RAK12047_ready;
}

void readRAK12047(void)
{
  if(RAK1901_ready)
    iVOC = mySGP40.getVOCindex(humi, temp); 
  else 
    iVOC = mySGP40.getVOCindex(); //Get the VOC Index using the default RH (50%) and T (25C)

  if (iVOC == -100)
    Serial.println("Error in readSample()");
  else
    Serial.printf("RAK12047: iVOC = %3d\n", iVOC);
}
