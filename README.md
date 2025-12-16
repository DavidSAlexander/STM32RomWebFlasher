# STM32RomWebFlasher

ESP8266-based web flasher for STM32 built-in **ROM (System Memory) Bootloader** over UART.

This library turns an ESP8266 into a standalone “browser flasher”:
- Connect a target STM32 (UART + BOOT0 + NRST)
- Open a web UI in your browser
- Upload `update.bin` to ESP8266 LittleFS
- Run bootloader commands (detect/connect, erase, program, jump to app)

---

## Highlights

- **Browser-only workflow**: upload + flash from any device on the same WiFi/LAN.
- **Uses STM32 ROM bootloader USART protocol** (ACK/NACK-based command frames).
- **Embedded UI**: firmware upload, control buttons, command log.
- **mDNS access**: `http://<mdns-host>.local/`
- **LittleFS storage**: stores firmware file on ESP8266 (default `/update.bin`).
- **Simple access control**: username/password + session bound to client IP (LAN use).

---

## Compatibility

- **Supported**: ESP8266 (Arduino core)
- **ESP32**: not supported as-is. It can be ported, but requires a separate implementation layer (different web server, mDNS, FS, and serial handling).

---

## Hardware requirements

### Minimum wiring
- ESP8266 TX → STM32 RX (cross)
- ESP8266 RX → STM32 TX (cross)
- GND ↔ GND
- ESP8266 GPIO → STM32 BOOT0 (recommended with proper driver/level shifting)
- ESP8266 GPIO → STM32 NRST (recommended with proper driver/open-drain transistor)

### Notes
- ESP8266 logic is **3.3V**. Avoid direct 5V UART.
- Many STM32 boards need **BOOT0 = 1 during reset** to enter system bootloader.
- Many STM32 ROM bootloaders use **8E1** (8 data bits, even parity, 1 stop bit).

---

## Installation (Arduino IDE)

### Folder placement
Put the library folder here (Windows):
C:\Users<you>\Documents\Arduino\libraries\STM32RomWebFlasher\

Inside it you should have:
STM32RomWebFlasher/
library.properties
src/
examples/


### Install methods
- **ZIP**: Sketch → Include Library → Add .ZIP Library… (zip must contain the library root)
- **Manual**: copy the folder to `Documents/Arduino/libraries/`
- Restart Arduino IDE after installing.

---

## Quick start (ESP8266 example)

Create a sketch like this:

```cpp
#include <Arduino.h>
#include <STM32WebFlasherESP8266.h>

static const char* WIFI_SSID = "YOUR_SSID";
static const char* WIFI_PASS = "YOUR_PASS";
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
```

## Open the web UI

- Via IP: `http://<esp-ip>/`
- Via mDNS: `http://stm32flasher.local/`

---

## How the web UI works

1. **Login**
2. **Connect Target**
   - ESP8266 drives **BOOT0/NRST** and attempts ROM bootloader **sync/detect**
3. **Upload firmware**
   - Upload a `.bin` file; it is saved to LittleFS as `/update.bin`
4. **Run commands**
   - Erase / Program / Full Update / Jump to App / Read Info

---

## Command mapping (UI ↔ device)

The UI sends: `GET /cmd?c=<letter>`

| Code | Button label | Meaning |
|---:|---|---|
| `S` | Full Update | Erase + Program + Jump to app |
| `E` | Erase Only | Mass erase (if supported) |
| `U` | Program Only | Program `/update.bin` to flash |
| `J` | Reset to App | Exit bootloader / jump to user app |
| `G` | Read Chip ID | Reads device ID (implementation-dependent) |
| `R` | Bootloader Version | Reads ROM bootloader protocol version |
| `C` | Get Commands | Reads supported bootloader commands (implementation-dependent) |
| `T` | Test RAM Write | Writes a test block to RAM to verify link |

---

## HTTP endpoints

### `GET /`
Returns the Login page or Main UI depending on session.

### `POST /login`
Body: `user=<...>&pass=<...>`  
JSON:
- `{ "ok": true }`
- `{ "ok": false, "error": "..." }`

### `POST /logout`
Logs out and clears session.

### `POST /connect`
Tries to enter ROM bootloader and detect target.

### `POST /disconnect`
Exits bootloader / jumps to application.

### `GET /status`
JSON status (connected, hasFile, flashKB, devId, desc).

### `POST /upload`
Multipart firmware upload → saved to LittleFS as `/update.bin`.

### `GET /cmd?c=X`
Runs command `X`.

---

## Public API

### `STM32WebFlasherConfig`

Constructor:

```cpp
STM32WebFlasherConfig(
  const char* wifiSsid,
  const char* wifiPass,
  const char* mdnsHost,
  const char* webUser,
  const char* webPass,
  uint16_t httpPort,
  uint8_t boot0Pin,
  uint8_t resetPin,
  uint32_t uartBaud,
  bool uartSwap,
  uint32_t syncTimeoutMs,
  const char* updatePath
);
```

---

## Configuration fields

### `STM32WebFlasherConfig` fields

- `wifiSsid`, `wifiPass`  
  Wi-Fi network credentials.

- `mdnsHost`  
  The `.local` mDNS host name (example: `stm32flasher` → `stm32flasher.local`).

- `webUser`, `webPass`  
  Web UI login credentials.

- `httpPort`  
  HTTP server port (usually `80`).

- `boot0Pin`  
  ESP GPIO pin connected to STM32 **BOOT0** (controls boot mode).

- `resetPin`  
  ESP GPIO pin connected to STM32 **NRST** (controls reset).

- `uartBaud`  
  UART baud rate for STM32 ROM bootloader communication.

- `uartSwap`  
  ESP8266 UART pin swap enable/disable.

- `syncTimeoutMs`  
  Timeout for sync/ACK operations (milliseconds).

- `updatePath`  
  Firmware file path inside LittleFS (example: `"/update.bin"`).

---

## Library classes

### `STM32WebFlasherESP8266`

Constructor and main methods:

```cpp
STM32WebFlasherESP8266(HardwareSerial& serial, const STM32WebFlasherConfig& cfg);
bool begin();
void loop();
```

## Behavior

- `begin()` configures WiFi, LittleFS, mDNS, web routes, and UART.  
- `loop()` must be called continuously.

---

## Common problems and fixes: `Missing final ACK`

This is a **protocol-level** failure: the STM32 did not ACK the last expected step.

### Most common causes
- BOOT0 not asserted correctly during reset  
- NRST timing incorrect  
- Wrong UART settings (many STM32 need **8E1 parity**)  
- TX/RX swapped or missing common GND  
- Target not actually in system bootloader mode  
- Baud mismatch (try `57600` / `38400`)  

### Recommended checks
- BOOT0 HIGH → pulse NRST → attempt sync  
- Confirm correct STM32 bootloader UART pins for your MCU  
- Confirm ESP UART config matches the bootloader (including parity)  
- Validate hardware with STM32CubeProgrammer or `stm32flash` first  

---

## References
- STM32 ROM bootloader commands overview (ST Community):  
  https://community.st.com/t5/stm32-mcus/how-to-utilize-stm32-system-rom-bootloader-commands/ta-p/605286

- AN3155 — USART protocol used in the STM32 bootloader (ST Application Note):  
  https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf

