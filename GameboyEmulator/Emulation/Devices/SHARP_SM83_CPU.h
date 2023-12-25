#pragma once
#include <iostream>
#include "DeviceDefinitions/CPUDefs.h"
#include "DeviceHandlers/Device.h"
#include <functional>
#include <vector>
#include "DeviceHandlers/BUS.h"
#include "Registers.h"






namespace gb {
    /*
    DMG: clocked at 4.194304 MHz
    CGB: optionally clocked at 8.388608 MHz
    Two types of cycles: T states and M cycles
        M cycles (“machine” cycles, 1:4 clock) are the base unit of CPU instructions
        T states or T cycles (“transistor”(?) states, 1:1 clock) are the base unit of system operation and many components are clocked directly on T state
    8-bit general purpose registers
        A, B, C, D, E, H, L
    8-bit flags register (F)
        Bits 0–3 are grounded to 0
        Bit 4: C (carry flag)
        Bit 5: H (half-carry flag)
        Bit 6: N (negative flag)
        Bit 7: Z (zero flag)
    16-bit general purpose register views
        AF, BC, DE, HL
        AF does not allow writing F bits 0–3
    16-bit special purpose registers
        PC (program counter)
        SP (stack pointer)
*/
    class SHARP_SM83_CPU : public Device
    {
    public:


        SHARP_SM83_CPU(BUS* bus) { BaseInit(bus); }

        //initializes the cpu
        void Init() override;


        void Clock();
        //cpu helpers

        CPU_INFO::CPU_REGISTER Registers;

        uint64_t Cycle;
        uint64_t TotalCycles;
        bool HALTED;
        bool STOPED;
        //this is due to ime being delayed by one instruction
        bool IME_Buffer;
   
        CPU_INFO::Instruction LoadedInstruction;

    private:
        std::vector<std::function<void()>> FunctionDefs;
   
    


        uint8_t ReadBYTEPC();
        uint16_t ReadShortPC();

        uint8_t* Get_8BIT_Register(CPU_INFO::ReferenceName ref);
        uint16_t* Get_16BIT_Register(CPU_INFO::ReferenceName ref);

        bool GetFlag(CPU_INFO::ReferenceName ref);
        void SetZeroFlag(uint8_t a);
        void SetFlagsDirect(uint8_t a);
        void SetFlags8bit_MATH(uint8_t a, uint16_t b);





       

        void ld_r_r();
        void ld_r_n();
        void ld_r_$HL$();
        void ld_$HL$_r();
        void ld_$HL$_n();
        void ld_A_$BC$();
        void ld_A_$DE$();
        void ld_A_$nn$();
        void ld_$BC$_A();
        void ld_$DE$_A();
        void ld_$nn$_A();
        void ld_A_$FF00_n$();
        void ld_$FF00_n$_A();
        void ld_A_$FF00_C$();
        void ld_$FF00_C$_A();
        void ldi_$HL$_A();
        void ldi_A_$HL$();
        void ldd_$HL$_A();
        void ldd_A_$HL$();
        void ld_rr_nn();
        void ld_$nn$_SP();
        void ld_SP_HL();
        void push_rr();
        void pop_rr();
        void add_A_r();
        void add_A_n();
        void add_A_$HL$();
        void adc_A_r();
        void adc_A_n();
        void adc_A_$HL$();
        void sub_r();
        void sub_n();
        void sub_$HL$();
        void sbc_A_r();
        void sbc_A_n();
        void sbc_A_$HL$();
        void and_r();
        void and_n();
        void and_$HL$();
        void xor_r();
        void xor_n();
        void xor_$HL$();
        void or_r();
        void or_n();
        void or_$HL$();
        void cp_r();
        void cp_n();
        void cp_$HL$();
        void inc_r();
        void inc_$HL$();
        void dec_r();
        void dec_$HL$();
        void daa();
        void cpl();
        void add_HL_rr();
        void inc_rr();
        void dec_rr();
        void add_SP_dd();
        void ld_HL_SP_dd();
        void rlca();
        void rla();
        void rrca();
        void rra();
        void rlc_r();
        void rlc_$HL$();
        void rl_r();
        void rl_$HL$();
        void rrc_r();
        void rrc_$HL$();
        void rr_r();
        void rr_$HL$();
        void sla_r();
        void sla_$HL$();
        void swap_r();
        void swap_$HL$();
        void sra_r();
        void sra_$HL$();
        void srl_r();
        void srl_$HL$();
        void bit_n_r();
        void bit_n_$HL$();
        void set_n_r();
        void set_n_$HL$();
        void res_n_r();
        void res_n_$HL$();
        void ccf();
        void scf();
        void nop();
        void halt();
        void stop();
        void di();
        void ei();
        void jp_nn();
        void jp_HL();
        void jp_f_nn();
        void jr_PC_dd();
        void jr_f_PC_dd();
        void call_nn();
        void call_f_nn();
        void ret();
        void ret_f();
        void reti();
        void rst_n();
        void ILLEGAL();




    };


}
