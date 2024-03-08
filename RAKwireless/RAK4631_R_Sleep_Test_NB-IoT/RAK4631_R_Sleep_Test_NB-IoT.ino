/*
 * File: RAK4631-R_Sleep_Test_NB-IoT.ino
 *
 * Monitoring the voltage of the solar cell and battery 
 * to get information on the battery's state of charge
 * Sleep time is 5 minutes.
 *
 * 2023-11-06 Claus Kühnel info@ckuehnel.ch
 */
#include "Arduino.h"

#define BG77_POWER_KEY WB_IO1

const uint8_t AIN1 = WB_A1;   // for RAK19007

String command; // String to store BG77 commands.
uint8_t data_array[5] = {0};

void setup()
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);

  pinMode(BG77_POWER_KEY, OUTPUT);

  uint32_t t = millis();
  uint32_t timeout = 5000;

	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

	Serial.begin(115200);
  while (!Serial) if (millis() - t > timeout) break;

  Serial.println("\nMonitoring Vsol & Vbat - NB-IoT output");
  Serial.println("---------------------------------------");
  
	// Create a unified timer
  if (api.system.timer.create(RAK_TIMER_0, (RAK_TIMER_HANDLER) handler, RAK_TIMER_PERIODIC) != true) 
  {
    Serial.printf("Creating timer failed.\r\n");
    return;
  }
  // Set timer periode to 5 min and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * 300 , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }

  // Check if the modem is already awake
  timeout = millis();
  bool moduleSleeps = true;
  Serial1.begin(115200);
  delay(1000);

  initADC();
  delay(100);

  Serial1.println("ATI");
  
  //BG77 init
  while ((millis() - timeout) < 4000)
  {
    if (Serial1.available())
    {
      String result = Serial1.readString();
      Serial.println("Modem response after start:");
      Serial.println(result);
      moduleSleeps = false;
    }
  }
  modemOff();
  Connection();
}

void Connection(void)
{
  float Vsolar = readADC(AIN1);
  float Vbat = api.system.bat.get();
  float State = Vsolar/Vbat;
	Serial.printf("Vsolar = %5.3f V  Vbat = %5.3f V  State = %3.1f\r\n", Vsolar, Vbat, State );

  digitalWrite(LED_GREEN, HIGH);
  delay(20);
  digitalWrite(LED_GREEN, LOW);

  modemOn();
  Serial.println("BG77 power up!");
  delay(1000);

  modemConnect();

  command = "AT+QMTPUB=0,0,0,0,\"topic/pub\"\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "{\"Vsolar\":" + String(Vsolar,3) + "," + "\"Vbat\":" + String(Vbat, 3) + "}\r\032";
  BG77_write(command.c_str());
  BG77_read(2000);

  modemOff();  
}

void handler(void *)
{
  Connection();
}

// This example is complete timer driven. 
// The loop() does nothing than sleep.
void loop()
{
	api.system.sleep.all();
  api.system.scheduler.task.destroy();
}