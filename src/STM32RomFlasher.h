/********************************************************************************************************
 *  [FILE NAME]   :      <STM32RomFlasher.h>                                                            *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for STM32 ROM Flasher>                                            *
 ********************************************************************************************************/

#ifndef STM32_ROM_FLASHER_H
#define	STM32_ROM_FLASHER_H


#include <Arduino.h>
#include "STM32RomBootloader.h"
#include "STM32FamilyDb.h"

class STM32RomFlasher
{
	public:
	STM32RomFlasher(Stream& io, uint8_t boot0Pin, uint8_t resetPin);
	void beginPins();
	void disconnect();
	void enterRomBootloader();
	void exitToUserApp();

	bool detect(String& desc, String& err);

	bool massErase(String& err);
	bool flashBuffer(uint32_t addr, const uint8_t* data, size_t len, String& err);
	bool testRam(String& out);

	bool readFlashSizeKB(uint16_t& outKb, String& err);

	bool isConnected() const;
	uint16_t devId() const;
	uint16_t flashKb() const;
	uint8_t eraseCmd() const;
	uint32_t eraseTimeoutMs() const;
	uint32_t flashStart() const;
	uint32_t sramTestAddr() const;
	String desc() const;

	STM32FamilyInfo familyInfo() const;

	private:
	Stream* _io;
	uint8_t _boot0;
	uint8_t _reset;

	STM32RomBootloader _bl;
	STM32FamilyInfo _fi;

	bool _connected;
	uint16_t _devId;
	uint16_t _flashKb;
	uint8_t _eraseCmd;
	uint32_t _eraseTimeout;
	uint32_t _flashStart;
	uint32_t _sramAddr;
	String _desc;

	bool computeEraseFromSupported(const uint8_t* cmds, size_t n, uint8_t& eraseCmdOut);
};

#endif	/* STM32_ROM_FLASHER_H */