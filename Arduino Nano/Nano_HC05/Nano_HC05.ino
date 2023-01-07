/*
 * Die Daten müssen im Format "R222#" gesendet werden.
 * Der Buchstabe steht für die Farbe (R,G,B), die Zahl (0-255) für die Helligkeit.
 * Die Raute dient als Terminator
 * Source: Franzis Bluething Board (Best.-Nr. CQ-14 48 59)
 */

char Zeichen;   //Jedes empfangene Zeichen kommt kurzzeitig in diese Variable.
String Text;    //In diesem String speichern wir dann unseren kompletten Text.

//const int ledPin =  13;      // the number of the LED pin
int rot=6;
int gruen=3;
int blau=5;

#include <SoftwareSerial.h>
SoftwareSerial HC05(12, 11); // RX, TX

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println(F("Bluething can be connected via DeviceName Bluething104"));
  Serial.println(F("RGB Control via #Rxxx or #Gxxx or #Bxxx (x = 0..255)"));

  //pinMode(ledPin, OUTPUT);
  pinMode(rot, OUTPUT);
  pinMode(gruen, OUTPUT);
  pinMode(blau, OUTPUT);
  
    
  // set the data rate for the SoftwareSerial port
  HC05.begin(38400);
  //Puffer leeren
  while(HC05.available() > 0) HC05.read();
   
  analogWrite(rot, 0);
  analogWrite(gruen, 0);
  analogWrite(blau, 255); 

  digitalWrite(LED_BUILTIN, 1);
  delay(100);
  digitalWrite(LED_BUILTIN, 0);     
}

void loop() 
{
  while(HC05.available() > 0)                 // So lange etwas empfangen wird, durchlaufe die Schleife.
  {  
    Zeichen = HC05.read();                    // Speichere das empfangene Zeichen in der Variablen "Zeichen".
    if (Zeichen!='#') Text.concat(Zeichen);   // Füge das Zeichen an den String an, damit wir den kompletten Text erhalten.
    if (Zeichen == '#') {                     // War das letzte Zeichen eine Raute?
      if (Text.startsWith("R"))               // Wird "Led an" gesendet wird die Led eingeschaltet
      {
        Text.replace("R", "");
        Serial.print("R: ");
        Serial.println(Text.toInt());
        analogWrite(rot, Text.toInt());
      }
      if (Text.startsWith("G")) {
        Text.replace("G", "");
        Serial.print("G: ");
        Serial.println(Text.toInt());
        analogWrite(gruen, Text.toInt());
      }
      if (Text.startsWith("B")) {
        Text.replace("B", "");
        Serial.print("B: ");
        Serial.println(Text.toInt());
        Serial.println();
        analogWrite(blau, Text.toInt());
      }
      Text="";                                 // Lösche den String für die nächste Nachricht.
    }
  }
}
