// Prepare data and send therm to Weathercloud

void sendData2Weathercloud(void)
{
   Serial.println("\nPrepare data to send to WetaherCloud...");
  //change TZ to WET
  setTimezone(WET); // Western European Time
  printLocalTime();

  String URL = "http://api.weathercloud.net/v01/set";
  URL += String("/wid/") + String(WID); 
  URL += String("/key/") + String(KEY);
  URL += String("/temp/") + String(temperature * 10);
  URL += String("/hum/") + String(humidity);
  URL += String("/bar/") + String(pressure * 10);
  URL += String("/wspd/") + String(wind_speed * 10);
  URL += String("/softwareid/") + String(SWID);
  URL += String("/");
  Serial.printf("Temperature %5.1f °C, Humidity %3.0f %%, Pressure %4.1f hPa, Wind Speed %4.2f m/s\n", temperature, humidity, pressure, wind_speed);
  Serial.println(URL);

  http.begin(URL);
    
  // Send HTTP GET request
  int httpResponseCode = http.GET();  
  if (httpResponseCode>0) 
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else 
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}