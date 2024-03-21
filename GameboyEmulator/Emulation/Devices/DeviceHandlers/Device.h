#pragma once
#include <inttypes.h>
#include "DeviceList.h"


#define DOT_PER_SECOND 4194304

namespace gb {

	class BUS;


	class Device
	{
	public:


	
		DeviceType Type;
		size_t StateSize;
		BUS* Bus;
		void* deviceState;

		//sets Type and State Size and sets d_deviceState to data among other initialization stuff
		virtual void Init() {};
		
		virtual uint8_t Read(uint16_t Address) { return 0; };
		virtual void Write(uint16_t Adddress, uint8_t Data) {};

		uint8_t ReadBus(uint16_t Address);

		void WriteBus(uint16_t Address, uint8_t Byte);

		uint16_t ReadWord(uint16_t Address);

		void WriteWord(uint16_t Address, uint16_t Word);

		void* SaveState();
		void LoadState(void* state);
		void BaseInit(BUS* bus);

	};


}

