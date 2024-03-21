#pragma once
#include "DeviceHandlers/BUS.h"
#include "DeviceHandlers/Device.h"





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