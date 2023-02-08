/*
 * File: LGT8F_ADDA_Test.ino
 * 
 * LGT8F328P Test of DAC & ADC
 * 
 * The LGT8F328P board by AZ-Delivery has one 8-bit DAC channel, connected to D4.
 * There are 8 12-bit ADC channels.  
 * Connect A2 & D4 for this test.
 * 
 * 2023-02-07 Claus Kuehnel  info@ckuehnel.ch
 */

#define DAC D4
#define ADC A2

uint8_t dac;
uint16_t adc;

void setup() 
{
   analogReadResolution(ADC12BIT);  // ADC10BIT default is 10bit 
   analogReference(DEFAULT);  // DEFAULT EXTERNAL INTERNAL1V024 INTERNAL2V048 INTERNAL4V096
   pinMode(DAC, ANALOG);
   pinMode(LED_BUILTIN, OUTPUT);
   
   Serial.begin(115200);
   delay(2000);   // wait for serial monitor
   Serial.println("\n DAC\tADC\t DIFF");
}

void loop()
{
  int16_t diff;
  
  for (dac = 0; dac < 256; dac++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    analogWrite(DAC, dac);
    delay(100);
    adc = analogRead(ADC);
    diff = 16 * dac - adc;
    Serial.print(dac); Serial.print("\t"); Serial.print(adc); Serial.print("\t"); Serial.println(diff);
    digitalWrite(LED_BUILTIN, LOW);
    delay(800);
  }
}


   
