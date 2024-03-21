#pragma once




#define CART_BEGIN 0x0000
#define CART_END 0x7fff
#define CART_SIZE 0x8000

#define WORK_RAM_BEGIN 0xC000
#define WORK_RAM_END 0xDFFF
#define WORK_RAM_SIZE WORK_RAM_END - WORK_RAM_BEGIN + 1

#define HIGH_RAM_BEGIN 0xff80
#define HIGH_RAM_END 0xfffe
#define HIGH_RAM_SIZE HIGH_RAM_END - HIGH_RAM_BEGIN + 1

#define VRAM_BEGIN 0x8000
#define VRAM_END 0x9fff
#define VRAM_SIZE VRAM_END - VRAM_BEGIN + 1

#define OAM_BEGIN 0xFE00
#define OAM_END 0xFE9F
#define OAM_SIZE OAM_END - OAM_BEGIN + 1





enum DeviceType {
	//device integer aliases
	CPU_TYPE,
	CART_TYPE,
	RAM_TYPE,
	PPU_TYPE,
	APU_TYPE,
	REGISTERS_TYPE,
	END_TYPE
};