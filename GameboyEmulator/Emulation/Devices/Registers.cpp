#include "Registers.h"
namespace gb {
	void Registers::Init()
	{
		Type = REGISTERS_TYPE;
		StateSize = sizeof(Registers) - sizeof(Device);
		deviceState = (uint8_t*)this + sizeof(Device);

		Bus->SetMemoryMap(REGISTERS_TYPE, RegisterInfo::P1_JOYP, RegisterInfo::IF - RegisterInfo::P1_JOYP + 1);
		Bus->SetMemoryMap(REGISTERS_TYPE, RegisterInfo::IE, 1);
		Write(RegisterInfo::TAC, 0);
	}
	uint8_t Registers::Read(uint16_t Address)
	{
		switch (Address) {

		case RegisterInfo::P1_JOYP:

			
			return joypad_register.Register;

		case RegisterInfo::SB:
			break;

		case RegisterInfo::SC:
			break;
		case RegisterInfo::DIV:
			return timer_register.DIV;
		case RegisterInfo::TIMA:
			return timer_register.TIMA;
		case RegisterInfo::TMA:
			return timer_register.TMA;
		case RegisterInfo::TAC:
			return timer_register.TAC;
		case RegisterInfo::IF:
			return Bus->interupt_register.IF.DATA;
		case RegisterInfo::IE:
			return Bus->interupt_register.IE.DATA;
		}
		return 0;
	}
	void Registers::Write(uint16_t Address, uint8_t Data)
	{
		//this is more of a guide most of these will be implemented in the devices them selves
		switch (Address) {

		case RegisterInfo::P1_JOYP:

			joypad_register.Register = Data;
			joypad_register.Updated = true;
			break;
		case RegisterInfo::SB:
			break;

		case RegisterInfo::SC:
			break;
		case RegisterInfo::DIV:
			timer_register.DIV = 0x00;
			break;
		case RegisterInfo::TIMA:
			timer_register.TIMA = Data;
			break;
		case RegisterInfo::TMA:
			timer_register.TMA = Data;
			break;
		case RegisterInfo::TAC:
			timer_register.TAC = Data;

			switch (timer_register.ClockSelect) {
			case 0:

				timer_register.CLOCK_SPEED = DOT_PER_SECOND / 4096;
				break;
			case 1:

				timer_register.CLOCK_SPEED = DOT_PER_SECOND / 262144;
				break;
			case 2:
				timer_register.CLOCK_SPEED = DOT_PER_SECOND / 65536;
				break;

			case 3:
				timer_register.CLOCK_SPEED = DOT_PER_SECOND / 16384;
				break;
			}

			break;
		case RegisterInfo::IF:
			Bus->interupt_register.IF.DATA = Data;
			break;
		case RegisterInfo::IE:
			Bus->interupt_register.IE.DATA = Data;
			break;
		}
	}
	void Registers::ClockJoypad()
	{
		//this checks if any bits changed from high to low
		uint8_t BeforeUpdate = joypad_register.GetInputButtons();
		if (!joypad_register.SelectButtons) {
			joypad_register.A_Right = !Bus->CurrentIOstate.ControllerInput.A;
			joypad_register.B_Left = !Bus->CurrentIOstate.ControllerInput.B;
			joypad_register.Select_Up = !Bus->CurrentIOstate.ControllerInput.Select;
			joypad_register.Start_Down = !Bus->CurrentIOstate.ControllerInput.Start;
		}
		if (!joypad_register.Selectdpad) {
			joypad_register.A_Right = !Bus->CurrentIOstate.ControllerInput.Right;
			joypad_register.B_Left = !Bus->CurrentIOstate.ControllerInput.Left;
			joypad_register.Select_Up = !Bus->CurrentIOstate.ControllerInput.Up;
			joypad_register.Start_Down = !Bus->CurrentIOstate.ControllerInput.Down;
		}
		if (!joypad_register.Updated && BeforeUpdate != joypad_register.GetInputButtons()) {
			Bus->interupt_register.IF.JOYPAD = true;
		}

		joypad_register.Updated = false;
	}
	void Registers::ClockTimer()
	{

		if (Bus->TimeCode % DIV_SPEED == 0) {
			timer_register.DIV++;
		}
		if (Bus->TimeCode % timer_register.CLOCK_SPEED == 0 && timer_register.Enable) {

			if (timer_register.TIMA == 0xff) {

				Bus->interupt_register.IF.TIMER = true;
				timer_register.TIMA = timer_register.TMA;
			}
			else {
				timer_register.TIMA++;
			}

		}

	}
	void Registers::Clock()
	{
		
		ClockJoypad();
		ClockTimer();
	}
}