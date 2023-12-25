#pragma once
#include "DeviceHandlers/BUS.h"
#include "DeviceHandlers/Device.h"


#define WORK_RAM_BEGIN 0xC000
#define WORK_RAM_END 0xDFFF
#define WORK_RAM_SIZE WORK_RAM_END - WORK_RAM_BEGIN + 1

#define HIGH_RAM_BEGIN 0xff80
#define HIGH_RAM_END 0xfffe
#define HIGH_RAM_SIZE HIGH_RAM_END - HIGH_RAM_BEGIN + 1


namespace gb {



	class Ram : public Device {


	public:

		Ram(BUS* bus) { BaseInit(bus); }

		uint8_t WRAM[WORK_RAM_SIZE];
		uint8_t HRAM[HIGH_RAM_SIZE];
		void Init() override;

		uint8_t Read(uint16_t Address) override;
		void Write(uint16_t Address, uint8_t Byte) override;



	};

}