#include "Gameboy.h"


namespace gb {
    Gameboy::Gameboy()
    {


        bus = new BUS();

        cpu = new SHARP_SM83_CPU(bus);
        cart = new Cartridge(bus);
        ram = new Ram(bus);
        ppu = new PPU(bus);
        registers = new Registers(bus);

        cart->LoadRom("C:\\Users\\naomi\\Downloads\\bgb\\Tetris (World) (Rev A).gb");

        for (int i = 0; i < END_TYPE; i++) {

            if (bus->_Devices[i] == NULL)
                continue;
            EmulatorState.Devices[i] = bus->_Devices[i]->deviceState;
            InitialState.Devices[i] = bus->_Devices[i]->SaveState();
           
        }

    }

    void Gameboy::Clock() {

        cpu->Clock();
        for (int i = 0; i < cpu->Cycle * 4; i++) {
            ppu->Clock();
            registers->Clock();

            bus->TimeCode++;

        }

    }

    void Gameboy::Update()
    {
        
        
        uint64_t DotsExecuted = 0;
        while (DotsExecuted < DotsPerUpdate && !Paused) {
            Clock();
            DotsExecuted += cpu->Cycle * 4;
        }
      
    }

    void Gameboy::Reset()
    {
        loadState(InitialState);
    }

    void Gameboy::Pause()
    {
        Paused = true;
    }

    void Gameboy::Start()
    {
        Paused = false;
    }

    SaveState Gameboy::saveState()
    {
        SaveState SaveState;

        SaveState.TimeCode = bus->TimeCode;
        for (int i = 0; i < END_TYPE; i++) {

            if (bus->_Devices[i] == NULL)
                continue;
            SaveState.Devices[i] = bus->_Devices[i]->SaveState();
        }
        std::memcpy(&SaveState.InteruptRegister, &bus->interupt_register, sizeof(BUS::InteruptRegister));
        return SaveState;
    }

    void Gameboy::loadState(SaveState savestate)
    {

        bus->TimeCode = savestate.TimeCode;
        for (int i = 0; i < END_TYPE; i++) {

            if (bus->_Devices[i] == NULL)
                continue;
            bus->_Devices[i]->LoadState(savestate.Devices[i]);
        }
        std::memcpy(&bus->interupt_register, &savestate.InteruptRegister,  sizeof(BUS::InteruptRegister));
    }

    void Gameboy::SetSpeed(float speed)
    {
        DotsPerUpdate = DOTS_PER_FRAME * speed;
    }



}

