/********************************************************************************************************
 *  [FILE NAME]   :      <STM32RomFlasher.cpp>                                                          *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Source file for STM32 ROM Flasher>                                            *
 ********************************************************************************************************/


#include "STM32RomFlasher.h"

STM32RomFlasher::STM32RomFlasher(Stream& io, uint8_t boot0Pin, uint8_t resetPin)
: _io(&io),
_boot0(boot0Pin),
_reset(resetPin),
_bl(io),
_fi((STM32FamilyInfo){STM32_UNKNOWN, 0, 0, F1_FLASH_SIZE_ADDR, 15000, 0x43, true, 0x08000000, 0x20000200, "STM32 Unknown"}),
_connected(false),
_devId(0),
_flashKb(0),
_eraseCmd(0x43),
_eraseTimeout(15000),
_flashStart(STM32_FLASH_START_DEFAULT),
_sramAddr(0x20000200),
_desc("")
{
}

void STM32RomFlasher::beginPins()
{
	pinMode(_boot0, OUTPUT);
	pinMode(_reset, OUTPUT);
	digitalWrite(_boot0, LOW);
	digitalWrite(_reset, HIGH);
}

void STM32RomFlasher::enterRomBootloader()
{
	digitalWrite(_boot0, HIGH);
	digitalWrite(_reset, LOW);
	delay(50);
	digitalWrite(_reset, HIGH);
	delay(120);
	_bl.clearRx();
}

void STM32RomFlasher::exitToUserApp()
{
	digitalWrite(_boot0, LOW);
	digitalWrite(_reset, LOW);
	delay(50);
	digitalWrite(_reset, HIGH);
	delay(120);
}

bool STM32RomFlasher::computeEraseFromSupported(const uint8_t* cmds, size_t n, uint8_t& eraseCmdOut)
{
	bool has43 = false;
	bool has44 = false;
	for (size_t i = 0; i < n; i++)
	{
		if (cmds[i] == 0x43) has43 = true;
		if (cmds[i] == 0x44) has44 = true;
	}
	if (has44) { eraseCmdOut = 0x44; return true; }
	if (has43) { eraseCmdOut = 0x43; return true; }
	return false;
}

bool STM32RomFlasher::detect(String& desc, String& err)
{
	err = "";
	desc = "";
	_connected = false;

	enterRomBootloader();

	if (!_bl.sync(1000))
	{
		exitToUserApp();
		err = "SYNC failed";
		return false;
	}

	uint16_t dev;
	if (!_bl.getId(dev, err))
	{
		exitToUserApp();
		return false;
	}

	uint8_t cmds[64];
	size_t cmdCount = 0;
	uint8_t proto = 0;
	String getErr;
	if (!_bl.getSupportedCommands(cmds, cmdCount, proto, getErr))
	{
		exitToUserApp();
		err = getErr;
		return false;
	}

	STM32FamilyInfo fi = STM32FamilyDb::getFamilyInfo(dev);

	uint8_t eraseDetected = fi.eraseCmd;
	computeEraseFromSupported(cmds, cmdCount, eraseDetected);

	fi.eraseCmd = eraseDetected;
	fi.supportsGlobalErase = (fi.eraseCmd == 0x43);

	_fi = fi;
	_devId = dev;
	_flashKb = fi.FlashSize;
	_eraseCmd = fi.eraseCmd;
	_eraseTimeout = fi.eraseTimeout;
	_flashStart = fi.flashStart;
	_sramAddr = fi.sramTestAddr;

	_desc = String(fi.name) + " (ID: 0x" + String(dev, HEX) + ", Flash: " + String(_flashKb) + "KB)";
	desc = _desc;
	_connected = true;
	return true;
}

bool STM32RomFlasher::massErase(String& err)
{
	err = "";
	enterRomBootloader();
	if (!_bl.sync(1000))
	{
		exitToUserApp();
		err = "SYNC failed";
		return false;
	}
	if (!_bl.massErase(_eraseCmd, _eraseTimeout, err))
	{
		exitToUserApp();
		return false;
	}
	return true;
}

bool STM32RomFlasher::flashBuffer(uint32_t addr, const uint8_t* data, size_t len, String& err)
{
	err = "";
	enterRomBootloader();
	if (!_bl.sync(1000))
	{
		exitToUserApp();
		err = "SYNC failed";
		return false;
	}
	return _bl.writeMemory(addr, data, len, err, STM32_CHUNK);
}

bool STM32RomFlasher::testRam(String& out)
{
	out = "";
	String err;

	enterRomBootloader();
	if (!_bl.sync(1000))
	{
		exitToUserApp();
		out = "SYNC failed";
		return false;
	}

	uint8_t tx[16];
	for (int i = 0; i < 16; i++) tx[i] = (uint8_t)(0x10 + i);

	if (!_bl.writeMemory(_sramAddr, tx, sizeof(tx), err))
	{
		exitToUserApp();
		out = "RAM write failed: " + err;
		return false;
	}

	uint8_t rx[16];
	if (!_bl.readMemory(_sramAddr, rx, sizeof(rx), err))
	{
		exitToUserApp();
		out = "RAM read failed: " + err;
		return false;
	}

	for (int i = 0; i < 16; i++)
	{
		if (rx[i] != tx[i])
		{
			out = "RAM mismatch at index " + String(i);
			return false;
		}
	}

	out = "RAM Write/Read at 0x" + String(_fi.sramTestAddr ,HEX) + " OK";
	return true;
}

bool STM32RomFlasher::readFlashSizeKB(uint16_t& outKb, String& err)
{
	err = "";
	outKb = 0;

	enterRomBootloader();
	if (!_bl.sync(1000))
	{
		exitToUserApp();
		err = "SYNC failed";
		return false;
	}

	uint8_t b[2];
	if (!_bl.readMemory(_fi.flashSizeAddr, b, 2, err))
	{
		exitToUserApp();
		return false;
	}

	outKb = (uint16_t)b[0] | ((uint16_t)b[1] << 8);
	return true;
}

void STM32RomFlasher::disconnect()
{
	exitToUserApp();
	_connected = false;
	_desc = "";
	_devId = 0;
	_flashKb = 0;
}

bool STM32RomFlasher::isConnected() const { return _connected; }
uint16_t STM32RomFlasher::devId() const { return _devId; }
uint16_t STM32RomFlasher::flashKb() const { return _flashKb; }
uint8_t STM32RomFlasher::eraseCmd() const { return _eraseCmd; }
uint32_t STM32RomFlasher::eraseTimeoutMs() const { return _eraseTimeout; }
uint32_t STM32RomFlasher::flashStart() const { return _flashStart; }
uint32_t STM32RomFlasher::sramTestAddr() const { return _sramAddr; }
String STM32RomFlasher::desc() const { return _desc; }
STM32FamilyInfo STM32RomFlasher::familyInfo() const { return _fi; }
