#pragma once
#include <vector>
#include "Devices/DeviceHandlers/DeviceList.h"


class SaveState
{
public:


	//this is the amount of dots since start of execution
	uint64_t TimeCode;
	//holds state for each device and accessed through alias
	void* Devices[END_TYPE];
	uint8_t InteruptRegister[0xff];
};

