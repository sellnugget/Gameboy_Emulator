#include "ClockUnit.h"


namespace gb {
	
	void ClockUnit::Init()
	{

		Type = CLOCK_TYPE;
		StateSize = sizeof(ClockUnit) - sizeof(Device);
		deviceState = (uint8_t*)this + sizeof(Device);
		Bus->SetMemoryMap(CLOCK_TYPE, 0xff04, 4);

		Write(TAC, 0);

	}
	uint8_t ClockUnit::Read(uint16_t Address)
	{
		switch (Address) {

		case DIV:
			return _DIV;
		case TIMA:
			return _TIMA;
		case TMA:
			return _TMA;
		case TAC:
			return TAC;
		}
	}
	void ClockUnit::Write(uint16_t Address, uint8_t Byte)
	{
		switch (Address) {

		case DIV:
			_DIV = 0x00;
			break;

		case TIMA:
			_TIMA = Byte;
			break;

		case TMA:
			_TMA = Byte;
			break;
		case TAC:
			_TAC = Byte;

			switch (ClockSelect) {
			case 0:

				_CLOCK_SPEED = DOT_PER_SECOND / 4096;
				break;
			case 1:

				_CLOCK_SPEED = DOT_PER_SECOND / 262144;
				break;
			case 2:
				_CLOCK_SPEED = DOT_PER_SECOND / 65536;
				break;

			case 3:
				_CLOCK_SPEED = DOT_PER_SECOND / 16384;
				break;
			}

			break;
		}
	}
	void ClockUnit::Clock()
	{
		if (Bus->TimeCode % DIV_SPEED == 0) {
			_DIV++;
		}
		if (Bus->TimeCode % _CLOCK_SPEED == 0 && Enable) {

			if (_TIMA == 0xff) {

				Registers* registers = (Registers*)Bus->_Devices[REGISTERS_TYPE];
				registers->interupt_register.IF.TIMER = true;
				_TIMA = _TMA;
			}
			else {
				_TIMA++;
			}
			
		}
	}
}