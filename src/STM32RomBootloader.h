/********************************************************************************************************
 *  [FILE NAME]   :      <STM32RomBootloader.h>                                                         *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for STM32 ROM Bootloader>                                         *
 ********************************************************************************************************/

#ifndef STM32_ROM_BOOTLOADER_H
#define	STM32_ROM_BOOTLOADER_H


#include <Arduino.h>
#include "STM32DeviceConstants.h"

class STM32RomBootloader
{
	public:
	explicit STM32RomBootloader(Stream& io);

	void clearRx();
	bool sync(uint32_t timeoutMs);

	bool getId(uint16_t& devId, String& err);
	bool getVersion(uint8_t& ver, uint8_t& opt1, uint8_t& opt2, String& err);
	bool getSupportedCommandsText(String& out, String& err);
	bool getSupportedCommands(uint8_t* out, size_t& outCount, uint8_t& proto, String& err);

	bool readMemory(uint32_t addr, uint8_t* buf, size_t len, String& err);
	bool writeMemory(uint32_t addr, const uint8_t* data, size_t len, String& err, size_t chunk = STM32_CHUNK);

	bool massErase(uint8_t eraseCmd, uint32_t eraseTimeoutMs, String& err);

	private:
	Stream* _io;

	bool readByteTimeout(uint8_t& b, uint32_t timeoutMs);
	bool waitAck(uint32_t timeoutMs, uint8_t& resp);
	bool waitAckSimple(uint32_t timeoutMs);

	bool sendCmdByte(uint8_t cmd, uint8_t& resp);
	bool sendAddress(uint32_t addr);

	bool writeChunk(uint32_t addr, const uint8_t* data, size_t len, String& err);
};


#endif	/* STM32_ROM_BOOTLOADER_H */