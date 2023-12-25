#include "BUS.h"
namespace gb {



	BUS::BUS()
	{
		for (int i = 0; i <= 0xffff; i++) {
			_MemoryMap[i] = END_TYPE;
		}
		for (int i = 0; i < END_TYPE; i++) {
			_Devices[i] = NULL;
		}
	}

	void BUS::SetDevice(DeviceType type, Device* device)
	{
		_Devices[type] = device;
	}

	void BUS::SetMemoryMap(DeviceType type, uint16_t Address, size_t RegionSize)
	{
		for (int i = Address; i < Address + RegionSize; i++) {
			_MemoryMap[i] = type;
		}

	}

	uint8_t BUS::ReadByte(uint16_t Address)
	{
		DeviceType type = _MemoryMap[Address];
		if (type != END_TYPE) {
			return _Devices[type]->Read(Address);
		}
		return 0;
	}

	uint16_t BUS::ReadWord(uint16_t Address)
	{
		uint16_t rtn = ReadByte(Address);
		rtn += ReadByte(Address + 1) << 8;
		return rtn;
	}

	void BUS::WriteByte(uint16_t Address, uint8_t data)
	{

		DeviceType type = _MemoryMap[Address];
		if (type != END_TYPE) {

		
			_Devices[type]->Write(Address, data);
		}

	}

	void BUS::WriteWord(uint16_t Address, uint16_t data)
	{
		WriteByte(Address, data & 0xff);
		WriteByte(Address + 1, (data & 0xff00) >> 8);
	}

	



}

