#include "Device.h"
#include "BUS.h"



namespace gb {
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
