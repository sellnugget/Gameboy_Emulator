#include "Ram.h"

namespace gb {
	void Ram::Init()
	{
		Type = RAM_TYPE;

		StateSize = sizeof(Ram) - sizeof(Device);

		deviceState = (uint8_t*)this + sizeof(Device);
		Bus->SetMemoryMap(RAM_TYPE, WORK_RAM_BEGIN, WORK_RAM_SIZE);
		Bus->SetMemoryMap(RAM_TYPE, HIGH_RAM_BEGIN, HIGH_RAM_SIZE);
	}

	uint8_t Ram::Read(uint16_t Address)
	{
		if (Address >= WORK_RAM_BEGIN && Address <= WORK_RAM_END) {
			return WRAM[Address - WORK_RAM_BEGIN];
		}
		if (Address >= HIGH_RAM_BEGIN && Address <= HIGH_RAM_END) {




			return HRAM[Address - HIGH_RAM_BEGIN];
		}
	}

	void Ram::Write(uint16_t Address, uint8_t Byte)
	{
		if (Address >= WORK_RAM_BEGIN && Address <= WORK_RAM_END) {
			WRAM[Address - WORK_RAM_BEGIN] = Byte;
		}
		if (Address >= HIGH_RAM_BEGIN && Address <= HIGH_RAM_END) {


			HRAM[Address - HIGH_RAM_BEGIN] = Byte;
		}
	}

}

