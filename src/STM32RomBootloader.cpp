/********************************************************************************************************
 *  [FILE NAME]   :      <STM32RomBootloader.cpp>                                                       *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Source file for STM32 ROM Bootloader>                                         *
 ********************************************************************************************************/


#include "STM32RomBootloader.h"

STM32RomBootloader::STM32RomBootloader(Stream& io) : _io(&io) {}

void STM32RomBootloader::clearRx()
{
	while (_io->available()) _io->read();
}

bool STM32RomBootloader::readByteTimeout(uint8_t& b, uint32_t timeoutMs)
{
	uint32_t start = millis();
	while (!_io->available())
	{
		if (millis() - start >= timeoutMs) return false;
		yield();
	}
	b = (uint8_t)_io->read();
	return true;
}

bool STM32RomBootloader::waitAck(uint32_t timeoutMs, uint8_t& resp)
{
	if (!readByteTimeout(resp, timeoutMs)) return false;
	return (resp == STM32_ACK);
}

bool STM32RomBootloader::waitAckSimple(uint32_t timeoutMs)
{
	uint8_t r;
	return waitAck(timeoutMs, r);
}

bool STM32RomBootloader::sync(uint32_t timeoutMs)
{
	clearRx();
	_io->write((uint8_t)0x7F);
	_io->flush();
	return waitAckSimple(timeoutMs);
}

bool STM32RomBootloader::sendCmdByte(uint8_t cmd, uint8_t& resp)
{
	uint8_t buf[2] = { cmd, (uint8_t)(cmd ^ 0xFF) };
	_io->write(buf, 2);
	_io->flush();
	return waitAck(1000, resp);
}

bool STM32RomBootloader::sendAddress(uint32_t addr)
{
	uint8_t a[4];
	a[0] = (addr >> 24) & 0xFF;
	a[1] = (addr >> 16) & 0xFF;
	a[2] = (addr >>  8) & 0xFF;
	a[3] = (addr >>  0) & 0xFF;
	uint8_t c = a[0] ^ a[1] ^ a[2] ^ a[3];
	_io->write(a, 4);
	_io->write(c);
	_io->flush();
	return waitAckSimple(1000);
}

bool STM32RomBootloader::getId(uint16_t& devId, String& err)
{
	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_GET_ID, resp))
	{
		err = (resp == STM32_NACK) ? "GET_ID: NACK" : "GET_ID: timeout/no ACK";
		return false;
	}

	uint8_t n;
	if (!readByteTimeout(n, 1000)) { err = "GET_ID: timeout reading N"; return false; }

	uint8_t high, low;
	if (!readByteTimeout(high, 1000) || !readByteTimeout(low, 1000))
	{
		err = "GET_ID: timeout reading ID bytes";
		return false;
	}

	uint8_t last;
	if (!readByteTimeout(last, 1000) || last != STM32_ACK)
	{
		err = "GET_ID: missing final ACK";
		return false;
	}

	devId = (uint16_t(high) << 8) | uint16_t(low);
	return true;
}

bool STM32RomBootloader::getVersion(uint8_t& ver, uint8_t& opt1, uint8_t& opt2, String& err)
{
	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_GET_VER, resp))
	{
		err = "GET_VER: NACK/timeout";
		return false;
	}

	uint8_t last;
	if (!readByteTimeout(ver, 1000))  { err = "GET_VER: timeout ver"; return false; }
	if (!readByteTimeout(opt1, 1000)) { err = "GET_VER: timeout opt1"; return false; }
	if (!readByteTimeout(opt2, 1000)) { err = "GET_VER: timeout opt2"; return false; }
	if (!readByteTimeout(last, 1000) || last != STM32_ACK)
	{
		err = "GET_VER: missing final ACK";
		return false;
	}

	return true;
}

bool STM32RomBootloader::getSupportedCommandsText(String& out, String& err)
{
	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_GET, resp))
	{
		err = (resp == STM32_NACK) ? "GET: NACK" : "GET: timeout/no ACK";
		return false;
	}

	uint8_t n;
	if (!readByteTimeout(n, 1000)) { err = "GET: timeout reading N"; return false; }

	uint8_t blVer;
	if (!readByteTimeout(blVer, 1000)) { err = "GET: timeout reading version"; return false; }

	out = "Protocol version = 0x";
	out += String(blVer, HEX);
	out += ", CMDs: ";

	for (uint16_t i = 0; i < n; i++)
	{
		uint8_t cmd;
		if (!readByteTimeout(cmd, 1000)) { err = "GET: timeout reading command byte"; return false; }
		out += "0x";
		out += String(cmd, HEX);
		if (i < n) out += ", ";
	}

	uint8_t last;
	if (!readByteTimeout(last, 1000) || last != STM32_ACK)
	{
		err = "GET: missing final ACK";
		return false;
	}

	return true;
}

bool STM32RomBootloader::getSupportedCommands(uint8_t* out, size_t& outCount, uint8_t& proto, String& err)
{
	outCount = 0;
	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_GET, resp))
	{
		err = "GET: NACK/timeout";
		return false;
	}

	uint8_t n;
	if (!readByteTimeout(n, 1000)) { err = "GET: timeout N"; return false; }
	if (!readByteTimeout(proto, 1000)) { err = "GET: timeout proto"; return false; }

	size_t want = (size_t)n;
	String DBG;
	DBG += "Protocol version = 0x";
	DBG += String(proto, HEX);
	DBG += ", CMDs: ";
	
	for (size_t i = 0; i < want; i++)
	{
		uint8_t c;
		if (!readByteTimeout(c, 1000)) { err = "GET: timeout cmd list"; return false; }
		out[outCount++] = c;
		DBG += "0x";
		DBG += String(c, HEX);
		if (i < (n - 1)) DBG += ", ";
	}

	uint8_t last;
	if (!readByteTimeout(last, 1000) || last != STM32_ACK)
	{
		err = "GET: missing final ACK";
		return false;
	}
    err = DBG;
	return true;
}

bool STM32RomBootloader::readMemory(uint32_t addr, uint8_t* buf, size_t len, String& err)
{
	if (len == 0 || len > 256) { err = "READ: len must be 1..256"; return false; }

	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_READ, resp))
	{
		err = (resp == STM32_NACK) ? "READ: NACK cmd" : "READ: timeout/no ACK cmd";
		return false;
	}

	if (!sendAddress(addr))
	{
		err = "READ: NACK/timeout address";
		return false;
	}

	uint8_t N = (uint8_t)(len - 1);
	uint8_t cs = (uint8_t)(N ^ 0xFF);
	_io->write(N);
	_io->write(cs);
	_io->flush();

	if (!waitAckSimple(1000))
	{
		err = "READ: NACK/timeout length";
		return false;
	}

	for (size_t i = 0; i < len; i++)
	{
		if (!readByteTimeout(buf[i], 1000))
		{
			err = "READ: timeout data";
			return false;
		}
	}

	return true;
}

bool STM32RomBootloader::writeChunk(uint32_t addr, const uint8_t* data, size_t len, String& err)
{
	if (len == 0) return true;
	if (len > STM32_CHUNK) { err = "WRITE: len > chunk"; return false; }

	size_t padded = (len + 3) & ~((size_t)3);
	uint8_t tmp[STM32_CHUNK];
	for (size_t i = 0; i < padded; i++) tmp[i] = (i < len) ? data[i] : 0xFF;

	uint8_t resp;
	if (!sendCmdByte(STM32_CMD_WRITE, resp))
	{
		err = (resp == STM32_NACK) ? "WRITE: NACK cmd" : "WRITE: timeout/no ACK cmd";
		return false;
	}

	if (!sendAddress(addr))
	{
		err = "WRITE: NACK/timeout address";
		return false;
	}

	uint8_t N = (uint8_t)(padded - 1);
	uint8_t c = N;
	_io->write(N);
	for (size_t i = 0; i < padded; i++)
	{
		_io->write(tmp[i]);
		c ^= tmp[i];
	}
	_io->write(c);
	_io->flush();

	if (!waitAck(10000, resp))
	{
		err = "WRITE: timeout/no ACK data";
		return false;
	}
	if (resp == STM32_NACK)
	{
		err = "WRITE: NACK data";
		return false;
	}

	return true;
}

bool STM32RomBootloader::writeMemory(uint32_t addr, const uint8_t* data, size_t len, String& err, size_t chunk)
{
	size_t offset = 0;
	while (offset < len)
	{
		size_t n = len - offset;
		if (n > chunk) n = chunk;

		if (!writeChunk(addr, data + offset, n, err)) return false;

		size_t adv = (n + 3) & ~((size_t)3);
		addr += (uint32_t)adv;
		offset += n;
		yield();
	}
	return true;
}

bool STM32RomBootloader::massErase(uint8_t eraseCmd, uint32_t eraseTimeoutMs, String& err)
{
	uint8_t resp;
	if (!sendCmdByte(eraseCmd, resp))
	{
		err = (resp == STM32_NACK) ? "ERASE: NACK cmd" : "ERASE: timeout/no ACK cmd";
		return false;
	}

	if (eraseCmd == STM32_CMD_ERASE)
	{
		uint8_t frame[2] = {0xFF, 0x00};
		_io->write(frame, 2);
		_io->flush();

		if (!waitAck(eraseTimeoutMs, resp))
		{
			err = "ERASE: timeout/no ACK frame";
			return false;
		}
		if (resp == STM32_NACK)
		{
			err = "ERASE: NACK frame";
			return false;
		}
		return true;
	}

	if (eraseCmd == STM32_CMD_XERASE)
	{
		uint8_t frame[3] = {0xFF, 0xFF, 0x00};
		_io->write(frame, 3);
		_io->flush();

		if (!waitAck(eraseTimeoutMs, resp))
		{
			err = "XERASE: timeout/no ACK frame";
			return false;
		}
		if (resp == STM32_NACK)
		{
			err = "XERASE: NACK frame";
			return false;
		}
		return true;
	}

	err = "ERASE: unsupported eraseCmd";
	return false;
}
