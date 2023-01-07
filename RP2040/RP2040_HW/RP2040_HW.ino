/*
 * File: RP2040_HW.ino
 * 
 * Used Board Maker Pi Pico w/ RP2040
 * 2022-02-10 Claus Kühnel info@ckuehnel.ch
 */

const int LED = 25; // On-board LED @ GPIO25
unsigned long last;
unsigned int i = 0;

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("RP2040 Dual-Core Test");
  last = millis();
}

void setup1()  // Setup for Core1
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() 
{
  Serial.print(".");
  delay(950);
  if (millis() - last > 10000)
  {
    last = millis();
    int j = (i++ % 5) + 1;
    Serial.printf("Send Number %d to Core1\n", j);
    rp2040.fifo.push((uint32_t) j);
  } 
}

void loop1()  // Loop for Core1
{
  if (rp2040.fifo.available() != 0) 
  {
    uint8_t number = (uint8_t) rp2040.fifo.pop();
    blink(number); 
  }
}

void blink(uint8_t count)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
  }
}
