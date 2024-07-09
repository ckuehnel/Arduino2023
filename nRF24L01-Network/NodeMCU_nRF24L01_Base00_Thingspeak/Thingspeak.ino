// Write a float value into a Thingspeak field

void writeValue(uint8_t field, float val)
{
  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, field, val, myWriteAPIKey);

  if (httpCode == 200) Serial.println("Channel write successful.");
  else                 Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
}
