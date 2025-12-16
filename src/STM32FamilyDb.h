/********************************************************************************************************
 *  [FILE NAME]   :      <STM32FamilyDb.h>                                                              *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for STM32 families database>                                      *
 ********************************************************************************************************/

#ifndef STM32_FAMILIES_H
#define	STM32_FAMILIES_H

#include <Arduino.h>
#include "STM32DeviceConstants.h"

enum STM32Family
{
	STM32_C0,
	STM32_F0,
	STM32_F1,
	STM32_F2,
	STM32_F3,
	STM32_F4,
	STM32_F7,
	STM32_H5,
	STM32_H7,
	STM32_L0,
	STM32_L1,
	STM32_L4,
	STM32_L5,
	STM32_G0,
	STM32_G4,
	STM32_WB,
	STM32_UNKNOWN
};

struct STM32FamilyInfo
{
	STM32Family family;
	uint16_t DevID;
	uint16_t FlashSize;
	uint32_t flashSizeAddr;
	uint32_t eraseTimeout;
	uint8_t  eraseCmd;
	bool     supportsGlobalErase;
	uint32_t flashStart;
	uint32_t sramTestAddr;
	const char* name;
};

class STM32FamilyDb
{
	public:
	static STM32FamilyInfo getFamilyInfo(uint16_t devId);
};

#endif	/* STM32_FAMILIES_H */