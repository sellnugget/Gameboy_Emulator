#pragma once
#include <string>
#include <vector>
namespace gb {

    struct CPU_INFO {
        enum ReferenceName {

            REF_A,
            REF_C,
            REF_B,
            REF_D,
            REF_E,
            REF_H,
            REF_L,
            REF_$HL,
            REF_0,
            REF_1,
            REF_2,
            REF_3,
            REF_4,
            REF_5,
            REF_6,
            REF_7,
            REF_n8,
            REF_a16,
            REF_HL,
            REF_SP,
            REF_BC,
            REF_DE,
            REF_AF,
            REF_e8,
            REF_n16,
            REF_NZ,
            REF_Z,
            REF_NC,
            REF_$a16,
            REF_$BC,
            REF_$DE,
            REF_$a8,
            REF_$C,
            REF_$00,
            REF_$08,
            REF_$10,
            REF_$18,
            REF_$20,
            REF_$28,
            REF_$30,
            REF_$38,

        };
        enum InstructionType {
            INST_ld_r_r,
            INST_ld_r_n,
            INST_ld_r_$HL$,
            INST_ld_$HL$_r,
            INST_ld_$HL$_n,
            INST_ld_A_$BC$,
            INST_ld_A_$DE$,
            INST_ld_A_$nn$,
            INST_ld_$BC$_A,
            INST_ld_$DE$_A,
            INST_ld_$nn$_A,
            INST_ld_A_$FF00_n$,
            INST_ld_$FF00_n$_A,
            INST_ld_A_$FF00_C$,
            INST_ld_$FF00_C$_A,
            INST_ldi_$HL$_A,
            INST_ldi_A_$HL$,
            INST_ldd_$HL$_A,
            INST_ldd_A_$HL$,
            INST_ld_rr_nn,
            INST_ld_$nn$_SP,
            INST_ld_SP_HL,
            INST_push_rr,
            INST_pop_rr,
            INST_add_A_r,
            INST_add_A_n,
            INST_add_A_$HL$,
            INST_adc_A_r,
            INST_adc_A_n,
            INST_adc_A_$HL$,
            INST_sub_r,
            INST_sub_n,
            INST_sub_$HL$,
            INST_sbc_A_r,
            INST_sbc_A_n,
            INST_sbc_A_$HL$,
            INST_and_r,
            INST_and_n,
            INST_and_$HL$,
            INST_xor_r,
            INST_xor_n,
            INST_xor_$HL$,
            INST_or_r,
            INST_or_n,
            INST_or_$HL$,
            INST_cp_r,
            INST_cp_n,
            INST_cp_$HL$,
            INST_inc_r,
            INST_inc_$HL$,
            INST_dec_r,
            INST_dec_$HL$,
            INST_daa,
            INST_cpl,
            INST_add_HL_rr,
            INST_inc_rr,
            INST_dec_rr,
            INST_add_SP_dd,
            INST_ld_HL_SP_dd,
            INST_rlca,
            INST_rla,
            INST_rrca,
            INST_rra,
            INST_rlc_r,
            INST_rlc_$HL$,
            INST_rl_r,
            INST_rl_$HL$,
            INST_rrc_r,
            INST_rrc_$HL$,
            INST_rr_r,
            INST_rr_$HL$,
            INST_sla_r,
            INST_sla_$HL$,
            INST_swap_r,
            INST_swap_$HL$,
            INST_sra_r,
            INST_sra_$HL$,
            INST_srl_r,
            INST_srl_$HL$,
            INST_bit_n_r,
            INST_bit_n_$HL$,
            INST_set_n_r,
            INST_set_n_$HL$,
            INST_res_n_r,
            INST_res_n_$HL$,
            INST_ccf,
            INST_scf,
            INST_nop,
            INST_halt,
            INST_stop,
            INST_di,
            INST_ei,
            INST_jp_nn,
            INST_jp_HL,
            INST_jp_f_nn,
            INST_jr_PC_dd,
            INST_jr_f_PC_dd,
            INST_call_nn,
            INST_call_f_nn,
            INST_ret,
            INST_ret_f,
            INST_reti,
            INST_rst_n,
            INST_ILLEGAL,
            INST_PREFIX,

        };
        enum FlagState {
            FLAG_NULL, FLAG_CONDITION, FLAG_SET_0, FLAG_SET_1
        };

        struct CPU_REGISTER {
            //bits 0-3 of f in 16 bit mode can't be written to
            union {
                struct {



                    union {
                        struct {
                            uint8_t Ground : 4;
                            uint8_t Carry : 1;
                            uint8_t Half_Carry : 1;
                            uint8_t Subtract : 1;
                            uint8_t zero : 1;
                        };
                        uint8_t F;
                    };
                    uint8_t A;

                };

                uint16_t AF;
            };
            union {
                struct {
                    uint16_t C : 8;
                    uint16_t B : 8;
                };
                uint16_t BC;
            };
            union {
                struct {
                    uint16_t E : 8;
                    uint16_t D : 8;
                };
                uint16_t DE;
            };
            union {
                struct {

                    uint16_t L : 8;
                    uint16_t H : 8;

                };
                uint16_t HL;
            };
            uint16_t PC;
            uint16_t SP;

            union {
                struct {
                    uint16_t D1 : 8;
                    uint16_t D2 : 8;
                };
                uint16_t A16;
            };
        };



        //0:null, 1:conditional, 2:set 0, 3:set 1
        struct FlagSwicher {
            FlagState Z;
            FlagState N;
            FlagState H;
            FlagState C;
        };

        struct Operand {
            ReferenceName name;
            bool Increment;
            bool immediate;
        };


        struct Instruction {
            std::string mnemonic;
            InstructionType instructionType;
            int Size;
            int Cycles;
            std::vector<Operand> Operands;
            bool Immediate;
            FlagSwicher flagstate;

        };

        static Instruction unprefixed[0x100];
        static Instruction cbprefixed[0x100];
    };
 
}

