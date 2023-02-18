/*
 * File: Uno_JsonOutputv6.ino 
 * Build Json string w/ measuring data
 * Claus Kühnel 2020-04-14 info@ckuehnel.ch
 */

#include <ArduinoJson.h>

// helper macro
#define LINE(name,val) Serial.print(name); Serial.print(" "); Serial.println(val);

#define SENSOR1 "RAK1901"
#define SENSOR2 "RAK1902"
#define SENSOR3 "RAK12010"
#define SENSOR4 "RAK12047"
#define SENSOR5 "RAK12037"
#define SENSOR6 "RAK12039"

void setup() 
{
  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;

  LINE("File", __FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;
  
  // Date of compilation
  LINE("Compilation @", s1 + " " + s2);
  
  // Arduino IDE SW version
  LINE("ARDUINO IDE", ARDUINO);

  Serial.print("Test JSON using ArduinoJson v.");
  Serial.println(ARDUINOJSON_VERSION);

  //https://arduinojson.org/v6/assistant/ used for configuration
  StaticJsonDocument<384> doc;

  Serial.println("Serialization of data...");



JsonObject sensor1 = doc.createNestedObject("sensor1");
sensor1["temperature"] = 22.2;
sensor1["humidity"] = 66;

JsonObject sensor2 = doc.createNestedObject("sensor2");
sensor2["temperature"] = 22.2;
sensor2["pressure"] = 999.9;
doc["sensor3"]["lux"] = 22.2;
doc["sensor4"]["iVOC"] = 555;

JsonObject sensor5 = doc.createNestedObject("sensor5");
sensor5["temperature"] = 22.2;
sensor5["humidity"] = 66;
sensor5["CO2"] = 555;

JsonObject sensor6 = doc.createNestedObject("sensor6");
sensor6["PM1"] = 5;
sensor6["PM5"] = 10;
sensor6["PM10"] = 15;

  Serial.println("\nOutput minified JSON...");
  serializeJson(doc, Serial);

  Serial.println("\n\nOutput prettified JSON...");
  serializeJsonPretty(doc, Serial);
}

void loop() {};
