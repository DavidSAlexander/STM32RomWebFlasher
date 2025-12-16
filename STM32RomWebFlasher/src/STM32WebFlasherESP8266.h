/********************************************************************************************************
 *  [FILE NAME]   :      <STM32WebFlasherESP8266.h>                                                     *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for STM32 Web Flasher for ESP8266>                                *
 ********************************************************************************************************/

#ifndef STM32_FOTA_ESP_H
#define	STM32_FOTA_ESP_H


#ifdef ESP8266

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <LittleFS.h>

#include "STM32WebPages.h"
#include "STM32WebFlasherConfig.h"
#include "STM32RomFlasher.h"

class STM32WebFlasherESP8266
{
	public:
	STM32WebFlasherESP8266(HardwareSerial& serial, const STM32WebFlasherConfig& cfg);

	bool begin();
	void loop();

	private:
	void sendHtml_P(const char* pgmHtml);
	void sendHtmlWithHost_P(const char* partA, const char* host, const char* partB);

	bool requireLogin();

	void routeRoot();
	void routeNotFound();
	void routeUpload();
	void routeCmd();
	void routeStatus();
	void routeConnect();
	void routeDisconnect();
	void routeLogin();
	void routeLogout();

	private:
	HardwareSerial* _serial;
	STM32WebFlasherConfig _cfg;
	ESP8266WebServer _server;

	STM32RomFlasher _flasher;

	bool _loggedIn;
	IPAddress _loggedIp;

	File _uploadFile;
};

#endif

#endif	/* STM32_FOTA_ESP_H */