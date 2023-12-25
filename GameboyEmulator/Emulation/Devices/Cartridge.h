#pragma once
#include <iostream>
#include<fstream>
#include "DeviceHandlers/Device.h"
#include "DeviceHandlers/BUS.h"



#define CART_BEGIN 0x0000
#define CART_END 0x7fff
#define CART_SIZE 0x8000

namespace gb {

	class Cartridge : public Device
	{
	public:

		Cartridge(BUS* bus) { BaseInit(bus); }
		void LoadRom(std::string Path);

		void Init() override;

		uint8_t Read(uint16_t Address) override;

		enum MemoryMapperType {
			ROM = 0x00,
			MBC1 = 0x01,
			MBC1_RAM = 0x02,
			MBC1_RAM_BATTERY = 0x03,
			MBC2 = 0x05,
			MBC2_BATTERY = 0x06,
			ROM_RAM = 0x08,
			ROM_RAM_BATTERY = 0x09,
			MMM01 = 0x0b,
			MMM01_RAM = 0x0c,
			MMM01_RAM_BATTERY = 0x0d,
			MBC3_TIMER_BATTERY = 0x0f,
			MBC3_TIMER_RAM_BATTERY = 0x10,
			MBC3 = 0x11,
			MBC3_RAM = 0x12,
			MBC3_RAM_BATTERY = 0x13,
			MBC4 = 0x15,
			MBC4_RAM = 0x16,
			MBC4_RAM_BATTERY = 0x17,
			MBC5 = 0x19,
			MBC5_RAM = 0x1A,
			MBC5_RAM_BATTERY = 0x1b,
			MBC5_RUMBLE = 0x1c,
			MBC5_RUMBLE_RAM = 0x1d,
			MBC5_RUMBLE_RAM_BATTERY = 0x1e,
			POCKET_CAMERA = 0xfc,
			Bandai_TAMA5 = 0xfd,
			HuC3 = 0xfe,
			HuC1_RAM_BATTERY = 0xff
		};
	private:
		uint8_t* RomData;
	};

}



