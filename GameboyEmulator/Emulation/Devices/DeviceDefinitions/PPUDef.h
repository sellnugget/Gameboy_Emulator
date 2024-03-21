#pragma once
#include <inttypes.h>






namespace gb {



	//when pixels start being sent
#define MODE3_START 80
	//number of dots per tile fetch
#define FETCH_LENGTH 6

	//the ppu phase
	enum MODE_TYPE {
		//Waiting until the end of the scanline
		MODE0,
		//Wiating until the next frame
		MODE1,
		//Searching for the OBJs which overlap this line.
		MODE2,
		//Sending pixels to the LCD
		MODE3
	};

	
	struct PPU_REGISTER {
		//LCDC is the main LCD Control register. Its bits toggle what elements are displayed on the screen, and how.
		struct LCDC_REG {

			//BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 = On
			uint8_t BG_WIN_ENABLE : 1;

			//OBJ enable: 0 = Off; 1 = On
			uint8_t OBJ_ENABLE : 1;

			//OBJ size: 0 = 8×8; 1 = 8×16
			uint8_t OBJ_SIZE : 1;

			//BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
			uint8_t BG_TILE_MAP : 1;

			//BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF
			uint8_t BG_WIN_TILES : 1;

			//Window enable: 0 = Off; 1 = On
			uint8_t WIN_ENABLE : 1;

			//Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
			uint8_t WIN_TILE_MAP : 1;

			//LCD & PPU enable: 0 = Off; 1 = On
			uint8_t LCD_PPU_ENABLE : 1;

			operator uint8_t() const { return *(uint8_t*)this; }

			LCDC_REG& operator=(uint8_t value) { *(uint8_t*)this = value; return *this; }
		};


		struct STAT_REG {

			//PPU mode (Read-only): Indicates the PPU’s current status.
			uint8_t PPU_MODE : 2;
			//LYC == LY (Read-only): Set when LY contains the same value as LYC; it is constantly updated.
			uint8_t LYC_EQL_LY : 1;
			//Mode 0 int select (Read/Write): If set, selects the Mode 0 condition for the STAT interrupt.
			uint8_t MODE_0_SELECT : 1;
			//Mode 1 int select (Read/Write): If set, selects the Mode 1 condition for the STAT interrupt.
			uint8_t MODE_1_SELECT : 1;
			//Mode 2 int select (Read/Write): If set, selects the Mode 2 condition for the STAT interrupt.
			uint8_t MODE_2_SELECT : 1;
			//LYC int select (Read/Write): If set, selects the LYC == LY condition for the STAT interrupt.
			uint8_t LYC_SELECT : 1;
			uint8_t Empty : 1;

			operator uint8_t() const { return *(uint8_t*)this; }

			STAT_REG& operator=(uint8_t value) { *(uint8_t*)this = value; return *this; }
		};


		

		//LCD control	R/W
		LCDC_REG LCDC;
		//LCD status	Mixed
		STAT_REG STAT;
		//Viewport Y position	R/W
		uint8_t SCY;
		//Viewport X position	R/W
		uint8_t SCX;
		//LCD Y coordinate	R
		uint8_t LY;
		//LY compare	R/W
		uint8_t LYC;
		//OAM DMA source address & start	R/W
		uint8_t DMA;
		//Window Y position	R/W
		uint8_t WY;
		//Window X position + 7	R/W
		uint8_t WX;


	};





	struct PixelFIFO {
		uint8_t Color : 2;
		uint8_t Palette : 3;
		uint8_t Sprite_Priority : 2;
		uint8_t Background_Priority : 1;
	};

	struct FIFO {


	
		PixelFIFO Pixels[16];
		uint8_t Position : 4 = 0;
		bool Empty = false;

		//pops the top most pixel
		PixelFIFO Pop() {

			
			Position--;
			if (Position == 0xf) {
				Empty = true;
				return Pixels[0];
			}
			return Pixels[Position + 1];
		}
		//pushes a tile into the pixel stack in the order selected
		void Push(uint8_t Hbyte, uint8_t Lbyte, bool Flip) {

			Empty = false;

			Position = 7;
			
			if (Flip) {
				uint8_t Hnew = 0;
				uint8_t Lnew = 0;
				for (int i = 0; i < 8; i++) {

					//flips the bits locations
					Hnew += ((Hbyte >> (7 - i)) << i) & (1 << i);
					Lnew += ((Hbyte >> (7 - i)) << i) & (1 << i);
				}

				Hbyte = Hnew;
				Lbyte = Lnew;
			}

			for (int i = 0; i < 8; i++) {
				PixelFIFO topush;
				topush.Color = (Lbyte & 0b1) + ((Hbyte & 0b1) << 1);
				Pixels[i] = topush;
				Lbyte >>= 1;
				Hbyte >>= 1;
			}
		
		}



	};

	//this contains most registers used for rendering on the ppus
	struct Fetcher {


		enum FETCH_STEP {

			//simple gets a tile from from the right location
			GET_TILE,
			//gets the low byte of the tile
			GET_TILE_DATA_LOW,
			//gets the high byte of the tile
			GET_TILE_DATA_HIGH,
			//pushes current tile into fifo
			PUSH,
		};
		FETCH_STEP FetchStep = GET_TILE;


		//dots in mode so far
		int8_t ClocksInMode = 0;
		
		//offsets the address tile number is read from
		uint8_t XPosition_Counter;

		//number of scanlines with window pixels on it
		uint8_t Window_Line_Counter;

		//this is the tile index
		uint8_t Tile;

		//low byte of current tile
		uint8_t TileDataLow;

		//high byte of current tile
		uint8_t TileDataHigh;

		//Window Internal Line Counter
		uint8_t WILC;

		//false if its the first time fetching 
		bool FirstFetch;

		void SetMode(FETCH_STEP fetchstep) {

			FetchStep = fetchstep;
			ClocksInMode = -1;
		}
	
	};

}