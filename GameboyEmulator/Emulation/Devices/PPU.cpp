#include "PPU.h"


#include <SFML/Graphics.hpp>

namespace gb {


	void PPU::Clock_DMA()
	{

		if (Transfer < 640) {

			if ((Transfer & 0b11) == 0) {
				OAM[Transfer >> 2] = ReadBus(Register.DMA << 8 + (Transfer >> 2));
			}
			Transfer++;
		}
		else if (Transfer == 640) {
			Transfer++;
		}
	}

	void PPU::EnterMode(int mode_number)
	{

	
		switch (mode_number) {
		case MODE0:
			//nothing happends all accessible
			if (Register.STAT.MODE_0_SELECT) {
				Bus->interupt_register.IF.LCD = true;
			}
			break;
		case MODE1:
	
			Bus->interupt_register.IF.VBLANK = true;
			if (Register.STAT.MODE_1_SELECT) {
				Bus->interupt_register.IF.LCD = true;
			}
			//nothing happends all accessible
			break;	
		case MODE2:

			if (Register.STAT.MODE_2_SELECT) {
				Bus->interupt_register.IF.LCD = true;
			}
		
			break;
		case MODE3:
			uint8_t* fet = (uint8_t*)&fetcher;
			for (int i = 0; i < sizeof(Fetcher); i++) {
				fet[i] = 0;
			}
			fet = (uint8_t*)&bgFIFO;
			for (int i = 0; i < sizeof(FIFO); i++) {
				fet[i] = 0;
			}
			fet = (uint8_t*)&objFIFO;
			for (int i = 0; i < sizeof(FIFO); i++) {
				fet[i] = 0;
			}
			ScrollOffset = Register.SCX % 8;
			bgFIFO.Empty = true;
			bgFIFO.Position = 0xf;
			break;
		}

		MODE = mode_number;
		Register.STAT.PPU_MODE = MODE;
	}

	bool PPU::InWindow()
	{
		return false;
	}

	void PPU::OnMode0()
	{


		if (Horizonal_Counter == 455) {


			if (Register.LY == 143) {
				Bus->CurrentIOstate.SwapBuffer();
				EnterMode(MODE1);
			}
			else {
				EnterMode(MODE2);
			}

		}

	}

	void PPU::OnMode1()
	{
		if (Horizonal_Counter == 455) {


			if (Register.LY == 153) {
				EnterMode(MODE2);
			}

		}
	}

	void PPU::OnMode2()
	{
		if (Horizonal_Counter == 79) {
			EnterMode(MODE3);
		}
	}

	void PPU::OnMode3()
	{
		FetchTile();
		if (Horizonal_Counter <= MODE3_START + (FETCH_LENGTH * 2)) {
			return;
		}
		if (ScrollOffset > 0) {

			bgFIFO.Pop();
			ScrollOffset--;
			return;
		}
		if (bgFIFO.Empty) {
			exit(-1);
		}
		PixelFIFO pixel = bgFIFO.Pop();
		
		if (!Register.LCDC.BG_WIN_ENABLE) {
			pixel.Color = 0;
		}


		struct ARGB{


			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		
			

			ARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) {
				this->a = a;
				this->r = r;
				this->g = g;
				this->b = b;
			}
			int toInt() {
				return *(int*)this;
			}
		};

		ARGB color(0,0,0);

		uint32_t pixelData = 0;

		switch (pixel.Color) {

		case 0:
			color = ARGB(155, 188, 15);
			
			pixelData = color.toInt();
			break;
		case 1:

			color = ARGB(139, 172, 15);
		
			pixelData = color.toInt();
			break;
		case 2:
			color = ARGB(48, 98, 48);
			pixelData = color.toInt();
			break;
		case 3:

			color = ARGB(15, 56, 15);
			pixelData = color.toInt();
			break;
		}


		Bus->CurrentIOstate.WritePixel(pixelData, Horizontal_Pixel, Register.LY);
		Horizontal_Pixel++;
		if (Horizontal_Pixel >= 160) {
			Horizontal_Pixel = 0;
			EnterMode(MODE0);
		}
	}


	void PPU::FetchTile()
	{

		
		if (fetcher.FetchStep == Fetcher::GET_TILE) {

			//need to add window support not currently implemented
			if (fetcher.ClocksInMode == 0) {



				//check if were in windowed

			

				uint16_t AddressToFetch = 0x9800;
				//first check which tilemap to use
				if (Register.LCDC.BG_TILE_MAP) {
					AddressToFetch = 0x9c00;
				}

				//offset based on X-Position-Counter
				AddressToFetch += fetcher.XPosition_Counter;
				//The value of SCX / 8 is also added if the Fetcher is not fetching Window pixels
				AddressToFetch += (Register.SCX / 8) & 0x1f;

				AddressToFetch += 32 * ((((Register.LY)+ Register.SCY) & 0xff) / 8);

				fetcher.Tile = VideoRam[AddressToFetch - VRAM_BEGIN];
			}
			else {
				fetcher.SetMode(Fetcher::GET_TILE_DATA_LOW);
			}
		}
		else if (fetcher.FetchStep == Fetcher::GET_TILE_DATA_LOW) {

			if (fetcher.ClocksInMode == 0) {
			
				uint16_t Address = 0x8800;
				if (Register.LCDC.BG_WIN_TILES) {
					Address = 0x8000;
				}
				Address += fetcher.Tile * 16;

				
				Address += 2 * (((Register.LY) + Register.SCY) % 8);

				fetcher.TileDataLow = VideoRam[Address - VRAM_BEGIN];
			}
			else {
				fetcher.SetMode(Fetcher::GET_TILE_DATA_HIGH);
			}

		}
		else if (fetcher.FetchStep == Fetcher::GET_TILE_DATA_HIGH) {


			if (fetcher.ClocksInMode == 0) {

				uint16_t Address = 0x8800;
				if (Register.LCDC.BG_WIN_TILES) {
					Address = 0x8000;

				}
				Address += fetcher.Tile * 16;
				Address += 2 * (((Register.LY) + Register.SCY) % 8) + 1;

				fetcher.TileDataHigh = VideoRam[Address - VRAM_BEGIN];
			}
			else {
				if (fetcher.FirstFetch) {
					fetcher.SetMode(Fetcher::PUSH);
				}
				else {
					fetcher.SetMode(Fetcher::GET_TILE);
					fetcher.FirstFetch = true;
				}
			
			}
		}
		else if (fetcher.FetchStep == Fetcher::PUSH) {


			if (bgFIFO.Empty) {
				fetcher.ClocksInMode = 0;
				bgFIFO.Push(fetcher.TileDataHigh, fetcher.TileDataLow, false);
			}
			else if (fetcher.ClocksInMode == 1) {

				fetcher.SetMode(Fetcher::GET_TILE);
				fetcher.XPosition_Counter++;
			}
			else {
				fetcher.ClocksInMode = -1;
			}
		}
		fetcher.ClocksInMode++;
	}

	void PPU::Init()
	{
		uint8_t* reg = (uint8_t*)&Register;

		for (int i = 0; i < sizeof(PPU_REGISTER); i++) {
			reg[i] = 0;
		}
		Transfer = 641;
		Type = PPU_TYPE;
		Register.LY = 0;
		StateSize = sizeof(PPU) - sizeof(Device);
		deviceState = (uint8_t*)this + sizeof(Device);
		MODE = MODE1;
		Bus->SetMemoryMap(PPU_TYPE, VRAM_BEGIN, VRAM_SIZE);
		Bus->SetMemoryMap(PPU_TYPE, OAM_BEGIN, OAM_SIZE);

		Bus->SetMemoryMap(PPU_TYPE, RegisterInfo::LCDC, 18);
	}

	uint8_t PPU::Read(uint16_t address)
	{

		if (address >= VRAM_BEGIN && address <= VRAM_END) {

			if (MODE == MODE3) {
				return 0xff;
			}

			return VideoRam[address - VRAM_BEGIN];
		}


		if (address >= OAM_BEGIN && address <= OAM_END) {

			if (MODE == MODE2 || MODE == MODE3) {
				return 0xff;
			}
			return OAM[address - OAM_BEGIN];
		}

		switch (address) {
		case RegisterInfo::LCDC:
			return Register.LCDC;
		case RegisterInfo::STAT:
			return Register.STAT;
		case RegisterInfo::SCY:
			return Register.SCY;
		case RegisterInfo::SCX:
			return Register.SCX;
		case RegisterInfo::LY:
			return Register.LY;
		case RegisterInfo::LYC:
			return Register.LYC;
			break;
		case RegisterInfo::DMA:
			return Register.DMA;
		case RegisterInfo::WY:
			return Register.WY;
		case  RegisterInfo::WX:
			return Register.WX;
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
		case RegisterInfo::LCDC:
			Register.LCDC = Byte;
			break;
		case RegisterInfo::STAT:
			Register.STAT = Byte;
			break;
		case RegisterInfo::SCY:
			Register.SCY = Byte;
			break;
		case RegisterInfo::SCX:
			Register.SCX = Byte;
			break;
		case RegisterInfo::LYC:
			Register.LYC = Byte;
			break;
		case RegisterInfo::DMA:
			Register.DMA = Byte;
			Transfer = 0;
			break;
		case RegisterInfo::WY:
			Register.WY = Byte;
			break;
		case RegisterInfo::WX:
			Register.WX = Byte;
			break;
		}
	}
	void PPU::Clock()
	{
		
		

		//DMA control
		Clock_DMA();
		

		


		switch (MODE) {

		case MODE0:
			OnMode0();
			break;
		case MODE1:
			OnMode1();
			break;
		case MODE2:
			OnMode2();
			break;
		case MODE3:
			OnMode3();

			break;
		}

		Register.STAT.LYC_EQL_LY = Register.LY == Register.LYC;

		if (Register.STAT.LYC_EQL_LY && Register.STAT.LYC_SELECT) {
			Bus->interupt_register.IF.LCD = true;
		}

		Horizonal_Counter++;
		if (Horizonal_Counter == 456) {
			Horizonal_Counter = 0;
			Horizontal_Pixel = 0;
			Register.LY++;
			if (Register.LY > 153) {
				Register.LY = 0;
			}
		}
		
	}


}
