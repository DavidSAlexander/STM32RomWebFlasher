/********************************************************************************************************
 *  [FILE NAME]   :      <STM32WebFlasherConfig.h>                                                      *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for STM32 Configurations>                                         *
 ********************************************************************************************************/

#ifndef STM32_FOTA_CONFIG_H
#define	STM32_FOTA_CONFIG_H

#include <Arduino.h>

struct STM32WebFlasherConfig
{
	const char* wifiSsid;
	const char* wifiPass;

	const char* mdnsHost;

	const char* webUser;
	const char* webPass;

	uint16_t httpPort;

	uint8_t boot0Pin;
	uint8_t resetPin;

	uint32_t uartBaud;
	bool uartSwap;

	uint32_t syncTimeoutMs;

	const char* updatePath;

	STM32WebFlasherConfig()
	: wifiSsid(""),
	wifiPass(""),
	mdnsHost("stm32flasher"),
	webUser("admin"),
	webPass("admin"),
	httpPort(80),
	boot0Pin(0xFF),
	resetPin(0xFF),
	uartBaud(115200),
	uartSwap(true),
	syncTimeoutMs(1000),
	updatePath("/update.bin")
	{}

	STM32WebFlasherConfig(
	const char* ssid,
	const char* pass,
	const char* mdns,
	const char* user,
	const char* pw,
	uint16_t port,
	uint8_t boot0,
	uint8_t rst,
	uint32_t baud,
	bool swapUart,
	uint32_t syncTo,
	const char* path
	)
	: wifiSsid(ssid),
	wifiPass(pass),
	mdnsHost(mdns),
	webUser(user),
	webPass(pw),
	httpPort(port),
	boot0Pin(boot0),
	resetPin(rst),
	uartBaud(baud),
	uartSwap(swapUart),
	syncTimeoutMs(syncTo),
	updatePath(path)
	{}
};

#endif	/* STM32_FOTA_CONFIG_H */