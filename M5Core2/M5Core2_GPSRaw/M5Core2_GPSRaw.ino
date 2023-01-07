#include <M5Core2.h>

HardwareSerial GPSRaw(2);

void setup() {
  
  M5.begin();
  GPSRaw.begin(9600, SERIAL_8N1, 13, 14);  // GPS init for Core2

  Serial.println("Hello");
} 

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    int ch = Serial.read();
    GPSRaw.write(ch);  
  }

  if(GPSRaw.available()) {
    int ch = GPSRaw.read();
    Serial.write(ch);
  }
}
