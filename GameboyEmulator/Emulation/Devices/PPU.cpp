#include "PPU.h"


#include <SFML/Graphics.hpp>

namespace gb {



	uint8_t PPU::ReadLY() {
		return _LY;
	}

	void PPU::Init()
	{
		_SCY = 0;
		_SCX = 0;
		_LY = 0;
		_LYC = 0;

		Type = PPU_TYPE;
		StateSize = sizeof(PPU) - sizeof(Device);
		deviceState = (uint8_t*)this + sizeof(Device);

		Bus->SetMemoryMap(PPU_TYPE, VRAM_BEGIN, VRAM_SIZE);
		Bus->SetMemoryMap(PPU_TYPE, OAM_BEGIN, OAM_SIZE);

		Bus->SetMemoryMap(PPU_TYPE, LCDC, 18);
	}

	uint8_t PPU::Read(uint16_t address)
	{

		if (address >= VRAM_BEGIN && address <= VRAM_END) {
			return VideoRam[address - VRAM_BEGIN];
		}


		if (address >= OAM_BEGIN && address <= OAM_END) {
			return OAM[address - OAM_BEGIN];
		}

		switch (address) {
		case HardwareRegister::LY:
			return _LY;
		}
		

		return 0;
	
	}

	void PPU::Write(uint16_t Address, uint8_t Byte)
	{
		if (Address >= VRAM_BEGIN && Address <= VRAM_END) {
			VideoRam[Address - VRAM_BEGIN] = Byte;
			return;
		}
		if (Address >= OAM_BEGIN && Address <= OAM_END) {
			OAM[Address - OAM_BEGIN] = Byte;
			return;
		}
		switch (Address) {
		case LCDC:
			_LCDC = Byte;
			break;
		case STAT:
			_LCD_STAT = Byte;

		}
	}
	void PPU::Clock()
	{
		//Mode 0

		if (_LYC >= 80 && _LYC < 80 + 160 && _LY < 144) {


			uint8_t tileIndex = Read(0x9800 + 32 * (_LY / 8) + ((_LYC - 80) / 8));


			int Index = tileIndex * 16 + ((_LY % 8) * 2);

			uint8_t high = Read(0x8000 + Index + 1);
			uint8_t low = Read(0x8000 + Index);


			int yindex = 7 - ((_LYC - 80) % 8);

			uint8_t Color = ((high >> yindex & 1) << 1) + ((low >> yindex) & 1);


			int rnd = Color;

			sf::Color Color1;
			sf::Color Color2;


			if (rnd == 0) {
				Color1 = sf::Color(0x00, 8, 24, 32);
			}
			else if (rnd == 1) {
				Color1 = sf::Color(0xff, 52, 104, 86);
			}
			else if (rnd == 2) {
				Color1 = sf::Color(0xff, 136, 192, 112);
			}
			else {
				
				Color1 = sf::Color(0xff, 224, 248, 208);
			}


		
			Bus->CurrentIOstate.WritePixel(Color1.toInteger(), _LYC - 80, _LY);
		}


		_LYC++;
		if (_LYC > 456) {
			_LYC = 0;
			_LY++;
			if (_LY == 144) {

				Registers* reg = (Registers*)Bus->_Devices[REGISTERS_TYPE];
				reg->interupt_register.IF.VBLANK = true;
				Bus->CurrentIOstate.SwapBuffer();
			}
			
			if (_LY > 153) {
				_LY = 0;
			}
		}
	}


}
