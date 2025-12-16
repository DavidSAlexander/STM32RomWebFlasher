/********************************************************************************************************
 *  [FILE NAME]   :      <STM32FamilyDb.cpp>                                                            *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Source file for STM32 families database>                                      *
 ********************************************************************************************************/

#include "STM32FamilyDb.h"

/* Note: not all families are tested */
STM32FamilyInfo STM32FamilyDb::getFamilyInfo(uint16_t devId)
{
	switch (devId & 0xFFF)
	{
		case 0x443:
		return (STM32FamilyInfo){STM32_C0, devId, 32,  C0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20001000, "STM32C011xx"};
		case 0x453:
		return (STM32FamilyInfo){STM32_C0, devId, 32,  C0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20001000, "STM32C031xx"};
		case 0x44C:
		return (STM32FamilyInfo){STM32_C0, devId, 64,  C0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20001000, "STM32C051xx"};
		case 0x493:
		return (STM32FamilyInfo){STM32_C0, devId, 128, C0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20001000, "STM32C071xx"};
		case 0x44D:
		return (STM32FamilyInfo){STM32_C0, devId, 256, C0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20001000, "STM32C091xx/92xx"};

		case 0x440:
		return (STM32FamilyInfo){STM32_F0, devId, 64,  F0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20000800, "STM32F030x8/F05xxx"};
		case 0x444:
		return (STM32FamilyInfo){STM32_F0, devId, 32,  F0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20000800, "STM32F03xx4/6"};
		case 0x442:
		return (STM32FamilyInfo){STM32_F0, devId, 256, F0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20000800, "STM32F030xC/F09xxx"};
		case 0x445:
		return (STM32FamilyInfo){STM32_F0, devId, 32,  F0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20000800, "STM32F04xxx/F070x6"};
		case 0x448:
		return (STM32FamilyInfo){STM32_F0, devId, 128, F0_FLASH_SIZE_ADDR, 2000, 0x43, true, 0x08000000, 0x20000800, "STM32F070xB/F071xx/F72xx"};

		case 0x412:
		return (STM32FamilyInfo){STM32_F1, devId, 32,   F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx Low-density"};
		case 0x410:
		return (STM32FamilyInfo){STM32_F1, devId, 128,  F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx Medium-density"};
		case 0x414:
		return (STM32FamilyInfo){STM32_F1, devId, 512,  F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx High-density"};
		case 0x420:
		return (STM32FamilyInfo){STM32_F1, devId, 128,  F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx Medium-density VL"};
		case 0x428:
		return (STM32FamilyInfo){STM32_F1, devId, 512,  F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx High-density VL"};
		case 0x418:
		return (STM32FamilyInfo){STM32_F1, devId, 256,  F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F105xx/F107xx"};
		case 0x430:
		return (STM32FamilyInfo){STM32_F1, devId, 1024, F1_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F10xxx XL-density"};

		case 0x411:
		return (STM32FamilyInfo){STM32_F2, devId, 1024, F2_FLASH_SIZE_ADDR, 12000, 0x43, true, 0x08000000, 0x20010000, "STM32F2xxxx"};

		case 0x432:
		return (STM32FamilyInfo){STM32_F3, devId, 256, F3_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F373xx/F378xx"};
		case 0x422:
		return (STM32FamilyInfo){STM32_F3, devId, 256, F3_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F302xB(C)/F303xB(C)/F358xx"};
		case 0x439:
		return (STM32FamilyInfo){STM32_F3, devId, 64,  F3_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F301xx/F302x4(6/8)/F318xx"};
		case 0x438:
		return (STM32FamilyInfo){STM32_F3, devId, 64,  F3_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F303x4(6/8)/F334xx/F328xx"};
		case 0x446:
		return (STM32FamilyInfo){STM32_F3, devId, 512, F3_FLASH_SIZE_ADDR, 5000, 0x43, true, 0x08000000, 0x20001000, "STM32F302xD(E)/F303xD(E)/F398xx"};

		case 0x413:
		return (STM32FamilyInfo){STM32_F4, devId, 1024, F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F40xxx/41xxx"};
		case 0x419:
		return (STM32FamilyInfo){STM32_F4, devId, 2048, F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F42xxx/43xxx"};
		case 0x423:
		return (STM32FamilyInfo){STM32_F4, devId, 256,  F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F401xB(C)"};
		case 0x433:
		return (STM32FamilyInfo){STM32_F4, devId, 512,  F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F401xD(E)"};
		case 0x431:
		return (STM32FamilyInfo){STM32_F4, devId, 512,  F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F411xx"};
		case 0x434:
		return (STM32FamilyInfo){STM32_F4, devId, 2048, F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F469xx/479xx"};
		case 0x458:
		return (STM32FamilyInfo){STM32_F4, devId, 128,  F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F410xx"};
		case 0x441:
		return (STM32FamilyInfo){STM32_F4, devId, 1024, F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F412xx"};
		case 0x463:
		return (STM32FamilyInfo){STM32_F4, devId, 1536, F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F413xx/423xx"};
		case 0x421:
		return (STM32FamilyInfo){STM32_F4, devId, 512,  F4_FLASH_SIZE_ADDR, 35000, 0x44, false, 0x08000000, 0x20008000, "STM32F446xx"};

		case 0x452:
		return (STM32FamilyInfo){STM32_F7, devId, 512,  F7_FLASH_SIZE_ADDR, 20000, 0x43, true, 0x08000000, 0x20020000, "STM32F72xxx/73xxx"};
		case 0x449:
		return (STM32FamilyInfo){STM32_F7, devId, 1024, F7_FLASH_SIZE_ADDR, 20000, 0x43, true, 0x08000000, 0x20020000, "STM32F74xxx/75xxx"};
		case 0x451:
		return (STM32FamilyInfo){STM32_F7, devId, 2048, F7_FLASH_SIZE_ADDR, 20000, 0x43, true, 0x08000000, 0x20020000, "STM32F76xxx/77xxx"};

		case 0x474:
		return (STM32FamilyInfo){STM32_H5, devId, 512,  H5_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20004000, "STM32H503xx"};
		case 0x484:
		return (STM32FamilyInfo){STM32_H5, devId, 1024, H5_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20004000, "STM32H5A3xx/H56xxx/H57xxx"};
		case 0x478:
		return (STM32FamilyInfo){STM32_H5, devId, 256,  H5_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20004000, "STM32H563xx"};

		case 0x485:
		return (STM32FamilyInfo){STM32_H7, devId, 2048, H7_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20008000, "STM32H7Rxxx/7Sxxx"};
		case 0x480:
		return (STM32FamilyInfo){STM32_H7, devId, 2048, H7_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20008000, "STM32H7A3xx/7B3xx/7B0xx"};
		case 0x483:
		return (STM32FamilyInfo){STM32_H7, devId, 2048, H7_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20008000, "STM32H72xxx/73xxx"};
		case 0x450:
		return (STM32FamilyInfo){STM32_H7, devId, 2048, H7_FLASH_SIZE_ADDR, 40000, 0x44, false, 0x08000000, 0x20008000, "STM32H74xxx/75xxx"};

		case 0x457:
		return (STM32FamilyInfo){STM32_L0, devId, 16,  L0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20000400, "STM32L01xxx/02xxx"};
		case 0x425:
		return (STM32FamilyInfo){STM32_L0, devId, 32,  L0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20000400, "STM32L031xx/041xx"};
		case 0x417:
		return (STM32FamilyInfo){STM32_L0, devId, 64,  L0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20000400, "STM32L05xxx/06xxx"};
		case 0x447:
		return (STM32FamilyInfo){STM32_L0, devId, 192, L0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20000400, "STM32L07xxx/08xxx"};

		case 0x416:
		return (STM32FamilyInfo){STM32_L1, devId, 128, L1_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20001000, "STM32L1xxx6(8/B)"};
		case 0x429:
		return (STM32FamilyInfo){STM32_L1, devId, 128, L1_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20001000, "STM32L1xxx6(8/B)A"};
		case 0x427:
		return (STM32FamilyInfo){STM32_L1, devId, 256, L1_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20001000, "STM32L1xxxC"};
		case 0x436:
		return (STM32FamilyInfo){STM32_L1, devId, 384, L1_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20001000, "STM32L1xxxD"};
		case 0x437:
		return (STM32FamilyInfo){STM32_L1, devId, 512, L1_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20001000, "STM32L1xxxE"};

		case 0x464:
		return (STM32FamilyInfo){STM32_L4, devId, 2048, L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L4Rxxx/4Sxxx"};
		case 0x435:
		return (STM32FamilyInfo){STM32_L4, devId, 256,  L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L43xxx/44xxx"};
		case 0x462:
		return (STM32FamilyInfo){STM32_L4, devId, 512,  L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L45xxx/46xxx"};
		case 0x415:
		return (STM32FamilyInfo){STM32_L4, devId, 1024, L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L47xxx/48xxx"};
		case 0x461:
		return (STM32FamilyInfo){STM32_L4, devId, 1024, L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L496xx/4A6xx"};
		case 0x470:
		return (STM32FamilyInfo){STM32_L4, devId, 512,  L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L41xxx/42xxx"};
		case 0x471:
		return (STM32FamilyInfo){STM32_L4, devId, 1024, L4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L4P5xx/4Q5xx"};

		case 0x472:
		return (STM32FamilyInfo){STM32_L5, devId, 512, L5_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32L55xxx/56xxx"};

		case 0x466:
		return (STM32FamilyInfo){STM32_G0, devId, 64,  G0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20001000, "STM32G0C1xx"};
		case 0x456:
		return (STM32FamilyInfo){STM32_G0, devId, 32,  G0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20001000, "STM32G0B0xx/B1xx"};
		case 0x460:
		return (STM32FamilyInfo){STM32_G0, devId, 128, G0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20001000, "STM32G07xxx/08xxx"};
		case 0x467:
		return (STM32FamilyInfo){STM32_G0, devId, 64,  G0_FLASH_SIZE_ADDR, 3000, 0x43, true, 0x08000000, 0x20001000, "STM32G031xx/041xx"};

		case 0x468:
		return (STM32FamilyInfo){STM32_G4, devId, 512, G4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20004000, "STM32G43xxx/44xxx"};
		case 0x469:
		return (STM32FamilyInfo){STM32_G4, devId, 128, G4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20004000, "STM32G47xxx/48xxx"};
		case 0x479:
		return (STM32FamilyInfo){STM32_G4, devId, 512, G4_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20004000, "STM32G49xxx/4Axxx"};

		case 0x494:
		return (STM32FamilyInfo){STM32_WB, devId, 1024, WB_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32WB55xx/35xx"};
		case 0x495:
		return (STM32FamilyInfo){STM32_WB, devId, 512,  WB_FLASH_SIZE_ADDR, 8000, 0x43, true, 0x08000000, 0x20008000, "STM32WB10xx/15xx/50xx"};

		default:
		return (STM32FamilyInfo){STM32_UNKNOWN, devId, 64, 0x1FFFF7CC, 40000, 0x43, true, 0x08000000, 0x20000200, "STM32 Unknown"};
	}
}
