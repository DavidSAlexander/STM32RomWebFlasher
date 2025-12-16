/********************************************************************************************************
 *  [FILE NAME]   :      <STM32DeviceConstants.h>                                                       *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Header file for FOTA constants>                                               *
 ********************************************************************************************************/

#ifndef FOTA_CONSTANTS_H
#define	FOTA_CONSTANTS_H

#include <Arduino.h>

static const uint8_t STM32_ACK  = 0x79;
static const uint8_t STM32_NACK = 0x1F;

static const uint8_t STM32_CMD_GET     = 0x00;
static const uint8_t STM32_CMD_GET_VER = 0x01;
static const uint8_t STM32_CMD_GET_ID  = 0x02;
static const uint8_t STM32_CMD_READ    = 0x11;
static const uint8_t STM32_CMD_GO      = 0x21;
static const uint8_t STM32_CMD_WRITE   = 0x31;
static const uint8_t STM32_CMD_ERASE   = 0x43;
static const uint8_t STM32_CMD_XERASE  = 0x44;

static const uint32_t STM32_FLASH_START_DEFAULT = 0x08000000UL;
static const size_t   STM32_CHUNK = 256;

#define F0_FLASH_SIZE_ADDR   0x1FFFF7CCUL
#define G0_FLASH_SIZE_ADDR   0x1FFF75E0UL
#define L0_FLASH_SIZE_ADDR   0x1FF8007CUL
#define C0_FLASH_SIZE_ADDR   0x1FFF75E0UL

#define F1_FLASH_SIZE_ADDR   0x1FFFF7E0UL
#define F2_FLASH_SIZE_ADDR   0x1FFF7A22UL
#define L1_FLASH_SIZE_ADDR   0x1FF8004CUL

#define F3_FLASH_SIZE_ADDR   0x1FFFF7CCUL
#define F4_FLASH_SIZE_ADDR   0x1FFF7A22UL
#define L4_FLASH_SIZE_ADDR   0x1FFF75E0UL
#define G4_FLASH_SIZE_ADDR   0x1FFF75E0UL
#define WB_FLASH_SIZE_ADDR   0x1FFF75E0UL

#define F7_FLASH_SIZE_ADDR   0x1FF0F442UL
#define H7_FLASH_SIZE_ADDR   0x1FF1E880UL
#define H7_FLASH_SIZE_ADDR2  0x1FF1E881UL

#define H5_FLASH_SIZE_ADDR   0x1FFF75E0UL
#define L5_FLASH_SIZE_ADDR   0x0BFA05E0UL
#define L5_FLASH_SIZE_ADDR_S 0x0BFB05E0UL
#define U5_FLASH_SIZE_ADDR   0x0BFA05E0UL

#define WL_FLASH_SIZE_ADDR   0x1FFF75E0UL

#endif	/* FOTA_CONSTANTS_H */