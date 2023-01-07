/*
 * File: M5CameraF_Timer_FTP.ino
 * 
 * based on Web-Mini-Cam-FTP-Timer-PIR.ino by Michael U.
 * WiFi & FTP credentials in arduino_secrets.h
 * Adaptions to M5CameraF(isheye)
 * 2021-05-05 Claus Kühnel info@ckuehnel.ch
 */
 
#include <WiFi.h>
#include <WiFiclient.h>
#include "esp_camera.h"
#include "arduino_secrets.h"

#define DEBUG
#define DEBUG_FTP false // true
#define DEBUG_CAM false //true

// FTP-Userdaten direkt in Zeile 41 eintragen, also xxxx und yyyy anpassen
// da wohl verschiedene Kameramodule im Umlauf sind ab Zeile 180 hmirror und vflip anpassen wenn das Bild falsch herum ist.

// PIR-Cam ist das Modul mit PIR und OLED, M5Cam ist hier die M5CameraF. Beide mit AI Thinker ESP32-CAM compilieren.
//#define PIR_CAM                           // für M5Cam auskommentieren

// DeepSleep Timer
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason(void);

String ftp_pfad = "ftp/ESP32";

#ifdef PIR_CAM
String bildname = "PIR_CAM";               
#else
String bildname = "M5_CAM";              
#endif
  
#define FTP_ANZAHL 4
String ftp_comm[] = {"USER ck","PASS Cksz08011951_","SYST","Type I"};
String ftp_ret[] = {"331","230","215","200"};

WiFiClient ftpclient;
WiFiClient dclient;

String uploadFTP(String filename);
String ftpReceive(void);
bool ftpCommand(uint8_t index);

// Kamera
#ifdef PIR_CAM

#define PWDN_GPIO_NUM 26
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 32
#define SIOD_GPIO_NUM 13
#define SIOC_GPIO_NUM 12

#define Y9_GPIO_NUM 39
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 23
#define Y6_GPIO_NUM 18
#define Y5_GPIO_NUM 15
#define Y4_GPIO_NUM 4
#define Y3_GPIO_NUM 14
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 27
#define HREF_GPIO_NUM 25
#define PCLK_GPIO_NUM 19
#define XCLK_FREQUENZ  20000000

#define RESOLUTION  FRAMESIZE_SXGA

#else

//CAMERA_MODEL_M5STACK_WIDE
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     22
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#define RESOLUTION  FRAMESIZE_XGA

#endif
  
static camera_config_t camera_config = 
{
    .pin_pwdn  = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk  = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,

    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,//YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = RESOLUTION,   //QQVGA-QXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 10, //0-63 lower number means higher quality
    .fb_count = 1 //if more than one, i2s runs in continuous mode. Use only with JPEG
};

esp_err_t camera_init(void);
esp_err_t camera_capture(void);
void setSensordaten(void);
void grabPicture(String filename);

//#########################################################

void setup(void)
{
  Serial.begin(115200);

//Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

//Print the wakeup reason for ESP32
  print_wakeup_reason();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Wifi connecting...");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  esp_err_t err = esp_camera_init(&camera_config);
  if (err == ESP_OK)
  {
    Serial.println("Cam Init ok");
  }
  else  
  {
    Serial.println("Cam Init FAIL");
  }
  setSensordaten();
  
  sensor_t *s = esp_camera_sensor_get();
#ifndef PIR_CAM
  s->set_vflip(s, 1);          // 0 off, 1 on
  s->set_hmirror(s, 1);        // 0 off, 1 on
#endif  
}

//----------------------------------------------------------

void loop(void)
{
  Serial.print(bildname); Serial.println(".jpg");
  uploadFTP(bildname);  

  Serial.println("Going to sleep now");
  delay(100);
  Serial.flush(); 

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  btStop();  
  esp_deep_sleep_start();
  
  Serial.println("This will never be printed");
}

//#########################################################
//-------------------- Sleep Tools ------------------------
//#########################################################


void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

//#########################################################
//--------------------- CAM Tools -------------------------
//#########################################################

void setSensordaten()
{
  size_t res = 0;
  sensor_t * s = esp_camera_sensor_get();

//  res = s->set_framesize(s, (framesize_t)val);          // Framesize      FRAMESIZE_(QQVGA, HQVGA, QVGQ, CIF, VGA, SVGA, XGA, SXGA, UXGA)
  res = s->set_quality(s, 10);                          // JPEG Quality   (10...63)  10
  res = s->set_contrast(s, 0);                          // Kontrast       (-2...+2)   0
  res = s->set_brightness(s, 0);                        // Helligkeit     (-2...+2)   0
  res = s->set_saturation(s, 0);                        // Farbsättigung  (-2...+2)   0 
  res = s->set_gainceiling(s, (gainceiling_t) GAINCEILING_2X);      // Verstärkung    GAINCEILING_(2x, 4x, 8x, 16x, 32x, 64x, 128x)   2x
  res = s->set_colorbar(s, 0);                          // Farbbalken     (off/on)   off 
  res = s->set_whitebal(s, 1);                          // Weißbalance    (off/on)    on
  res = s->set_gain_ctrl(s, 1);                         // AGC            (off/on)    on
  res = s->set_exposure_ctrl(s, 1);                     // AEC            (off/on)    on               
//  res = s->set_hmirror(s, val);                         // H-Mirror       (off/on) 
//  res = s->set_vflip(s, val);                           // V-Flip         (off/on) 
  res = s->set_awb_gain(s, 1);                        // Verstärkung AWB
  res = s->set_agc_gain(s, 1);                        // Verstärkung AGC
//  res = s->set_aec_value(s, val);                       // Wert AEC       (0...1200)  
//  res = s->set_aec2(s, val);                            // Wert AEC2
  res = s->set_dcw(s, 1);                             // Downsize       (off/on)    on  
  res = s->set_bpc(s, 0);                             //                (off/on)   off
  res = s->set_wpc(s, 1);                             //                (off/on)    on
  res = s->set_raw_gma(s, 1);                         // RAW-Gamma      (off/on)    on
  res = s->set_lenc(s, 1);                            // Linsenkorr.    (off/on)    on 
  res = s->set_special_effect(s, 0);                  // Special Effekt 
  res = s->set_wb_mode(s, 0);                         // WB Mode         (Auto/Sunny/..) 
  res = s->set_ae_level(s, 0);                        // AE Level        (-2...+2)    0
}

//#########################################################
//--------------------- FTP Tools -------------------------
//#########################################################

String uploadFTP(String bildname)
{ 
  camera_fb_t * fb = esp_camera_fb_get();

  String returnText = "";
  String meldung = "";
  bool flag_ftp_ok = true;
   
//  String bildnummer = "00000" + String(bootCount, DEC);
//  bildnummer = bildnummer.substring(bildnummer.length() - 5);
  String dateiname = bildname /*+ bildnummer*/ + ".jpg";

  if (ftpclient.connect(ftpserver, ftpport)) // 21 = FTP server
  { 
    Serial.println(F("Command connected"));
  }
  else
  {
    Serial.println(F("Command connection failed"));
    return "FTP Error";
  }

  returnText = ftpReceive();
  meldung = returnText.substring(0,3); // erste 3 Zeichen
  if (DEBUG_FTP) Serial.println("Return: " + meldung);
  if (meldung == "220")
  {
    for (uint8_t i = 0; i < FTP_ANZAHL; i++)
    {
      if (!ftpCommand(i))
      {
        flag_ftp_ok = false;
        break;
      }  
    }  

    if (flag_ftp_ok)    // alles klar, PASV senden
    {
      uint8_t werte[6];
      uint8_t pos = 0;
      uint8_t start = 0;
      uint8_t komma = 0;
      uint16_t dport = 0;
      
      if (DEBUG_FTP) Serial.println("PASV");
      ftpclient.println("PASV");
      String returnText = ftpReceive();
      String meldung = returnText.substring(0,3); // erste 3 Zeichen
      if (DEBUG_FTP) Serial.println("Return: " + meldung); 
      if (meldung == "227")
      {
        start = returnText.indexOf("(");
        for (uint8_t i = 0; i < 5; i++)
        {
          komma = returnText.indexOf(",", start + 1);
          werte[pos] = returnText.substring(start + 1, komma).toInt();
          if (DEBUG_FTP) Serial.println(werte[pos], DEC);
          pos++; 
          start = komma;
        }       
        werte[pos] = returnText.substring(start + 1).toInt();
        if (DEBUG_FTP) Serial.println(werte[pos], DEC);
        dport = (werte[4] << 8) | werte[5];
        if (DEBUG_FTP) Serial.println("Datenport: " + String(dport, DEC));
        if (dclient.connect(ftpserver, dport))
        {
          Serial.println(F("Data connected"));
        }
        else
        {
          Serial.println(F("Data connection failed"));
          ftpclient.stop();
          return "Error";
        }
        if (DEBUG_FTP) Serial.println("CWD " + ftp_pfad);
        ftpclient.println("CWD " + ftp_pfad);
        String returnText = ftpReceive();
        String meldung = returnText.substring(0,3); // erste 3 Zeichen
        if (DEBUG_FTP) Serial.println("Return: " + meldung); 
      
        if (meldung == "250")
        {
          Serial.println(dateiname);
          if (DEBUG_FTP) Serial.println("STOR " + dateiname);
          ftpclient.println("STOR " + dateiname);
          String returnText = ftpReceive();
          String meldung = returnText.substring(0,3); // erste 3 Zeichen
          if (DEBUG_FTP) Serial.println("Return: " + meldung); 
          if (meldung == "150")
          {
            Serial.print(F("Writing..."));
            dclient.write((const uint8_t *)fb->buf, fb->len);
            Serial.println(F("OK"));
          }          
        }        
        dclient.stop();
        Serial.println(F("Data disconnected"));
      }  
    }

  }
 
  ftpclient.println(F("QUIT"));

  ftpclient.stop();
  Serial.println(F("Command disconnected"));

  esp_camera_fb_return(fb);  

  return "OK";
} 

//----------------------------------------------------------

String ftpReceive()  
{                       // gibt nur die letzte Zeile zurück
  char thisByte;
  uint8_t count = 0;
  String outText[20];
  String retText = "";
  
  while (!ftpclient.available()) delay(1);  // auf Daten warten

  while (ftpclient.available())
  {
    thisByte = ftpclient.read();
#ifdef DEBUG_FTP
    if (DEBUG_FTP) Serial.write(thisByte);
#endif
    if (thisByte == 13)          // return
    {
      count++;      // nächste Zeile
    }
    else if (thisByte != 10)          // newline
    {
      outText[count] += thisByte;
    }
  }  
  for (uint8_t i = 20; i > 0; i--)
  {
    if (outText[i - 1] > "")
    {
      retText = outText[i - 1];
      break;
    }  
  }  
  return retText;
} 

//----------------------------------------------------------

bool ftpCommand(uint8_t index)
{
  if (DEBUG_FTP) Serial.println(ftp_comm[index]);
  ftpclient.println(ftp_comm[index]);
  String returnText = ftpReceive();
  String meldung = returnText.substring(0,3); // erste 3 Zeichen
  if (DEBUG_FTP) Serial.println("Return: " + meldung);

  if (meldung == ftp_ret[index])
  {
    return true;
  }
  else  
  {
    return false;
  }
}
