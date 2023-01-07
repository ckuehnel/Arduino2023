// Initialize OLED display

void initOLED()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    Serial.println("SSD1306 allocation failed");
    for(;;);        // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setTextSize(2);               // 2:1 pixel scale
  display.clearDisplay();               // Clear display
  display.display();
}
