#pragma once

#include <iostream>
#include <vector>
#include "Registers.h"
#include "DeviceHandlers/Device.h"
#include "DeviceHandlers/BUS.h"
#include "DeviceDefinitions/PPUDef.h"
namespace gb {







	class PPU : public Device
	{
	public:
		


		PPU(BUS* bus) { BaseInit(bus); }

		uint8_t VideoRam[VRAM_SIZE];




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


		//all registers accessible by the cpu are stored in Register
		PPU_REGISTER Register;

	private:

		//current phase of PPU
		uint8_t MODE;
		//controls the transfer and length. if -1 then no transfer is underway
		int Transfer = -1;
		
		//keep track of number of clock cycles since begining this line
		uint16_t Horizonal_Counter = 0;


		//only used in mode 3
		//Fetcher contains most of the registers used by the ppu while drawing the scene
		Fetcher fetcher;
	

		//current background first in first out
		FIFO bgFIFO;

		//current object first in first out
		FIFO objFIFO;

		uint8_t Horizontal_Pixel = 0;

		//keeps track of number of pixels that need to be discarded at start of scanline
		uint8_t ScrollOffset = 0;

		void Clock_DMA();

		void EnterMode(int mode_number);


		//checks if rendering is currently in window
		bool InWindow();


		void OnMode0();

		void OnMode1();

		void OnMode2();






		void OnMode3();


		//runs the fetcher for one cycle
		void FetchTile();



	};

}

