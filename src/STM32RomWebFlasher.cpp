/********************************************************************************************************
 *  [FILE NAME]   :      <STM32RomWebFlasher.cpp>                                                       *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Source file for STM32 Web Flasher for ESP8266>                                *
 ********************************************************************************************************/

#ifdef ESP8266
#include "STM32RomWebFlasher.h"

STM32WebFlasherESP8266::STM32WebFlasherESP8266(HardwareSerial& serial, const STM32WebFlasherConfig& cfg)
: _serial(&serial),
_cfg(cfg),
_server(cfg.httpPort),
_flasher(serial, cfg.boot0Pin, cfg.resetPin),
_loggedIn(false),
_loggedIp(0,0,0,0)
{
}

void STM32WebFlasherESP8266::sendHtml_P(const char* pgmHtml)
{
	_server.send_P(200, "text/html", pgmHtml);
}

void STM32WebFlasherESP8266::sendHtmlWithHost_P(const char* partA, const char* host, const char* partB)
{
	_server.setContentLength(CONTENT_LENGTH_UNKNOWN);
	_server.send(200, "text/html", "");
	if (partA) _server.sendContent_P(partA);
	_server.sendContent(host ? host : "");
	if (partB) _server.sendContent_P(partB);
}

bool STM32WebFlasherESP8266::requireLogin()
{
	if (!_loggedIn) return false;
	if (_server.client().remoteIP() != _loggedIp) return false;
	return true;
}

bool STM32WebFlasherESP8266::begin()
{
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.setSleepMode(WIFI_NONE_SLEEP);

	_flasher.beginPins();

	_serial->begin(_cfg.uartBaud, SERIAL_8E1);
	if (_cfg.uartSwap) _serial->swap();

	if (!LittleFS.begin()) return false;

	WiFi.begin(_cfg.wifiSsid, _cfg.wifiPass);
	uint32_t start = millis();
	while (WiFi.status() != WL_CONNECTED)
	{
		if (millis() - start > 20000) break;
		delay(50);
		yield();
	}
	if (WiFi.status() != WL_CONNECTED) return false;

	if (!MDNS.begin(_cfg.mdnsHost)) return false;
	MDNS.addService("http", "tcp", _cfg.httpPort);

	_server.on("/", HTTP_GET, [this](){ routeRoot(); });
	_server.onNotFound([this](){ routeNotFound(); });

	_server.on("/upload", HTTP_POST,
	[this](){
		if (!requireLogin()) { _server.send(403, "text/plain", "Not logged in"); return; }
		_server.send(200, "text/plain", "Upload OK");
	},
	[this](){ routeUpload(); }
	);

	_server.on("/cmd", HTTP_GET, [this](){ routeCmd(); });
	_server.on("/status", HTTP_GET, [this](){ routeStatus(); });
	_server.on("/connect", HTTP_POST, [this](){ routeConnect(); });
	_server.on("/disconnect", HTTP_POST, [this](){ routeDisconnect(); });
	_server.on("/login", HTTP_POST, [this](){ routeLogin(); });
	_server.on("/logout", HTTP_POST, [this](){ routeLogout(); });

	_server.begin();
	return true;
}

void STM32WebFlasherESP8266::loop()
{
	_server.handleClient();
	MDNS.update();
	yield();
}

void STM32WebFlasherESP8266::routeRoot()
{
	if (!requireLogin())
	{
		sendHtmlWithHost_P(LOGIN_HTML_A, _cfg.mdnsHost, LOGIN_HTML_B);
		return;
	}
	sendHtmlWithHost_P(INDEX_HTML_A, _cfg.mdnsHost, INDEX_HTML_B);
}

void STM32WebFlasherESP8266::routeNotFound()
{
	_server.send(404, "text/plain", "Not found");
}

void STM32WebFlasherESP8266::routeUpload()
{
	if (!requireLogin()) return;

	HTTPUpload& upload = _server.upload();
	if (upload.status == UPLOAD_FILE_START)
	{
		if (LittleFS.exists(_cfg.updatePath)) LittleFS.remove(_cfg.updatePath);
		_uploadFile = LittleFS.open(_cfg.updatePath, "w");
	}
	else if (upload.status == UPLOAD_FILE_WRITE)
	{
		if (_uploadFile) _uploadFile.write(upload.buf, upload.currentSize);
	}
	else if (upload.status == UPLOAD_FILE_END)
	{
		if (_uploadFile) _uploadFile.close();
	}
}

void STM32WebFlasherESP8266::routeCmd()
{
	if (!requireLogin()) { _server.send(403, "text/plain", "Not logged in"); return; }
	if (!_server.hasArg("c")) { _server.send(400, "text/plain", "Missing c"); return; }

	String arg = _server.arg("c");
	if (arg.length() == 0) { _server.send(400, "text/plain", "Empty c"); return; }

	char c = arg[0];

	if (!_flasher.isConnected() && (c == 'S' || c == 'E' || c == 'U' || c == 'J'))
	{
		_server.send(400, "text/plain", "Target not connected. Use Connect first.");
		return;
	}

	if (c == 'C')
	{
		STM32RomBootloader bl(*_serial);
		_flasher.enterRomBootloader();

		if (!bl.sync(1000))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", "SYNC failed");
			return;
		}

		String out, err;
		if (!bl.getSupportedCommandsText(out, err))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", err);
			return;
		}

		_flasher.exitToUserApp();
		_server.send(200, "text/plain", out);
		return;
	}

	if (c == 'G')
	{
		STM32RomBootloader bl(*_serial);
		_flasher.enterRomBootloader();

		if (!bl.sync(1000))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", "SYNC failed");
			return;
		}

		uint16_t id = 0;
		String err;
		if (!bl.getId(id, err))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", err);
			return;
		}

		_flasher.exitToUserApp();

		char tmp[32];
		snprintf(tmp, sizeof(tmp), "Chip ID = 0x%03X", id);
		_server.send(200, "text/plain", String(tmp));
		return;
	}

	if (c == 'R')
	{
		STM32RomBootloader bl(*_serial);
		_flasher.enterRomBootloader();

		if (!bl.sync(1000))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", "SYNC failed");
			return;
		}

		uint8_t v, o1, o2;
		String err;
		if (!bl.getVersion(v, o1, o2, err))
		{
			_flasher.exitToUserApp();
			_server.send(200, "text/plain", err);
			return;
		}

		_flasher.exitToUserApp();

		char tmp[64];
		snprintf(tmp, sizeof(tmp), "Protocol version = 0x%02X, Opt1 = 0x%02X, Opt2 = 0x%02X", v, o1, o2);
		_server.send(200, "text/plain", String(tmp));
		return;
	}

	if (c == 'E')
	{
		String err;
		bool ok = _flasher.massErase(err);
		_server.send(200, "text/plain", ok ? "Erase OK" : ("Erase failed: " + err));
		return;
	}

	if (c == 'T')
	{
		String out;
		_flasher.testRam(out);
		_server.send(200, "text/plain", out);
		return;
	}

	if (c == 'U' || c == 'S')
	{
		if (!LittleFS.exists(_cfg.updatePath))
		{
			_server.send(200, "text/plain", String("No ") + _cfg.updatePath);
			return;
		}

		String err;
		if (c == 'S')
		{
			bool okErase = _flasher.massErase(err);
			if (!okErase) { _server.send(200, "text/plain", "Erase failed: " + err); return; }
		}

		File f = LittleFS.open(_cfg.updatePath, "r");
		if (!f) { _server.send(200, "text/plain", "Open update failed"); return; }

		STM32RomBootloader bl(*_serial);
		_flasher.enterRomBootloader();
		if (!bl.sync(1000)) { f.close(); _flasher.exitToUserApp(); _server.send(200, "text/plain", "SYNC failed"); return; }

		uint32_t addr = _flasher.flashStart();
		uint8_t buf[STM32_CHUNK];
		size_t total = 0;

		while (true)
		{
			size_t n = f.read(buf, STM32_CHUNK);
			if (n == 0) break;

			if (!bl.writeMemory(addr, buf, n, err))
			{
				f.close();
				_flasher.exitToUserApp();
				char tmp[160];
				snprintf(tmp, sizeof(tmp), "Write error at 0x%08lX: %s", (unsigned long)addr, err.c_str());
				_server.send(200, "text/plain", String(tmp));
				return;
			}

			addr += (uint32_t)((n + 3) & ~((size_t)3));
			total += n;
			yield();
		}

		f.close();
		_flasher.exitToUserApp();
		_server.send(200, "text/plain", String("Upload OK, Bytes = ") + String((unsigned long)total));
		return;
	}

	if (c == 'J')
	{
		_flasher.exitToUserApp();
		_server.send(200, "text/plain", "Jump to application.");
		return;
	}

	_server.send(200, "text/plain", String("Unknown CMD: ") + c);
}


void STM32WebFlasherESP8266::routeStatus()
{
	if (!requireLogin())
	{
		_server.send(403, "application/json", "{\"ok\":false,\"error\":\"not logged in\"}");
		return;
	}

	bool hasFile = LittleFS.exists(_cfg.updatePath);
	String json = "{";
		json += "\"ok\":true";
		json += ",\"connected\":";
		json += _flasher.isConnected() ? "true" : "false";
		json += ",\"desc\":\"";
		json += _flasher.isConnected() ? _flasher.desc() : "";
		json += "\"";
		json += ",\"hasFile\":";
		json += hasFile ? "true" : "false";
		json += ",\"flashKB\":";
		json += _flasher.flashKb();
		json += ",\"devId\":";
		json += _flasher.devId();
	json += "}";
	_server.send(200, "application/json", json);
}

void STM32WebFlasherESP8266::routeConnect()
{
	if (!requireLogin()) { _server.send(403, "application/json", "{\"ok\":false,\"error\":\"not logged in\"}"); return; }

	String desc, err;
	bool ok = _flasher.detect(desc, err);
	if (ok)
	{
		String json = "{\"ok\":true,\"connected\":true,\"desc\":\"" + desc + "\"}";
		_server.send(200, "application/json", json);
	}
	else
	{
		String json = "{\"ok\":false,\"connected\":false,\"error\":\"" + err + "\"}";
		_server.send(200, "application/json", json);
	}
}

void STM32WebFlasherESP8266::routeDisconnect()
{
	if (!requireLogin()) { _server.send(403, "application/json", "{\"ok\":false,\"error\":\"not logged in\"}"); return; }
	_flasher.disconnect();
	_server.send(200, "application/json", "{\"ok\":true,\"connected\":false}");
}


void STM32WebFlasherESP8266::routeLogin()
{
	if (!_server.hasArg("user") || !_server.hasArg("pass"))
	{
		_server.send(400, "application/json", "{\"ok\":false,\"error\":\"Missing credentials\"}");
		return;
	}

	String u = _server.arg("user");
	String p = _server.arg("pass");

	if (u == _cfg.webUser && p == _cfg.webPass)
	{
		_loggedIn = true;
		_loggedIp = _server.client().remoteIP();
		_server.send(200, "application/json", "{\"ok\":true}");
	}
	else
	{
		_loggedIn = false;
		_loggedIp = IPAddress(0,0,0,0);
		_server.send(200, "application/json", "{\"ok\":false,\"error\":\"Invalid username or password\"}");
	}
}

void STM32WebFlasherESP8266::routeLogout()
{
	_loggedIn = false;
	_loggedIp = IPAddress(0,0,0,0);
	_flasher.exitToUserApp();
	_server.send(200, "application/json", "{\"ok\":true}");
}

#endif
