// WiFi Initialization
// reconnection after WiFi loss based on 
// https://forum.arduino.cc/t/arduino-uno-wifi-rev-2-wifinina-restart-after-network-connection-loss/576092

void initWiFi() 
{
  //connect to my SSID
  Serial.print("\nConnecting to "); Serial.println(ssid);
  WiFi.disconnect();
  status= WL_IDLE_STATUS;
  while(status!=WL_CONNECTED)
  {
    status = WiFi.begin(ssid, password);
    if (status==WL_CONNECTED) digitalWrite(LED_BUILTIN, HIGH); //LED ON to show connected
    else 
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void TestWiFiConnection()
//test if always connected
{
 int StatusWiFi=WiFi.status();
 if (StatusWiFi==WL_CONNECTION_LOST || StatusWiFi==WL_DISCONNECTED || StatusWiFi==WL_SCAN_COMPLETED) //if no connection
 {
  digitalWrite(LED_BUILTIN, LOW); //LED OFF to show disconnected
  if(ScanSSIDs()) initWiFi(); //if my SSID is present, connect
 }
} 

char ScanSSIDs()
//scan SSIDs, and if my SSID is present return 1
{
  char score=0;
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) return(0); //error
  for(int thisNet=0;thisNet<numSsid;thisNet++) if(strcmp(WiFi.SSID(thisNet),ssid)==0) score=1; //if one is = to my SSID
  return(score);
}

void connectServer()
{
  if (client.connect(server, 80))
  {
    client.print("GET /radmon.php?function=submit&user=");
    client.print(RadmonUserName); client.print("&password=");
    client.print(RadMonPassWord); client.print("&value=");
    client.print(int(rate));
    client.print("&unit=CPM");
    client.println(" HTTP/1.0");
    client.println("HOST: radmon.org");
    client.println();
    Serial.println("Data sent to Radmon.org.\n");
  }
  else Serial.println("Radmon Server not connected.\n");
}
