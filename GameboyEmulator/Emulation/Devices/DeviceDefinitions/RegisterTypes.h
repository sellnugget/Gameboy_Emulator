#pragma once

#include <inttypes.h>



//some are not implemented in registers device but they are still defined here
enum HardwareRegister {
	P1_JOYP = 0xFF00,
	SB,
	SC,
	DIV = 0xFF04,
	TIMA,
	TMA,
	TAC,
	IF = 0xff0f,

	NR10 = 0xFF10,  //		Sound channel 1 sweep	R / W	All
	NR11 = 0xFF11,//	Sound channel 1 length timer & duty cycle	Mixed	All
	NR12 = 0xFF12,//	Sound channel 1 volume & envelope	R / W	All
	NR13 = 0xFF13,//	Sound channel 1 period low	W	All
	NR14 = 0xFF14,//	Sound channel 1 period high & control	Mixed	All
	NR21 = 0xFF16,//	Sound channel 2 length timer & duty cycle	Mixed	All
	NR22 = 0xFF17,//	Sound channel 2 volume & envelope	R / W	All
	NR23 = 0xFF18,//	Sound channel 2 period low	W	All
	NR24 = 0xFF19,//	Sound channel 2 period high & control	Mixed	All
	NR30 = 0xFF1A,//	Sound channel 3 DAC enable	R / W	All
	NR31 = 0xFF1B,//	Sound channel 3 length timer	W	All
	NR32 = 0xFF1C,//	Sound channel 3 output level	R / W	All
	NR33 = 0xFF1D,//	Sound channel 3 period low	W	All
	NR34 = 0xFF1E,//	Sound channel 3 period high & control	Mixed	All
	NR41 = 0xFF20,//	Sound channel 4 length timer	W	All
	NR42 = 0xFF21,//	Sound channel 4 volume & envelope	R / W	All
	NR43 = 0xFF22,//	Sound channel 4 frequency & randomness	R / W	All
	NR44 = 0xFF23,//	Sound channel 4 control	Mixed	All
	NR50 = 0xFF24,//	Master volume & VIN panning	R / W	All
	NR51 = 0xFF25,//	Sound panning	R / W	All
	NR52 = 0xFF26,//	Sound on / off	Mixed	All
	WAVE_RAM_START = 0xFF30,//Storage for one of the sound channels’ waveform	R / W	All
	WAVE_RAM_END = 0xFF3f,
	LCDC = 0xFF40,//	LCD control	R / W	All
	STAT = 0xFF41,//	LCD status	Mixed	All
	SCY = 0xFF42,//	Viewport Y position	R / W	All
	SCX = 0xFF43,//	Viewport X position	R / W	All
	LY = 0xFF44,//	LCD Y coordinate	R	All
	LYC = 0xFF45,// LY compare	R / W	All
	DMA = 0xFF46,//				OAM DMA source address & start	R / W	All
	BGP = 0xFF47,//		BG palette data	R / W	DMG
	OBP0 = 0xFF48,//		OBJ palette 0 data	R / W	DMG
	OBP1 = 0xFF49,//		OBJ palette 1 data	R / W	DMG
	WY = 0xFF4A, //	Window Y position	R / W	All
	WX = 0xFF4B,	//	Window X position plus 7	R / W	All
	KEY1 = 0xFF4D,//		Prepare speed switch	Mixed	CGB
	VBK = 0xFF4F,	//	VRAM bank	R / W	CGB
	HDMA1 = 0xFF51,//		VRAM DMA source high	W	CGB
	HDMA2 = 0xFF52,	//	VRAM DMA source low	W	CGB
	HDMA3 = 0xFF53,	//	VRAM DMA destination high	W	CGB
	HDMA4 = 0xFF54,	//	VRAM DMA destination low	W	CGB
	HDMA5 = 0xFF55,	//	VRAM DMA length / mode / start	R / W	CGB
	RP = 0xFF56,	//Infrared communications port	Mixed	CGB
	BCPS_BGPI = 0xFF68,	//	Background color palette specification / Background palette index	R / W	CGB
	BCPD_BGPD = 0xFF69,//		Background color palette data / Background palette data	R / W	CGB
	OCPS_OBPI = 0xFF6A,//		OBJ color palette specification / OBJ palette index	R / W	CGB
	OCPD_OBPD = 0xFF6B,	//	OBJ color palette data / OBJ palette data	R / W	CGB
	OPRI = 0xFF6C,		//Object priority mode	R / W	CGB
	SVBK = 0xFF70,	//	WRAM bank	R / W	CGB
	PCM12 = 0xFF76,	//	Audio digital outputs 1 & 2	R	CGB
	PCM34 = 0xFF77,//		Audio digital outputs 3 & 4	R	CGB
	IE = 0xFFFF,
};


struct InteruptRegister {


	struct REG {
		union {
			struct {
				unsigned char VBLANK : 1;
				unsigned char LCD : 1;
				unsigned char TIMER : 1;
				unsigned char SERIAL : 1;
				unsigned char JOYPAD : 1;
				unsigned char UNALLOCATED : 3;
			};
			unsigned char DATA;
		};
	};
	REG IE;
	REG IF;
	bool IME;
};

//controller 


struct JoyPad {
	union {

		struct {

			uint8_t A_Right : 1;

			uint8_t B_Left : 1;

			uint8_t Select_Up : 1;

			uint8_t Start_Down : 1;

			uint8_t Selectdpad : 1;

			uint8_t SelectButtons : 1;

			uint8_t Empty : 2;
		};

		uint8_t Register;
	};
};
