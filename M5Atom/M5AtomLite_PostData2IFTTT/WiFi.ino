// WiFi Initialization

void initWiFi()
{
  boolean flag = false;
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    flag = !flag;
    if (flag) M5.dis.drawpix(0, 0x0000f0);
    else      M5.dis.drawpix(0, 0x000000); 
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void closeWiFi()
{
  if (WiFi.isConnected()) 
  { 
    WiFi.disconnect();
    Serial.println("Disconnected now");
  }
}
