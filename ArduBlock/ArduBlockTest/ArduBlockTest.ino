/**
   file:	ArduBlockTest
   author:	Claus Kuehnel

   description:
   Test der ArduBlock Programmierumgebung

   date:	15.01.2024
   version:	0.1
*/

byte count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  delay( 1000 );

  Serial.print("Test of ArduBlock Environment ");
  Serial.println();

}

void loop() {
  Serial.print("* ");
  count = ( count + 1 );
  if (( count > 20 )) {

    Serial.print("> ");
    Serial.println();
    count = 0;
  }
  digitalWrite( 13 , HIGH );
  delay( 100 );
  digitalWrite( 13 , LOW );
  delay( 900 );
}
