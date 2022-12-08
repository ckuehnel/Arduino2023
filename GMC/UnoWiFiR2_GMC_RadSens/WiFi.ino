// WiFi Initialization

void initWiFi() 
{
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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
    Serial.println("Data sent to Radmon.org.");
  }
  else Serial.println("Radmon Server not connected.");
}
