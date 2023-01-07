/*
 * File: StackyPi_ESB_OLED.ino
 *
 * All StackyPi_ESB_xxx.ino programs are used to test single fuctions
 * of the Coral Environmental Sensor Board ESB, a Raspberry Pi HAT.
 * On-board OLED is connected to SPI0.
 * 
 * (c) 2022-05-05 Claus Kuehnel (info@ckuehnel.ch)
 */
#include <Wire.h>
#include <SPI.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Coral ESB Pin Definitions
#define DC 26
#define RST 22
#define CS 5
#define MOSI 3
#define CLK 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, MOSI, CLK, DC, RST, CS);

void setup(void) 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.printf("StackyPi GPIO initialization...");
  initGPIO();
  if (!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    Serial.println("SSD1306 allocation failed");
    for(;;);        // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  Serial.printf("done\n");

  Serial.println("\nThis program tests basic functions of ESB connected to the StackyPi board.");
  Serial.println("The OLED of ESB is used for text output.");
}

void loop() 
{
  display.setTextSize(1);             // Normal 1:1 pixel scale

  display.clearDisplay();
  display.setCursor(0, 0); display.println(F("This is"));             
  display.setCursor(0, 8); display.println(F("Coral"));
  display.setCursor(0,16); display.println(F("Environmental"));             
  display.setCursor(0,24); display.println(F("Sensor Board"));
  display.display();
  delay(2000);
  
  display.clearDisplay();
  display.setCursor(0, 0); display.println(F("as pHAT on"));             
  display.setCursor(0, 8); display.println(F("StackyPi"));
  display.setCursor(0,16); display.println(F("driven by"));                
  display.setCursor(0,24); display.println(F("RaspberryPi Pico"));
  display.display();
  delay(2000);

  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(0, 0); display.println(F("Coral"));             
  display.setCursor(0,16); display.println(F("OLED"));
  display.display();
  delay(2000);
}

   
