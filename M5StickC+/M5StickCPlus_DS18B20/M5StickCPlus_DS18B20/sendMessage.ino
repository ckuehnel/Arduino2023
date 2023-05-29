// send message after BTN A pressed

void sendMessage(void)
{
  Serial.println("BtnA pressed...");
  M5.Beep.beep();
  //Serial.printf("Vbat: %5.3f V\r\n",vbat);
  
  WiFi.disconnect();  
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
        heartbeat();
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 
  String message = "Temperature = ";
         message += String(tmp,1); 
         message += " °C";

  int len = message.length();
  char charBuf[len+1];
  message.toCharArray(charBuf, len+1);
  Serial.println(charBuf);
  pushover(charBuf,0); // Priority 0
  WiFi.disconnect();
  Serial.println("WiFi disconnected.");  


}
