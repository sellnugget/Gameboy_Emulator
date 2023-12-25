#pragma once
#include "DeviceHandlers/Device.h"
#include "DeviceDefinitions/RegisterTypes.h"
#include "DeviceHandlers/BUS.h"


namespace gb {
	//device to handle simple registers 


	class Registers : public Device
	{

	public:

		Registers(BUS* bus) { BaseInit(bus); }

		InteruptRegister interupt_register;
		JoyPad joypad_register;


		void Init() override;
		uint8_t Read(uint16_t Address) override;
		void Write(uint16_t Address, uint8_t Data) override;




	};
}


