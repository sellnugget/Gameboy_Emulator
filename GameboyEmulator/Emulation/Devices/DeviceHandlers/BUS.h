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




		//total number of dots since execution began
		uint64_t TimeCode = 0;
		BUS();
		Device* _Devices[END_TYPE];
		IOState CurrentIOstate;

		
		void SetDevice(DeviceType type, Device* device);
		void SetMemoryMap(DeviceType type, uint16_t Address, size_t RegionSize);

		uint8_t ReadByte(uint16_t Address);
		uint16_t ReadWord(uint16_t Address);
		void WriteByte(uint16_t Address, uint8_t data);
		void WriteWord(uint16_t Address, uint16_t data);


		


	private:
		DeviceType _MemoryMap[0xffff];
	};


}

