/********************************************************************************************************
 *  [FILE NAME]   :      <ESP8266_WebFlasher.ino>														                            *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Example code for ESP8266 to run the STM32 ROM Bootloader>					            *
 ********************************************************************************************************/

#include <STM32RomWebFlasher.h>

static const char* WIFI_SSID = "";
static const char* WIFI_PASS = "";
static const char* MDNS_HOST = "stm32flasher";

static const char* WEB_USER = "admin";
static const char* WEB_PASS = "admin";

STM32WebFlasherConfig cfg = 
{
  WIFI_SSID,                 // WiFi SSID
  WIFI_PASS,                 // WiFi PASSWORD
  MDNS_HOST,                 // Web Accessable Address http://MDNS_HOST.local
  WEB_USER,                  // Webpage Login Username
  WEB_PASS,                  // Webpage Login Password
  80,                        // Port
  D1,                        // STM32 Target BOOT0 Pin
  D2,                        // STM32 Target Reset Pin
  115200,                    // UART Baudrate
  true,                      // UART Swap feature
  1000,                      // Sync Timeout
  "/update.bin"              // Firmware Update Path
};

STM32WebFlasherESP8266 Flasher(Serial, cfg);

void setup()
{
  if (!Flasher.begin())
  {
    while (true) delay(1000);
  }
}

void loop()
{
  Flasher.loop();
}
