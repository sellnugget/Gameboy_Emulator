#include "Cartridge.h"
namespace gb {
    void Cartridge::LoadRom(std::string Path)
    {
        std::ifstream rf(Path, std::ios::out | std::ios::binary);
        RomData = new uint8_t[0x8000];
        if (!rf) {
            std::cout << "Failed to open rom\n";
            exit(-1);
        }

        rf.read((char*)RomData, 0x8000);


        rf.close();

        struct Header {
            char Title[16];
            char NewLicensee[2];
            uint8_t SuperGameboy;
            uint8_t CartType;
            uint8_t CartSize;
            uint8_t SaveRam_Size;
            uint8_t CountryCode;

            uint8_t Licensee;
        };


    }

    void Cartridge::Init()
    {
        Type = CART_TYPE;
        StateSize = sizeof(Cartridge) - sizeof(Device) - sizeof(RomData);
        deviceState = (uint8_t*)this + sizeof(Device);

        Bus->SetMemoryMap(CART_TYPE, CART_BEGIN, CART_SIZE);
    }

    uint8_t Cartridge::Read(uint16_t Address)
    {
        return RomData[Address];
    }

}
