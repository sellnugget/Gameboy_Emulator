#pragma once

#include <iostream>
#include <vector>
#include "Registers.h"
#include "DeviceHandlers/Device.h"
#include "DeviceHandlers/BUS.h"
namespace gb {



#define VRAM_BEGIN 0x8000
#define VRAM_END 0x9fff
#define VRAM_SIZE VRAM_END - VRAM_BEGIN + 1

#define OAM_BEGIN 0xFE00
#define OAM_END 0xFE9F
#define OAM_SIZE OAM_END - OAM_BEGIN + 1


	class PPU : public Device
	{
	public:



		PPU(BUS* bus) { BaseInit(bus); }

		uint8_t VideoRam[VRAM_SIZE];
		//current phase of PPU
		uint8_t MODE;

		struct SPRITE_OBJ {


			//y position of sprite + 16
			uint8_t Y_Position;

			//x position of sprite + 8
			uint8_t X_Position;

			//index of tile. if in 8x16 tile mode then 
			//the least significant bit of the tile index is ignored;
			//that is, the top 8×8 tile is “NN & $FE”, and the bottom 
			//8×8 tile is “NN | $01”.
			uint8_t Tile_Index;

			struct Attributes {
				//CGB palette [CGB Mode Only]: Which of OBP0–7 to use
				uint8_t CGB_PALETTE : 3;
				//Bank [CGB Mode Only]: 0 = Fetch tile from VRAM bank 0, 1 = Fetch tile from VRAM bank 1
				uint8_t BANK : 1;
				//DMG palette [Non CGB Mode only]: 0 = OBP0, 1 = OBP1
				uint8_t DMG_PALETTE : 1;

				//flip x?
				uint8_t X_FLIP : 1;
				//flip y?
				uint8_t Y_FLIP : 1;

				//Priority: 0 = No, 1 = BG and Window colors 1–3 are drawn over this OBJ
				uint8_t PRIORITY : 1;
			};
		};


		union {
			uint8_t OAM[OAM_SIZE];

			SPRITE_OBJ Sprites[40];
		};
	

		void Init() override;


		uint8_t Read(uint16_t address) override;
		void Write(uint16_t Address, uint8_t Byte) override; 


		void Clock();



		//register stuff

		uint8_t ReadLY();
	private:


		union {
			struct {

				//BG & Window enable / priority
				uint8_t BG_WIN_ENABLE : 1;

				uint8_t OBJ_ENABLE : 1;

				uint8_t OBJ_SIZE : 1;

				uint8_t BG_TILE_MAP : 1;

				uint8_t BG_WIN_TILES : 1;

				uint8_t WIN_ENABLE : 1;

				uint8_t WIN_TILE_MAP : 1;

				uint8_t LCD_PPU_ENABLE : 1;

			};

			uint8_t _LCDC;
		};

		union {
			struct {
				uint8_t PPU_MODE : 2;
				uint8_t LYC_EQL_LY : 1;
				uint8_t MODE_0_SELECT : 1;
				uint8_t MODE_1_SELECT : 1;
				uint8_t MODE_2_SELECT : 1;
				uint8_t LYC_SELECT : 1;
			};

			uint8_t _LCD_STAT;
		};
		//scroll y
		uint8_t _SCY;
		//scroll x
		uint8_t _SCX;
		//line y
		uint8_t _LY;
		//line y count
		uint16_t _LYC;

		//DMA not a register in the same way

		//Window Y position
		uint8_t _WY;
		//Window X Position + 7
		uint8_t _WX;


		//Window Internal Line Counter
		uint8_t _WILC;


	};

}

