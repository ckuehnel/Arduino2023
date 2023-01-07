/* Released into the public domain */

void setup() 
{
    Serial.begin(115200);
    delay(5000);
    Serial.printf("Temperature measurement by Tiny2040\n");
}

void loop() 
{
    Serial.printf("Core temperature: %2.1f *C\n", analogReadTemp());
    delay(5000);
}
