/*
 * File: Octopus_CO2_Ampel.ino
 * 
 * Measuring Air Quality (IAQ) by Bosch BME680 and calculationg the IAQ Index 
 * w/ Bosch BSEC algorithm
 */
#include "bsec.h"
#include <Adafruit_NeoPixel.h>
#include "octopus_pins.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(Neopixel_Number,Neopixel_Data, NEO_GRBW + NEO_KHZ800);

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 15 // Set BRIGHTNESS to about 1/5 (max = 255)

#define DEBUG 1

// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

// Create an object of the class Bsec
Bsec iaqSensor;

float iAQ = 0;
int iAQ_Acc = 0;

String output;

// Entry point for the example
void setup(void)
{
  /* Initializes the Serial communication */
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nOctopus CO2 Ampel");
  Serial.println("Initializing Octopus...");
  
  pixels.begin(); // intitializing Neopixel
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setBrightness(BRIGHTNESS);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // LED off
  
  iaqSensor.begin(BME68X_I2C_ADDR_LOW, Wire);
  
  output = "BSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  if (DEBUG) Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[13] = 
  {
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_STABILIZATION_STATUS,
    BSEC_OUTPUT_RUN_IN_STATUS,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    BSEC_OUTPUT_GAS_PERCENTAGE
  };

  iaqSensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  Serial.println("Initialization finished.\n");

  // Print the header
  output = "Timestamp [ms], IAQ, IAQ accuracy, Static IAQ, CO2 equivalent, breath VOC equivalent, raw temp[°C], pressure [hPa], raw relative humidity [%], gas [Ohm], Stab Status, run in status, comp temp[°C], comp humidity [%], gas percentage";
  if (DEBUG) Serial.println(output);
}

// Function that is looped forever
void loop(void)
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run()) { // If new data is available
    digitalWrite(LED, LOW);
    output = String(time_trigger);
    iAQ = iaqSensor.iaq; output += ", " + String(iAQ);
    iAQ_Acc = iaqSensor.iaqAccuracy; output += ", " + String(iAQ_Acc);
    output += ", " + String(iaqSensor.staticIaq);
    output += ", " + String(iaqSensor.co2Equivalent);
    output += ", " + String(iaqSensor.breathVocEquivalent);
    output += ", " + String(iaqSensor.rawTemperature);
    output += ", " + String(iaqSensor.pressure);
    output += ", " + String(iaqSensor.rawHumidity);
    output += ", " + String(iaqSensor.gasResistance);
    output += ", " + String(iaqSensor.stabStatus);
    output += ", " + String(iaqSensor.runInStatus);
    output += ", " + String(iaqSensor.temperature);
    output += ", " + String(iaqSensor.humidity);
    output += ", " + String(iaqSensor.gasPercentage);
    if (DEBUG) Serial.println(output);
    digitalWrite(LED, HIGH);
  } else {
    checkIaqSensorStatus();
  }
  if (iAQ_Acc != 0)
  {
    if      (iAQ <= 50) colorWipe(pixels.Color(0, 228, 2), 10); // Green
    else if (iAQ > 50 && iAQ <= 100)  colorWipe(pixels.Color(126, 208, 20), 10); 
    else if (iAQ > 100 && iAQ <= 150) colorWipe(pixels.Color(255, 252,  5), 10); 
    else if (iAQ > 150 && iAQ <= 200) colorWipe(pixels.Color(255, 126,  0), 10);
    else if (iAQ > 200 && iAQ <= 250) colorWipe(pixels.Color(255,  19,  0), 10);
    else if (iAQ > 250 && iAQ <= 350) colorWipe(pixels.Color(163,  32, 64), 10);
    else     colorWipe(pixels.Color(102, 51, 0), 10);
  }
}

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

// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.bsecStatus != BSEC_OK) {
    if (iaqSensor.bsecStatus < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.bsecStatus);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme68xStatus != BME68X_OK) {
    if (iaqSensor.bme68xStatus < BME68X_OK) {
      output = "BME68X error code : " + String(iaqSensor.bme68xStatus);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BME68X warning code : " + String(iaqSensor.bme68xStatus);
      Serial.println(output);
    }
  }
}

void errLeds(void)
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}
