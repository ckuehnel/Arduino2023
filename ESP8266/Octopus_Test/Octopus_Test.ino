// Octopus
#include <Wire.h>
#include <i2cdetect.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>    // Encoder-Library http://www.pjrc.com/teensy/td_libs_Encoder.html
#include "octopus_pins.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(Neopixel_Number,Neopixel_Data, NEO_GRBW + NEO_KHZ800);

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 15 // Set BRIGHTNESS to about 1/5 (max = 255)

Adafruit_BME680 bme; // I2C
#define SEALEVELPRESSURE_HPA (1013.25)

int wert = 0 ;
Encoder button_encoder(14,12); // Encoder

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint8_t i=0; i<pixels.numPixels(); i++) 
  {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void setup()
{ 
  Serial.begin(115200);
  delay(2000);    // waiting for serial monitor
  Serial.println("\nOctopus - Test of internal Hardware");
  Serial.println("Initializing Octopus...");

  Wire.begin();

  pixels.begin(); // intitializing Neopixel
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setBrightness(BRIGHTNESS);

  pinMode(BTN, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // LED off

  if (bme.begin()) Serial.println("BME680 sensor connected.");
  else
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  
  Serial.println("Initialization finished.\n");
}

void loop() 
{ 
  wert = button_encoder.read() ;
  if (digitalRead(BTN) == 0) 
  {
    digitalWrite(LED, LOW);
    Serial.println("Button pressed");
  }
  else 
  {
    digitalWrite(LED,HIGH);
    Serial.println("Encoder Value:"+ String(wert));
  }
  Serial.println("\nBlinking Neopixels...");
  colorWipe(pixels.Color(255,   0,   0)     , 10); // Red
  delay(1000);
  colorWipe(pixels.Color(  0, 255,   0)     , 10); // Green
  delay(1000);
  colorWipe(pixels.Color(  0,   0, 255)     , 10); // Blue
  delay(1000);
  colorWipe(pixels.Color(  0,   0,   0)     , 10); // off
  delay(1000);
  
  Serial.println("\nScanning internal I2C Bus");
  i2cdetect(1,127);
  delay(2000);

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" °C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" kOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);
}