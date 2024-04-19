/*
 * Displaying measuring value of SHTC3 Sensor on ePaper is organized here.
 * 
 * 2024-04-17 Claus Kühnel info@ckuehnel.ch
 */

 void initDisplay()
 {
    pinMode(EPD_POWER_ENABLE, OUTPUT);
    digitalWrite(EPD_POWER_ENABLE, HIGH);
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(); // enable diagnostic output on Serial
    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    //display.setFont(&FreeMono9pt7b);
    display.setFont(&FreeSerif9pt7b);
 }

 void displayValues()
 {
    display.setCursor(0, 10); display.print("Node #"); display.println(ID);
    display.setCursor(0, 26); display.print("Temp "); display.print(Temperature, 1); display.println(" *C");
    display.setCursor(0, 42); display.print("Humidity "); display.print(Humidity, 0); display.println(" %");
    display.setCursor(0, 58); display.print("Battery "); display.print(volt,3); display.println(" V");
    display.setFont(&FreeSerif9pt7b);
    display.setCursor(0, 74); displayMac(); //display.println(WiFi.macAddress());
    display.update();

 }

void displayMac()
{
  for (int i=0; i <= sizeof(WiFi.macAddress()); i++)
  {
    uint8_t c = WiFi.macAddress()[i];
    if (c != ':') display.print(char(c));
  }
}
 