#pragma once
#include <iostream>
#include <vector>
#include<algorithm>
#include <functional>
#include "Device.h"
#include "DeviceList.h"
#include "IOState.h"
namespace gb {

	class BUS
	{


	public:
		struct InteruptRegister {


			struct REG {
				union {
					struct {
						unsigned char VBLANK : 1;
						unsigned char LCD : 1;
						unsigned char TIMER : 1;
						unsigned char SERIAL : 1;
						unsigned char JOYPAD : 1;
						unsigned char UNALLOCATED : 3;
					};
					unsigned char DATA;
				};
			};
			REG IE;
			REG IF;
			bool IME;
		};

		//interupt register used for handling interupts
		InteruptRegister interupt_register;
		//total number of dots since execution began
		uint64_t TimeCode = 0;
		BUS();
		Device* _Devices[END_TYPE];
		IOState CurrentIOstate;

		
		void SetDevice(DeviceType type, Device* device);
		void SetMemoryMap(DeviceType type, uint16_t Address, size_t RegionSize);


		uint8_t ReadByte(uint16_t Address, DeviceType device);
		uint16_t ReadWord(uint16_t Address, DeviceType device);
		void WriteByte(uint16_t Address, uint8_t data, DeviceType device);
		void WriteWord(uint16_t Address, uint16_t data, DeviceType device);


		


	private:


		DeviceType _MemoryMap[0x10000];
	};


}

