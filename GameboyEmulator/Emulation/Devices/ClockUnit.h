#pragma once

#include "DeviceHandlers/Device.h"
#include "DeviceHandlers/BUS.h"
#include "DeviceDefinitions/RegisterTypes.h"
#include "Registers.h"

#define DIV_SPEED DOT_PER_SECOND / 16384

namespace gb {

	class ClockUnit : public Device
	{



	public:

		ClockUnit(BUS* bus) { BaseInit(bus); };
		void Init() override;

		uint8_t Read(uint16_t Address) override;
	
		void Write(uint16_t Address, uint8_t Byte) override;
		void Clock();


	private:
		//Divider register
		uint8_t _DIV;

		//Timer Counter
		uint8_t _TIMA;

		//Timer modulo
		uint8_t _TMA;


		//this is the current clock speed in dots selected by TAC 
		uint64_t _CLOCK_SPEED;
		//Timer Control
		union {
			struct {

				uint8_t ClockSelect : 2;

				uint8_t Enable : 1;

				uint8_t Empty : 5;

			};
			uint8_t _TAC;
		};
	};
}

