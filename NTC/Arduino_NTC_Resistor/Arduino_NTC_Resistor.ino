//More information at: https://www.aeq-web.com/ 
//Version 1.0 | 01-22-2019 

int ntc_pin = A0; //Pin of NTC
float R1 = 10000; //10K Resistor

float temp_c;
float temp_f;

void setup() {
Serial.begin(9600);
}

void loop() {
  float ain = analogRead(ntc_pin);
  float R2 = R1 * (1023.0 / (float)ain - 1.0);
  float lgr = log(R2);
  float tmp = (1.0 / (1.009249522e-03 + 2.378405444e-04*lgr + 2.019202697e-07*lgr*lgr*lgr));
  temp_c = tmp - 273.15;
  temp_f = (temp_c * 9.0)/ 5.0 + 32.0; 

  Serial.print("Temperature: "); 
  Serial.print(temp_f);
  Serial.print(" F; ");
  Serial.print(temp_c);
  Serial.println(" C");   
  delay(500);
}
