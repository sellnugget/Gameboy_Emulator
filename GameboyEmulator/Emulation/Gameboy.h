#pragma once

#include "Devices/SHARP_SM83_CPU.h"
#include "Devices/PPU.h"
#include "Devices/Cartridge.h"
#include "Devices/Ram.h"
#include "Devices/Registers.h"
#include "Devices/ClockUnit.h"
#include "SaveState.h"


#define DOTS_PER_FRAME 70224

namespace gb {
	class Gameboy
	{
	public:





		//all devices
		SHARP_SM83_CPU* cpu;
		Cartridge* cart;
		Ram* ram;
		PPU* ppu;
		Registers* registers;
		BUS* bus;
		ClockUnit* clockunit;

		


	
		Gameboy();

		void Clock();

		void Update();

		//resets gameboy
		void Reset();

		//pauses emulation
		void Pause();

		//unpauses emulation
		void Start();

		//sets the speed to a percentage of original
		void SetSpeed(float speed);


		SaveState saveState();

		void loadState(SaveState savestate);

	

	private:



		/*by default the Dots per Update is one frame but this can be
		changed to speed up or slow down emulation*/
		uint64_t DotsPerUpdate = DOTS_PER_FRAME;

		//used for resetting efficiently
		SaveState InitialState;

		//current state
		SaveState EmulatorState;


		bool Paused = true;
	};

}



