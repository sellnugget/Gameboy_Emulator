#include "Device.h"
#include "BUS.h"



namespace gb {
	uint8_t Device::ReadBus(uint16_t Address)
	{
		return Bus->ReadByte(Address, Type);
	}
	void Device::WriteBus(uint16_t Address, uint8_t Byte)
	{
		Bus->WriteByte(Address, Byte, Type);
	}
	uint16_t Device::ReadWord(uint16_t Address)
	{
		return Bus->ReadWord(Address, Type);
	}
	void Device::WriteWord(uint16_t Address, uint16_t Word)
	{
		Bus->WriteWord(Address, Word, Type);
	}
	void* Device::SaveState()
	{
		void* Copy = malloc(StateSize);
		std::memcpy(Copy, deviceState, StateSize);
		return Copy;
	}

	void Device::LoadState(void* state)
	{
		std::memcpy(deviceState, state, StateSize);
	}

	void Device::BaseInit(BUS* bus)
	{
		Bus = bus;
		Init();
		Bus->SetDevice(Type, this);
	}

}
