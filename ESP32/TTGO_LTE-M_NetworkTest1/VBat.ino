// Measuring battery voltage

float readBattery(uint8_t pin)
{
    int vref = 1100;
    uint16_t volt = analogRead(pin);
    float battery_voltage = ((float)volt / 4096.0) * 2.0 * 3.3 * (vref);
    return battery_voltage;
}
