/********************************************************************************************************
 *  [FILE NAME]   : ESP8266_WebFlasher.ino
 *  [AUTHOR]      : David S. Alexander
 *  [DATE CREATED]: Jan 10, 2026
 *  [Description] : ESP8266 STM32 ROM Web Flasher with WiFiManager + Double Reset Configuration
 ********************************************************************************************************/

#include <STM32RomWebFlasher.h>

#include <WiFiManager.h>
#include <EEPROM.h>
#include <DoubleResetDetect.h>

/* ===================== Double Reset ===================== */
#define DRD_TIMEOUT  3
#define DRD_ADDRESS  0

DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

/* ===================== EEPROM ===================== */
#define EEPROM_SIZE  512
#define CONFIG_MAGIC 0xA5A5

/* ===================== Configuration Structure ===================== */
struct FlasherConfig
{
  uint16_t magic;

  char mdns_host[32];
  char web_user[16];
  char web_pass[16];

  uint16_t port;

  uint8_t  boot0_pin;     // ESP8266 GPIO number
  uint8_t  reset_pin;     // ESP8266 GPIO number
  uint32_t uart_baud;
  bool     uart_swap;
  uint16_t sync_timeout;

  char firmware_path[32];
};

FlasherConfig cfgData;

/* ===================== Defaults ===================== */
void loadDefaults()
{
  strcpy(cfgData.mdns_host,     "STM32Flasher");
  strcpy(cfgData.web_user,      "admin");
  strcpy(cfgData.web_pass,      "admin");
  strcpy(cfgData.firmware_path, "/update.bin");

  cfgData.port         = 80;

  cfgData.boot0_pin    = 5;   // D1
  cfgData.reset_pin    = 4;   // D2
  cfgData.uart_baud    = 115200;
  cfgData.uart_swap    = true;
  cfgData.sync_timeout = 1000;
}

/* ===================== EEPROM ===================== */
void loadConfig()
{
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, cfgData);

  if (cfgData.magic != CONFIG_MAGIC)
  {
    loadDefaults();
    cfgData.magic = CONFIG_MAGIC;
    EEPROM.put(0, cfgData);
    EEPROM.commit();
  }
}

void saveConfig()
{
  EEPROM.put(0, cfgData);
  EEPROM.commit();
}

/* ===================== WiFiManager ===================== */
WiFiManager wm;

/* Custom Parameters */
WiFiManagerParameter p_mdns     ("mdns",  "mDNS Host",       "STM32Flasher", 31);
WiFiManagerParameter p_user     ("user",  "Web User",        "admin", 15);
WiFiManagerParameter p_pass     ("pass",  "Web Password",    "admin", 15);
WiFiManagerParameter p_uart     ("baud",  "UART Baud",       "115200", 10);
WiFiManagerParameter p_path     ("path",  "FW Path",         "/update.bin", 31);

WiFiManagerParameter p_boot0    ("boot0", "BOOT0 GPIO",      "5", 3);
WiFiManagerParameter p_reset    ("reset", "RESET GPIO",      "4", 3);
WiFiManagerParameter p_swap     ("swap",  "UART Swap (0/1)", "1", 2);

/* ===================== Config Portal ===================== */
void startConfigPortal()
{
  wm.resetSettings();

  wm.addParameter(&p_mdns);
  wm.addParameter(&p_user);
  wm.addParameter(&p_pass);
  wm.addParameter(&p_uart);
  wm.addParameter(&p_path);

  wm.addParameter(&p_boot0);
  wm.addParameter(&p_reset);
  wm.addParameter(&p_swap);

  if (!wm.startConfigPortal("STM32-Flasher-Setup", "DavidAlex"))
  {
    ESP.restart();
  }

  strncpy(cfgData.mdns_host,     p_mdns.getValue(), sizeof(cfgData.mdns_host));
  strncpy(cfgData.web_user,      p_user.getValue(), sizeof(cfgData.web_user));
  strncpy(cfgData.web_pass,      p_pass.getValue(), sizeof(cfgData.web_pass));
  strncpy(cfgData.firmware_path, p_path.getValue(), sizeof(cfgData.firmware_path));

  cfgData.uart_baud = atoi(p_uart.getValue());
  cfgData.boot0_pin = atoi(p_boot0.getValue());
  cfgData.reset_pin = atoi(p_reset.getValue());
  cfgData.uart_swap = atoi(p_swap.getValue()) ? true : false;

  saveConfig();
}

/* ===================== STM32 Web Flasher ===================== */
STM32WebFlasherConfig flasherCfg;
STM32WebFlasherESP8266 *Flasher;

/* ===================== Setup ===================== */
void setup()
{
  Serial.begin(115200);
  delay(100);

  loadConfig();

  if (drd.detect())
  {
    startConfigPortal();
  }
  else
  {
    if (!wm.autoConnect("STM32-Flasher", "DavidAlex"))
    {
      startConfigPortal();
    }
  }

  flasherCfg =
  {
    WiFi.SSID().c_str(),
    WiFi.psk().c_str(),
    cfgData.mdns_host,
    cfgData.web_user,
    cfgData.web_pass,
    cfgData.port,
    cfgData.boot0_pin,
    cfgData.reset_pin,
    cfgData.uart_baud,
    cfgData.uart_swap,
    cfgData.sync_timeout,
    cfgData.firmware_path
  };

  Flasher = new STM32WebFlasherESP8266(Serial, flasherCfg);

  if (!Flasher->begin())
  {
    while (true) delay(1000);
  }
}

/* ===================== Loop ===================== */
void loop()
{
  Flasher->loop();
}
