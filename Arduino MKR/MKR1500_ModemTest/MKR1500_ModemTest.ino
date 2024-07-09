/*
 * This example tests to see if the modem of the  MKR NB 1500 board is working correctly. 
 * You do not need  a SIM card for this example.
 * 
 * Circuit: MKR NB 1500 board, Antenna
 * 
 * Created 12 Jun 2012 by David del Peral
 * modified 21 Nov 2012 by Tom Igoe
 */
#include <MKRNB.h>

NBModem modem;

String IMEI = "", ICCID = "";

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  // start modem test (reset and check response)
  Serial.println("Starting MKR1500 NB modem test...");
  if (modem.begin()) Serial.println("modem.begin() succeeded");
  else               Serial.println("ERROR, no modem answer.");

  // get modem IMEI
  Serial.println("Checking IMEI...");
  IMEI = modem.getIMEI();

  // check IMEI response
  if (IMEI != NULL) 
  {
    Serial.println("Modem's IMEI: " + IMEI);

    // reset modem to check booting:
    Serial.println("Resetting modem...");
   modem.begin();

    // get and check IMEI one more time
   if (modem.getIMEI() != NULL) Serial.println("Modem is functioning properly");
   else                         Serial.println("Error: getIMEI() failed after modem.begin()");
  } 
  else Serial.println("Error: Could not get IMEI");

  // get ICCID of SIM card
  Serial.println("Checking ICCID...");
  ICCID = modem.getICCID();
  Serial.println("ICCID: " + ICCID);
}

void loop(){};
