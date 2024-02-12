// Prepare data and send therm to Meteologix

// recalculation of units
// °C in °F    : tempf = temperature * 1.8 + 32
// hPa in inHg : pressinHg =  pressure * 0.02953
// m/s in mph  : wind_speed_mph = wind_speed * 2.23693629
// Time as UTC URL -encoded


void sendData2Meteologix(void)
{
   Serial.println("\nPrepare data to send to Meteologix...");
  //change TZ to UTC
  setTimezone("GMT0");
  printLocalTime();

  float tempf = temperature * 1.8 + 32;
  float pressinHg = pressure * 0.02953;
  float wind_speed_mph = wind_speed * 2.23693629;

  String URL = "http://wow.meteologix.com/pws/?";
  URL += String("siteid=") + String(STATION_ID_NUMBER); 
  URL += String("&sitekey=") + String(STATION_KEY);
  URL += String("&dateutc=") + String(urlEncode(timeStringBuff));
  URL += String("&tempf=") + String(tempf);
  URL += String("&humidity=") + String(humidity);
  URL += String("&baromin=") + String(pressinHg);
  URL += String("&windspeedmph=") + String(wind_speed_mph);
  Serial.printf("Temperature %5.1f °F, Humidity %3.0f %%, Pressure %4.1f inHG, Wind Speed %4.2f mph, \n", tempf, humidity, pressinHg, wind_speed_mph);
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