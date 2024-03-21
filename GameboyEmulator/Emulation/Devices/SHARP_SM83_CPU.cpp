#include "SHARP_SM83_CPU.h"



namespace gb {
    void SHARP_SM83_CPU::Init()
    {

        Registers.AF = 0x01b0;
        Registers.A16= 0x0000;
        Registers.BC = 0x0013;
        Registers.DE = 0x00d8;
        Registers.PC = 0x0100;
        Registers.SP = 0xfffe;
        Registers.HL = 0x014d;
        HALTED = false;


        Type = CPU_TYPE;
        StateSize = sizeof(SHARP_SM83_CPU) - sizeof(Device) - sizeof(FunctionDefs) - sizeof(LoadedInstruction);
        deviceState = (uint8_t*)this + sizeof(Device);
        //instruction defintions
        FunctionDefs = {
            [this]() { ld_r_r(); },
            [this]() { ld_r_n(); },
            [this]() { ld_r_$HL$(); },
            [this]() { ld_$HL$_r(); },
            [this]() { ld_$HL$_n(); },
            [this]() { ld_A_$BC$(); },
            [this]() { ld_A_$DE$(); },
            [this]() { ld_A_$nn$(); },
            [this]() { ld_$BC$_A(); },
            [this]() { ld_$DE$_A(); },
            [this]() { ld_$nn$_A(); },
            [this]() { ld_A_$FF00_n$(); },
            [this]() { ld_$FF00_n$_A(); },
            [this]() { ld_A_$FF00_C$(); },
            [this]() { ld_$FF00_C$_A(); },
            [this]() { ldi_$HL$_A(); },
            [this]() { ldi_A_$HL$(); },
            [this]() { ldd_$HL$_A(); },
            [this]() { ldd_A_$HL$(); },
            [this]() { ld_rr_nn(); },
            [this]() { ld_$nn$_SP(); },
            [this]() { ld_SP_HL(); },
            [this]() { push_rr(); },
            [this]() { pop_rr(); },
            [this]() { add_A_r(); },
            [this]() { add_A_n(); },
            [this]() { add_A_$HL$(); },
            [this]() { adc_A_r(); },
            [this]() { adc_A_n(); },
            [this]() { adc_A_$HL$(); },
            [this]() { sub_r(); },
            [this]() { sub_n(); },
            [this]() { sub_$HL$(); },
            [this]() { sbc_A_r(); },
            [this]() { sbc_A_n(); },
            [this]() { sbc_A_$HL$(); },
            [this]() { and_r(); },
            [this]() { and_n(); },
            [this]() { and_$HL$(); },
            [this]() { xor_r(); },
            [this]() { xor_n(); },
            [this]() { xor_$HL$(); },
            [this]() { or_r(); },
            [this]() { or_n(); },
            [this]() { or_$HL$(); },
            [this]() { cp_r(); },
            [this]() { cp_n(); },
            [this]() { cp_$HL$(); },
            [this]() { inc_r(); },
            [this]() { inc_$HL$(); },
            [this]() { dec_r(); },
            [this]() { dec_$HL$(); },
            [this]() { daa(); },
            [this]() { cpl(); },
            [this]() { add_HL_rr(); },
            [this]() { inc_rr(); },
            [this]() { dec_rr(); },
            [this]() { add_SP_dd(); },
            [this]() { ld_HL_SP_dd(); },
            [this]() { rlca(); },
            [this]() { rla(); },
            [this]() { rrca(); },
            [this]() { rra(); },
            [this]() { rlc_r(); },
            [this]() { rlc_$HL$(); },
            [this]() { rl_r(); },
            [this]() { rl_$HL$(); },
            [this]() { rrc_r(); },
            [this]() { rrc_$HL$(); },
            [this]() { rr_r(); },
            [this]() { rr_$HL$(); },
            [this]() { sla_r(); },
            [this]() { sla_$HL$(); },
            [this]() { swap_r(); },
            [this]() { swap_$HL$(); },
            [this]() { sra_r(); },
            [this]() { sra_$HL$(); },
            [this]() { srl_r(); },
            [this]() { srl_$HL$(); },
            [this]() { bit_n_r(); },
            [this]() { bit_n_$HL$(); },
            [this]() { set_n_r(); },
            [this]() { set_n_$HL$(); },
            [this]() { res_n_r(); },
            [this]() { res_n_$HL$(); },
            [this]() { ccf(); },
            [this]() { scf(); },
            [this]() { nop(); },
            [this]() { halt(); },
            [this]() { stop(); },
            [this]() { di(); },
            [this]() { ei(); },
            [this]() { jp_nn(); },
            [this]() { jp_HL(); },
            [this]() { jp_f_nn(); },
            [this]() { jr_PC_dd(); },
            [this]() { jr_f_PC_dd(); },
            [this]() { call_nn(); },
            [this]() { call_f_nn(); },
            [this]() { ret(); },
            [this]() { ret_f(); },
            [this]() { reti(); },
            [this]() { rst_n(); },
        };
    }

    void SHARP_SM83_CPU::Clock()
    {



       

        //interupt handler 
        if (Bus->interupt_register.IME) {

            if (Bus->interupt_register.IE.VBLANK && Bus->interupt_register.IF.VBLANK) {
                Bus->interupt_register.IME = false;
                Bus->interupt_register.IF.VBLANK = false;
                Registers.A16 = 0x0040;
                call_nn();
                Cycle = 5;
                TotalCycles += Cycle;
                HALTED = false;
                return;
            }
            else if (Bus->interupt_register.IE.LCD && Bus->interupt_register.IF.LCD) {
                Bus->interupt_register.IME = false;
                Bus->interupt_register.IF.LCD = false;
                Registers.A16 = 0x0048;
                call_nn();
                Cycle = 5;
                TotalCycles += Cycle;
                HALTED = false;
                return;
            }
            else if (Bus->interupt_register.IE.TIMER && Bus->interupt_register.IF.TIMER) {
                Bus->interupt_register.IME = false;
                Bus->interupt_register.IF.TIMER = false;
                Registers.A16 = 0x0050;
                call_nn();
                Cycle = 5;
                TotalCycles += Cycle;
                HALTED = false;
                return;
            }
            else if (Bus->interupt_register.IE.SERIAL && Bus->interupt_register.IF.SERIAL) {
                Bus->interupt_register.IME = false;
                Bus->interupt_register.IF.SERIAL = false;
                Registers.A16 = 0x0058;
                call_nn();
                Cycle = 5;
                TotalCycles += Cycle;
                HALTED = false;
                return;
            }
            else if (Bus->interupt_register.IE.JOYPAD && Bus->interupt_register.IF.JOYPAD) {
                Bus->interupt_register.IME = false;
                Bus->interupt_register.IF.JOYPAD = false;
                Registers.A16 = 0x0060;
                call_nn();
                Cycle = 5;
                TotalCycles += Cycle;
                HALTED = false;
                return;
            }
  
        }

        if (HALTED) {
            return;
        }



        if (IME_Buffer) {
            Bus->interupt_register.IME = true;
            IME_Buffer = false;
        }

        if (Registers.PC == 0x020B) {
            std::cout << "BreakPoint\n";
        }
        //first determine which instruction we need to execute is
        uint8_t OpCode = ReadBYTEPC();

        if (OpCode == 0xcb) {
            OpCode = ReadBYTEPC();
            LoadedInstruction = CPU_INFO::cbprefixed[OpCode];
        }
        else {
            LoadedInstruction = CPU_INFO::unprefixed[OpCode];

            if (LoadedInstruction.Size == 2) {
                Registers.D1 = ReadBYTEPC();
            }
            if (LoadedInstruction.Size == 3) {
                Registers.A16 = ReadShortPC();

            }
        }
     
        FunctionDefs[LoadedInstruction.instructionType]();

        Registers.Ground = 0;
        Cycle = LoadedInstruction.Cycles;
        TotalCycles += Cycle;
    }

    uint8_t SHARP_SM83_CPU::ReadBYTEPC()
    {
        uint8_t a = ReadBus(Registers.PC);
        Registers.PC++;
        return a;
    }

    uint16_t SHARP_SM83_CPU::ReadShortPC()
    {
        uint16_t a = ReadBYTEPC();
        a += (uint16_t)ReadBYTEPC() << 8;
        return a;
    }

    uint8_t* SHARP_SM83_CPU::Get_8BIT_Register(CPU_INFO::ReferenceName ref)
    {
        switch (ref) {
        case CPU_INFO::REF_A:
            return (uint8_t*)&Registers.AF + 1;
        case CPU_INFO::REF_B:
            return (uint8_t*)&Registers.BC + 1;
        case CPU_INFO::REF_C:

            return  (uint8_t*)&Registers.BC;
        case CPU_INFO::REF_D:

            return  (uint8_t*)&Registers.DE + 1;
        case CPU_INFO::REF_E:

            return  (uint8_t*)&Registers.DE;
        case CPU_INFO::REF_H:

            return  (uint8_t*)&Registers.HL + 1;
        case CPU_INFO::REF_L:

            return (uint8_t*)&Registers.HL;

        default:
            return NULL;
        }
    }

    uint16_t* SHARP_SM83_CPU::Get_16BIT_Register(CPU_INFO::ReferenceName ref)
    {

        switch (ref) {
        case CPU_INFO::REF_HL:
            return &Registers.HL;
        case CPU_INFO::REF_SP:
            return &Registers.SP;
        case CPU_INFO::REF_BC:
            return &Registers.BC;
        case CPU_INFO::REF_DE:
            return &Registers.DE;
        case CPU_INFO::REF_AF:
            return &Registers.AF;
        default:
            return NULL;
        }
    }

    bool SHARP_SM83_CPU::GetFlag(CPU_INFO::ReferenceName ref)
    {
        bool Flag = false;
        switch (ref) {
        case CPU_INFO::REF_NZ:
            Flag = !Registers.zero;
            break;
        case CPU_INFO::REF_Z:
            Flag = Registers.zero;
            break;
        case CPU_INFO::REF_NC:
            Flag = !Registers.Carry;
            break;
        case CPU_INFO::REF_C:
            Flag = Registers.Carry;
            break;
        }
        return Flag;
    }

    void SHARP_SM83_CPU::SetZeroFlag(uint8_t a)
    {
        switch (LoadedInstruction.flagstate.Z) {
        case CPU_INFO::FLAG_CONDITION:
            if (a == 0) {
                Registers.zero = 1;
            }
            else {
                Registers.zero = 0;
            }
            break;
        case CPU_INFO::FLAG_SET_0:
            Registers.zero = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.zero = 1;
            break;
        }
    }

    void SHARP_SM83_CPU::SetFlagsDirect(uint8_t a)
    {
        switch (LoadedInstruction.flagstate.N) {
        case CPU_INFO::FLAG_SET_0:
            Registers.Subtract = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Subtract = 1;
            break;
        }

        SetZeroFlag(a);
        switch (LoadedInstruction.flagstate.H) {
        case CPU_INFO::FLAG_SET_0:
            Registers.Half_Carry = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Half_Carry = 1;
            break;
        }
        switch (LoadedInstruction.flagstate.C) {
        case CPU_INFO::FLAG_SET_0:
            Registers.Carry = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Carry = 1;
            break;
        }
    }

    void SHARP_SM83_CPU::SetFlags8bit_MATH(uint8_t a, uint16_t b)
    {



        switch (LoadedInstruction.flagstate.N) {
        case CPU_INFO::FLAG_SET_0:
            Registers.Subtract = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Subtract = 1;
            break;
        }


        uint8_t ValueAfter = a + b;
        if (Registers.Subtract) {
            ValueAfter = a - b;
        }

        SetZeroFlag(ValueAfter);




        switch (LoadedInstruction.flagstate.H) {
        case CPU_INFO::FLAG_CONDITION:
            if ((Registers.Subtract && (a & 0x0f) < (b & 0x0f)) || (!Registers.Subtract && 0xf - (a & 0xf) < (b & 0xf))) {
                Registers.Half_Carry = 1;
            }
            else {
                Registers.Half_Carry = 0;
            }
            break;
        case CPU_INFO::FLAG_SET_0:
            Registers.Half_Carry = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Half_Carry = 1;
            break;
        }
        switch (LoadedInstruction.flagstate.C) {
        case CPU_INFO::FLAG_CONDITION:
            if ((Registers.Subtract && a < b) || (!Registers.Subtract && 0xff - a < b)) {
                Registers.Carry = 1;
            }
            else {
                Registers.Carry = 0;
            }
            break;
        case CPU_INFO::FLAG_SET_0:
            Registers.Carry = 0;
            break;
        case CPU_INFO::FLAG_SET_1:
            Registers.Carry = 1;
            break;
        }
    }

    void SHARP_SM83_CPU::ld_r_r()
    {
        uint8_t* a = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        uint8_t* b = Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        *a = *b;
    }

    void SHARP_SM83_CPU::ld_r_n()
    {
        uint8_t* a = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        *a = Registers.D1;
    }

    void SHARP_SM83_CPU::ld_r_$HL$()
    {
        uint8_t* a = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        *a = ReadBus(Registers.HL);
    }

    void SHARP_SM83_CPU::ld_$HL$_r()
    {
        uint8_t* b = Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        WriteBus(Registers.HL, *b);
    }

    void SHARP_SM83_CPU::ld_$HL$_n()
    {
        WriteBus(Registers.HL, Registers.D1);
    }

    void SHARP_SM83_CPU::ld_A_$BC$()
    {
        Registers.A = ReadBus(Registers.BC);
    }

    void SHARP_SM83_CPU::ld_A_$DE$()
    {
        Registers.A = ReadBus(Registers.DE);
    }

    void SHARP_SM83_CPU::ld_A_$nn$()
    {
        Registers.A = ReadBus(Registers.A16);
    }

    void SHARP_SM83_CPU::ld_$BC$_A()
    {
        WriteBus(Registers.BC, Registers.A);
    }

    void SHARP_SM83_CPU::ld_$DE$_A()
    {
        WriteBus(Registers.DE, Registers.A);
    }

    void SHARP_SM83_CPU::ld_$nn$_A()
    {
        WriteBus(Registers.A16, Registers.A);
    }

    void SHARP_SM83_CPU::ld_A_$FF00_n$()
    {
        Registers.A = ReadBus(0xFF00 + Registers.D1);
    }

    void SHARP_SM83_CPU::ld_$FF00_n$_A()
    {
        WriteBus(0xFF00 + Registers.D1, Registers.A);
    }

    void SHARP_SM83_CPU::ld_A_$FF00_C$()
    {
        Registers.A = ReadBus(0xFF00 + Registers.C);
    }

    void SHARP_SM83_CPU::ld_$FF00_C$_A()
    {
        WriteBus(0xFF00 + Registers.C, Registers.A);
    }

    void SHARP_SM83_CPU::ldi_$HL$_A()
    {
        WriteBus(Registers.HL, Registers.A);
        Registers.HL++;
    }

    void SHARP_SM83_CPU::ldi_A_$HL$()
    {
        Registers.A = ReadBus(Registers.HL);
        Registers.HL++;
    }

    void SHARP_SM83_CPU::ldd_$HL$_A()
    {
        WriteBus(Registers.HL, Registers.A);
        Registers.HL--;
    }

    void SHARP_SM83_CPU::ldd_A_$HL$()
    {
        Registers.A = ReadBus(Registers.HL);
        Registers.HL--;
    }

    void SHARP_SM83_CPU::ld_rr_nn()
    {
        uint16_t* rr = Get_16BIT_Register(LoadedInstruction.Operands[0].name);
        *rr = Registers.A16;
    }

    void SHARP_SM83_CPU::ld_$nn$_SP()
    {
        WriteWord(Registers.A16, Registers.SP);
    }

    void SHARP_SM83_CPU::ld_SP_HL()
    {
        Registers.SP = Registers.HL;
    }

    void SHARP_SM83_CPU::push_rr()
    {
        Registers.SP -= 2;
        uint16_t* rr = Get_16BIT_Register(LoadedInstruction.Operands[0].name);
        WriteWord(Registers.SP, *rr);
    }

    void SHARP_SM83_CPU::pop_rr()
    {
        uint16_t* rr = Get_16BIT_Register(LoadedInstruction.Operands[0].name);
        *rr = ReadWord(Registers.SP);
        Registers.SP += 2;
    }

    void SHARP_SM83_CPU::add_A_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);


        SetFlags8bit_MATH(Registers.A, *r);
        Registers.A += *r;
    }

    void SHARP_SM83_CPU::add_A_n()
    {
        SetFlags8bit_MATH(Registers.A, Registers.D1);
        Registers.A += Registers.D1;
    }

    void SHARP_SM83_CPU::add_A_$HL$()
    {
        uint8_t Value = ReadBus(Registers.HL);
        SetFlags8bit_MATH(Registers.A, Value);
        Registers.A += Value;
    }

    void SHARP_SM83_CPU::adc_A_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);

        uint16_t value = *r + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A += value;
    }

    void SHARP_SM83_CPU::adc_A_n()
    {

        uint16_t value = Registers.D1 + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A += value;
    }

    void SHARP_SM83_CPU::adc_A_$HL$()
    {
        uint16_t value = ReadBus(Registers.HL) + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A += value;
    }

    void SHARP_SM83_CPU::sub_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);


        SetFlags8bit_MATH(Registers.A, *r);
        Registers.A -= *r;
    }

    void SHARP_SM83_CPU::sub_n()
    {
        SetFlags8bit_MATH(Registers.A, Registers.D1);
        Registers.A -= Registers.D1;
    }

    void SHARP_SM83_CPU::sub_$HL$()
    {
        uint8_t Value = ReadBus(Registers.HL);
        SetFlags8bit_MATH(Registers.A, Value);
        Registers.A -= Value;
    }

    void SHARP_SM83_CPU::sbc_A_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);

        uint16_t value = *r + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A -= value;
    }

    void SHARP_SM83_CPU::sbc_A_n()
    {
        uint16_t value = Registers.D1 + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A -= value;
    }

    void SHARP_SM83_CPU::sbc_A_$HL$()
    {
        uint16_t value = ReadBus(Registers.HL) + (uint16_t)Registers.Carry;
        SetFlags8bit_MATH(Registers.A, value);
        Registers.A -= value;
    }

    void SHARP_SM83_CPU::and_r()
    {
        uint8_t value = *Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        Registers.A &= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::and_n()
    {
        uint8_t value = Registers.D1;
        Registers.A &= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::and_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        Registers.A &= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::xor_r()
    {
        uint8_t value = *Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        Registers.A ^= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::xor_n()
    {
        uint8_t value = Registers.D1;
        Registers.A ^= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::xor_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        Registers.A ^= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::or_r()
    {
        uint8_t value = *Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        Registers.A |= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::or_n()
    {
        uint8_t value = Registers.D1;
        Registers.A |= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::or_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        Registers.A |= value;
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::cp_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        SetFlags8bit_MATH(Registers.A, *r);
    }

    void SHARP_SM83_CPU::cp_n()
    {
        SetFlags8bit_MATH(Registers.A, Registers.D1);
    }

    void SHARP_SM83_CPU::cp_$HL$()
    {
        uint8_t Value = ReadBus(Registers.HL);
        SetFlags8bit_MATH(Registers.A, Value);
    }

    void SHARP_SM83_CPU::inc_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        SetFlags8bit_MATH(*r, 1);
        (*r)++;
    }

    void SHARP_SM83_CPU::inc_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        SetFlags8bit_MATH(value, 1);
        WriteBus(Registers.HL, value + 1);
    }

    void SHARP_SM83_CPU::dec_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        SetFlags8bit_MATH(*r, 1);
        (*r)--;
    }

    void SHARP_SM83_CPU::dec_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        SetFlags8bit_MATH(value, 1);
        WriteBus(Registers.HL, value - 1);
    }

    void SHARP_SM83_CPU::daa()
    {
        // note: assumes a is a uint8_t and wraps from 0xff to 0
        if (!Registers.Subtract) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
            if (!Registers.Carry || Registers.A > 0x99) { Registers.A += 0x60; Registers.Carry = 1; }
            if (Registers.Half_Carry || (Registers.A & 0x0f) > 0x09) { Registers.A += 0x6; }
        }
        else {  // after a subtraction, only adjust if (half-)carry occurred
            if (Registers.Carry) { Registers.A -= 0x60; }
            if (Registers.Half_Carry) { Registers.A -= 0x6; }
        }
        // these flags are always updated
        Registers.zero = (Registers.A == 0); // the usual z flag
        Registers.Half_Carry = 0; // h flag is always cleared
    }

    void SHARP_SM83_CPU::cpl()
    {
        SetFlagsDirect(0);
        Registers.A ^= 0xff;
    }

    void SHARP_SM83_CPU::add_HL_rr()
    {
        uint16_t Value = Registers.HL + *Get_16BIT_Register(LoadedInstruction.Operands[1].name);
        if (Value < Registers.HL) {
            Registers.Carry = true;
        }
        else {
            Registers.Carry = false;
        }
        if ((Value & 0xfff) < (Registers.HL & 0xfff)) {
            Registers.Half_Carry = true;
        }
        else {
            Registers.Half_Carry = false;
        }
        Registers.HL = Value;
    }

    void SHARP_SM83_CPU::inc_rr()
    {
        (*Get_16BIT_Register(LoadedInstruction.Operands[0].name))++;
    }

    void SHARP_SM83_CPU::dec_rr()
    {
        (*Get_16BIT_Register(LoadedInstruction.Operands[0].name))--;
    }

    void SHARP_SM83_CPU::add_SP_dd()
    {
        SetFlagsDirect(0);
        int8_t value = Registers.D1;

        uint16_t newSP = Registers.SP + value;
        if (value >= 0) {
            if (newSP < Registers.SP) {
                Registers.Carry = true;
            }
            else {
                Registers.Carry = false;
            }
            if ((newSP & 0xfff) < (Registers.SP & 0xfff)) {
                Registers.Half_Carry = true;
            }
            else {
                Registers.Half_Carry = false;
            }

        }
        else {
            if (newSP > Registers.SP) {
                Registers.Carry = true;
            }
            else {
                Registers.Carry = false;
            }
            if ((newSP & 0xfff) > (Registers.SP & 0xfff)) {
                Registers.Half_Carry = true;
            }
            else {
                Registers.Half_Carry = false;
            }
        }
        Registers.SP = newSP;
    }

    void SHARP_SM83_CPU::ld_HL_SP_dd()
    {
        SetFlagsDirect(0);
        int8_t value = Registers.D1;

        uint16_t newSP = Registers.SP + value;
        if (value >= 0) {
            if (newSP < Registers.SP) {
                Registers.Carry = true;
            }
            else {
                Registers.Carry = false;
            }
            if ((newSP & 0xfff) < (Registers.SP & 0xfff)) {
                Registers.Half_Carry = true;
            }
            else {
                Registers.Half_Carry = false;
            }

        }
        else {
            if (newSP > Registers.SP) {
                Registers.Carry = true;
            }
            else {
                Registers.Carry = false;
            }
            if ((newSP & 0xfff) > (Registers.SP & 0xfff)) {
                Registers.Half_Carry = true;
            }
            else {
                Registers.Half_Carry = false;
            }
        }
        Registers.HL = newSP;
    }

    void SHARP_SM83_CPU::rlca()
    {
        uint8_t old = Registers.A;
        Registers.A <<= 1;
        if ((old & 0b10000000) != 0) {
            Registers.A |= 1;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        SetFlagsDirect(Registers.A);

    }

    void SHARP_SM83_CPU::rla()
    {
        SetFlagsDirect(0);
        bool newCarry = (Registers.A & 0b10000000) >> 7;
        Registers.A <<= 1;
        if (Registers.Carry) {
            Registers.A |= 1;
        }
        Registers.Carry = newCarry;
    }

    void SHARP_SM83_CPU::rrca()
    {

        uint8_t old = Registers.A;
        Registers.A >>= 1;
        if ((old & 0b00000001) != 0) {
            Registers.A |= 0b10000000;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        SetFlagsDirect(Registers.A);
    }

    void SHARP_SM83_CPU::rra()
    {
        SetFlagsDirect(0);
        bool newCarry = Registers.A & 1;
        Registers.A >>= 1;
        if (Registers.Carry) {
            Registers.A |= 0b10000000;
        }
        Registers.Carry = newCarry;
    }

    void SHARP_SM83_CPU::rlc_r()
    {



        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);

        uint8_t old = *r;
        *r <<= 1;
        if ((old & 0b10000000) != 0) {
            *r |= 1;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        SetFlagsDirect(*r);

    }

    void SHARP_SM83_CPU::rlc_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        uint8_t old = value;
        value <<= 1;
        if ((value & 0b10000000) != 0) {
            value |= 1;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::rl_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        bool newCarry = (*r & 0b10000000) >> 7;
        *r <<= 1;
        if (Registers.Carry) {
            *r |= 1;
        }
        Registers.Carry = newCarry;

        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::rl_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        bool newCarry = (value & 0b10000000) >> 7;
        value <<= 1;
        if (Registers.Carry) {
            value |= 1;
        }
        Registers.Carry = newCarry;

        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::rrc_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);

        uint8_t old = *r;
        *r >>= 1;
        if ((old & 0b00000001) != 0) {
            *r |= 0b10000000;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::rrc_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        uint8_t old = value;
        value >>= 1;
        if ((old & 0b00000001) != 0) {
            value |= 0b10000000;
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::rr_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        bool newCarry = *r & 1;
        *r >>= 1;
        if (Registers.Carry) {
            *r |= 0b10000000;
        }
        Registers.Carry = newCarry;

        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::rr_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        bool newCarry = value & 1;
        value >>= 1;
        if (Registers.Carry) {
            value |= 0b10000000;
        }
        Registers.Carry = newCarry;

        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::sla_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        if ((*r & 0b10000000) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        *r <<= 1;

        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::sla_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        if ((value & 0b10000000) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        value <<= 1;

        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);

    }

    void SHARP_SM83_CPU::swap_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        *r = (*r >> 4) + ((*r & 0x0f) << 4);
        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::swap_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        value = (value >> 4) + ((value & 0x0f) << 4);
        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::sra_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        uint8_t old = *r;
        if ((*r & 1) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        *r >>= 1;
        *r |= (0b10000000 & old);
        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::sra_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        uint8_t old = value;
        if ((value & 1) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        value >>= 1;
        value |= (0b10000000 & old);
        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::srl_r()
    {
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[0].name);
        if ((*r & 1) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        *r >>= 1;
        SetFlagsDirect(*r);
    }

    void SHARP_SM83_CPU::srl_$HL$()
    {
        uint8_t value = ReadBus(Registers.HL);
        if ((value & 1) != 0) {
            Registers.Carry = 1;
        }
        else {
            Registers.Carry = 0;
        }
        value >>= 1;
        SetFlagsDirect(value);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::bit_n_r()
    {
        int Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        Registers.zero = !(*Get_8BIT_Register(LoadedInstruction.Operands[1].name) >> Index & 1);
        Registers.H = 1;
        Registers.Subtract = 0;
    }

    void SHARP_SM83_CPU::bit_n_$HL$()
    {
        int Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        Registers.zero = !(ReadBus(Registers.HL) >> Index & 1);
        Registers.H = 1;
        Registers.Subtract = 0;
    }

    void SHARP_SM83_CPU::set_n_r()
    {
        int Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        *r |= 1 << Index;
    }

    void SHARP_SM83_CPU::set_n_$HL$()
    {
        int Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        uint8_t value = ReadBus(Registers.HL);
        value |= 1 << Index;
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::res_n_r()
    {
        uint8_t Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        uint8_t* r = Get_8BIT_Register(LoadedInstruction.Operands[1].name);
        *r &= ~(uint8_t)(1 << Index);
    }

    void SHARP_SM83_CPU::res_n_$HL$()
    {
        int Index = LoadedInstruction.Operands[0].name - CPU_INFO::REF_0;
        uint8_t value = ReadBus(Registers.HL);
        value &= ~(1 << Index);
        WriteBus(Registers.HL, value);
    }

    void SHARP_SM83_CPU::ccf()
    {
        SetFlagsDirect(0);
        Registers.Carry = ~Registers.Carry;
    }

    void SHARP_SM83_CPU::scf()
    {
        SetFlagsDirect(0);
    }

    void SHARP_SM83_CPU::nop()
    {

    }

    void SHARP_SM83_CPU::halt()
    {
        HALTED = true;
    }

    void SHARP_SM83_CPU::stop()
    {
        STOPED = true;
    }

    void SHARP_SM83_CPU::di()
    {

        Bus->interupt_register.IME = false;
    }

    void SHARP_SM83_CPU::ei()
    {
        IME_Buffer = true;
    }

    void SHARP_SM83_CPU::jp_nn()
    {
        Registers.PC = Registers.A16;
    }

    void SHARP_SM83_CPU::jp_HL()
    {
        Registers.PC = Registers.HL;
    }

    void SHARP_SM83_CPU::jp_f_nn()
    {

        if (GetFlag(LoadedInstruction.Operands[0].name)) {
            jp_nn();

        }
        else {
            LoadedInstruction.Cycles = 12;
        }
    }

    void SHARP_SM83_CPU::jr_PC_dd()
    {
        Registers.PC += (int8_t)Registers.D1;
    }

    void SHARP_SM83_CPU::jr_f_PC_dd()
    {
        if (GetFlag(LoadedInstruction.Operands[0].name)) {
            jr_PC_dd();
        }
        else {
            LoadedInstruction.Cycles = 8;
        }
    }

    void SHARP_SM83_CPU::call_nn()
    {
        Registers.SP -= 2;
        WriteWord(Registers.SP, Registers.PC);
        Registers.PC = Registers.A16;
    }

    void SHARP_SM83_CPU::call_f_nn()
    {
        if (GetFlag(LoadedInstruction.Operands[0].name)) {
            call_nn();
        }
        else {
            LoadedInstruction.Cycles = 12;
        }

    }

    void SHARP_SM83_CPU::ret()
    {
        Registers.PC = ReadWord(Registers.SP);
        Registers.SP += 2;
    }

    void SHARP_SM83_CPU::ret_f()
    {
        if (GetFlag(LoadedInstruction.Operands[0].name)) {
            ret();
        }
        else {
            LoadedInstruction.Cycles = 8;
        }
    }

    void SHARP_SM83_CPU::reti()
    {
        ei();
        ret();

    }

    void SHARP_SM83_CPU::rst_n()
    {
        int value = LoadedInstruction.Operands[0].name - CPU_INFO::REF_$00;
        Registers.A16 = value * 0x08;
        call_nn();
    }


}



