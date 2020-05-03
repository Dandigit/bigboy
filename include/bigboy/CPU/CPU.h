//
// Created by Daniel Boulton on 30/12/19.
//

#ifndef BIGBOY_CPU_H
#define BIGBOY_CPU_H

#include <cstdint>
#include <array>

enum class OpCode : uint8_t {
    // LD r, r'
    // The contents of the register r' are loaded to the register r.
    // Registers r and r' may be any combination of B, C, D, E, H, L or A.
    // Bit-by-bit: 0 1 <r r r> <r' r' r'>
    LD_B_B = 0b01000000,
    LD_B_C = 0b01000001,
    LD_B_D = 0b01000010,
    LD_B_E = 0b01000011,
    LD_B_H = 0b01000100,
    LD_B_L = 0b01000101,
    LD_B_A = 0b01000111,

    LD_C_B = 0b01001000,
    LD_C_C = 0b01001001,
    LD_C_D = 0b01001010,
    LD_C_E = 0b01001011,
    LD_C_H = 0b01001100,
    LD_C_L = 0b01001101,
    LD_C_A = 0b01001111,

    LD_D_B = 0b01010000,
    LD_D_C = 0b01010001,
    LD_D_D = 0b01010010,
    LD_D_E = 0b01010011,
    LD_D_H = 0b01010100,
    LD_D_L = 0b01010101,
    LD_D_A = 0b01010111,

    LD_E_B = 0b01011000,
    LD_E_C = 0b01011001,
    LD_E_D = 0b01011010,
    LD_E_E = 0b01011011,
    LD_E_H = 0b01011100,
    LD_E_L = 0b01011101,
    LD_E_A = 0b01011111,

    LD_H_B = 0b01100000,
    LD_H_C = 0b01100001,
    LD_H_D = 0b01100010,
    LD_H_E = 0b01100011,
    LD_H_H = 0b01100100,
    LD_H_L = 0b01100101,
    LD_H_A = 0b01100111,

    LD_L_B = 0b01101000,
    LD_L_C = 0b01101001,
    LD_L_D = 0b01101010,
    LD_L_E = 0b01101011,
    LD_L_H = 0b01101100,
    LD_L_L = 0b01101101,
    LD_L_A = 0b01101111,

    LD_A_B = 0b01111000,
    LD_A_C = 0b01111001,
    LD_A_D = 0b01111010,
    LD_A_E = 0b01111011,
    LD_A_H = 0b01111100,
    LD_A_L = 0b01111101,
    LD_A_A = 0b01111111,

    // LD r, n
    // Byte n is read as an integer and loaded into register r. Register
    // r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit: 0 0 <r r r> 1 1 0 <n n n n n n n n>
    LD_B_n = 0b00000110,
    LD_C_n = 0b00001110,
    LD_D_n = 0b00010110,
    LD_E_n = 0b00011110,
    LD_H_n = 0b00100110,
    LD_L_n = 0b00101110,
    LD_A_n = 0b00111110,

    // LD r, (HL)
    // The byte at the memory address stored in the register pair HL is
    // loaded into register r. Register r may be any of B, C, D, E, H, L
    // or A.
    // Bit-by-bit: 0 1 <r r r> 1 1 0
    LD_B_HL = 0b01000110,
    LD_C_HL = 0b01001110,
    LD_D_HL = 0b01010110,
    LD_E_HL = 0b01011110,
    LD_H_HL = 0b01100110,
    LD_L_HL = 0b01101110,
    LD_A_HL = 0b01111110,

    // LD (HL), r
    // The contents of register r are loaded into the byte at the memory
    // address specified in the register pair HL. Register r may be any
    // of B, C, D, E, H, L or A.
    // Bit-by-bit: 0 1 1 1 0 <r r r>
    LD_HL_B = 0b01110000,
    LD_HL_C = 0b01110001,
    LD_HL_D = 0b01110010,
    LD_HL_E = 0b01110011,
    LD_HL_H = 0b01110100,
    LD_HL_L = 0b01110101,
    LD_HL_A = 0b01110111,

    // LD (HL), n
    // Byte n is read as an integer and loaded into the memory address
    // specified in the register pair HL.
    // Bit-by-bit: 0 0 1 1 0 1 1 0 <n n n n n n n n>
    LD_HL_n = 0b00110110,

    // LD A, (BC)
    // The byte at the memory address specified in the register pair BC
    // is loaded in to the register A.
    // Bit-by-bit: 0 0 0 0 1 0 1 0
    LD_A_BC = 0b00001010,

    // LD A, (DE)
    // The byte at the memory address specified in the register pair DE
    // is loaded into the register A.
    // Bit-by-bit: 0 0 0 1 1 0 1 0
    LD_A_DE = 0b00011010,

    // LD A, (nn)
    // The 16-bit short nn is read as an integer, specifying the memory
    // address of a byte which is loaded into the register A.
    // Bit-by-bit: 0 0 1 1 1 0 1 0 <n n n n n n n n> <n n n n n n n n>
    LD_A_nn = 0b00111010,

    // LD (BC), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified in the register pair BC.
    // Bit-by-bit: 0 0 0 0 0 0 1 0
    LD_BC_A = 0b00000010,

    // LD (DE), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified in the register pair BC.
    // Bit-by-bit: 0 0 0 1 0 0 1 0
    LD_DE_A = 0b00010010,

    // LD (nn), A
    // The 16 bit short nn is read as an integer, specifying the memory
    // address of a byte into which the register A is loaded.
    // Bit-by-bit: 0 0 1 1 0 0 1 0 <n n n n n n n n> <n n n n n n n n>
    LD_nn_A = 0b00110010,

    // LD dd, nn
    // The 16-bit short nn is read as an integer and loaded into the
    // register pair dd. Register pair dd may be any of BC, DE, HL or SP.
    // Bit-by-bit: 0 0 <d d> 0 0 0 1 <n n n n n n n n> <n n n n n n n n>
    LD_BC_nn = 0b00000001,
    LD_DE_nn = 0b00010001,
    LD_HL_nn = 0b00100001,
    LD_SP_nn = 0b00110001,

    // LD HL, (nn) [opcode renamed to LD_HL_mm]
    // The byte at the memory address (nn) is loaded into the low order byte
    // (l) of the register pair HL and the byte at the next highest memory
    // address (nn+1) is loaded into the high order portion (h).
    // Bit-by-bit: 0 0 1 0 1 0 1 0 <n n n n n n n n> <n n n n n n n n>
    LD_HL_mm = 0b00101010,

    // LD (nn), HL
    // The low order byte of the register pair HL (l) is loaded to the byte
    // at the memory address (nn), and the high order byte (h) is loaded to
    // the byte at the next highest memory address (nn+1).
    // Bit-by-bit: 0 0 1 0 0 0 1 0
    LD_nn_HL = 0b00100010,

    // ADD A, r:
    // The contents of register r are added to the contents of register A
    // (the Accumulator) and the result is stored in register A. Register
    // r can be any of B, C, D, E, H, L, or A.
    // Bit-by-bit: 1 0 0 0 0 <r r r>
    ADDA_B = 0b10000000,
    ADDA_C = 0b10000001,
    ADDA_D = 0b10000010,
    ADDA_E = 0b10000011,
    ADDA_H = 0b10000100,
    ADDA_L = 0b10000101,
    ADDA_A = 0b10000111,

    // ADD A, n:
    // Byte n is read as an integer and added to the contents of register
    // A, and the result is stored in register A.
    // Bit-by-bit: 1 1 0 0 0 1 1 0 <n n n n n n n n>
    ADDA_n = 0b11000110,

    // ADD A, (HL):
    // The byte at the memory address specified in the virtual 16-bit
    // register HL is added to the contents of register A and the result
    // is stored in register A.
    ADDA_HL = 0b10000110,

    // ADC A, r:
    // The contents of register r as well as the state of the carry flag
    // (0 or 1 for true or false respectively) are added to the contents
    // of register A and the result is stored in register A. Register r
    // may be any of B, C, D, H, L, or A.
    // Bit-by-bit: 1 0 0 0 1 <r r r>
    ADCA_B = 0b10001000,
    ADCA_C = 0b10001001,
    ADCA_D = 0b10001010,
    ADCA_E = 0b10001011,
    ADCA_H = 0b10001100,
    ADCA_L = 0b10001101,
    ADCA_A = 0b10001111,

    // ADC A, n
    // Byte n is read as an integer and added to the contents of register
    // A along with the value of the carry flag. The result is then stored
    // in register A.
    // Bit-by-bit: 1 1 0 0 1 1 1 0 <n n n n n n n n>
    ADCA_n = 0b11001110,

    // ADC A, (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL along with the value of the carry flag are added to the
    // register A and the result is stored in register A.
    ADCA_HL = 0b10001110,

    // SUB r
    // The contents of the register r are subtracted from the contents of
    // register A, and the result is stored in register A. Register r may be
    // any of B, C, D, E, H, L, or A.
    // Bit-by-bit: 1 0 0 1 0 <r r r>
    SUB_B = 0b10010000,
    SUB_C = 0b10010001,
    SUB_D = 0b10010010,
    SUB_E = 0b10010011,
    SUB_H = 0b10010100,
    SUB_L = 0b10010101,
    SUB_A = 0b10010111,

    // SUB n
    // Byte n is read as an integer and subtracted from the contents of
    // register A, and the result is stored in register A.
    // Bit-by-bit: 1 1 0 1 0 1 1 0 <n n n n n n n n>
    SUB_n = 0b11010110,

    // SUB (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL is subtracted from the register A and the result is
    // stored in register A.
    // Bit-by-bit: 1 0 0 1 0 1 1 0
    SUB_HL = 0b10010110,

    // SBC A, r
    // The contents of the register r along with the value of the carry
    // flag are both subtracted from the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit: 1 0 0 1 1 <r r r>
    SBCA_B = 0b10011000,
    SBCA_C = 0b10011001,
    SBCA_D = 0b10011010,
    SBCA_E = 0b10011011,
    SBCA_H = 0b10011100,
    SBCA_L = 0b10011101,
    SBCA_A = 0b10011111,

    // SBC A, n
    // Byte n is read as an integer and along with the value of the carry
    // flag, it is subtracted from register A, and the result is stored in
    // register A.
    // Bit-by-bit: 1 1 0 1 1 1 1 0 <n n n n n n n n>
    SBCA_n = 0b11011110,

    // SBC A, (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL and the value of the carry flag are both subtracted from
    // the register A, and the result is stored in register A.
    // Bit-by-bit: 1 0 0 1 1 1 1 0
    SBCA_HL = 0b10011110,

    // AND r
    // A bitwise AND operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Bit-by-bit: 1 0 1 0 0 <r r r>
    AND_B = 0b10100000,
    AND_C = 0b10100001,
    AND_D = 0b10100010,
    AND_E = 0b10100011,
    AND_H = 0b10100100,
    AND_L = 0b10100101,
    AND_A = 0b10100111,

    // AND n
    // A bitwise AND operation is performed between the byte n and the
    // contents of register A, and the result is stored in register A.
    // Bit-by-bit: 1 1 1 0 0 1 1 0 <n n n n n n n n>
    AND_n = 0b11100110,

    // AND (HL)
    // A bitwise AND operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Bit-by-bit: 1 0 1 0 0 1 1 0
    AND_HL = 0b10100110,

    // OR r
    // A bitwise OR operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Bit-by-bit: 1 0 1 1 0 <r r r>
    OR_B = 0b10110000,
    OR_C = 0b10110001,
    OR_D = 0b10110010,
    OR_E = 0b10110011,
    OR_H = 0b10110100,
    OR_L = 0b10110101,
    OR_A = 0b10110111,

    // OR n
    // A bitwise OR operation is performed between the byte n and the
    // contents of register A, and the result is stored in register A.
    // Bit-by-bit: 1 1 1 1 0 1 1 0 <n n n n n n n n>
    OR_n = 0b11110110,

    // OR (HL)
    // A bitwise OR operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Bit-by-bit: 1 0 1 1 0 1 1 0
    OR_HL = 0b10110110,

    // XOR r
    // A bitwise XOR operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Bit-by-bit: 1 0 1 0 1 <r r r>
    XOR_B = 0b10101000,
    XOR_C = 0b10101001,
    XOR_D = 0b10101010,
    XOR_E = 0b10101011,
    XOR_H = 0b10101100,
    XOR_L = 0b10101101,
    XOR_A = 0b10101111,

    // XOR n
    // A bitwise XOR operation is performed between the byte n and the
    // contents of register A, and the result is stored in register A.
    // Bit-by-bit: 1 1 1 0 1 1 1 0 <n n n n n n n n>
    XOR_n = 0b11101110,

    // XOR (HL)
    // A bitwise XOR operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Bit-by-bit: 1 0 1 0 1 1 1 0
    XOR_HL = 0b10101110,

    // CP r
    // The contents of register R are compared with (subtracted from) the
    // register A, setting the appropriate flags but not storing the result.
    // Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit: 1 0 1 1 1 <r r r>
    CP_B = 0b10111000,
    CP_D = 0b10111001,
    CP_C = 0b10111010,
    CP_E = 0b10111011,
    CP_H = 0b10111100,
    CP_L = 0b10111101,
    CP_A = 0b10111111,

    // CP n
    // The byte n is compared with (subtracted from) the register A, setting
    // the appropriate flags but not storing the result.
    // Bit-by-bit: 1 1 1 1 1 1 1 0 <b b b b b b b b>
    CP_n = 0b11111110,

    // CP (HL)
    // The byte at the memory address specified in the register HL is compared
    // with (subtracted from) the register A, setting the appropriate flags but
    // not storing the result.
    // Bit-by-bit: 1 0 1 1 1 1 1 0
    CP_HL = 0b10111110,

    // INC r
    // The register r is incremented by 1. Register r may be any of B, C, D, E,
    // H, L or A.
    // Bit-by-bit: 0 0 <r r r> 1 0 0
    INC_B = 0b00000100,
    INC_C = 0b00001100,
    INC_D = 0b00010100,
    INC_E = 0b00011100,
    INC_H = 0b00100100,
    INC_L = 0b00101100,
    INC_A = 0b00111100,

    // INC (HL)
    // The byte at the memory address specified in the register HL is incremented
    // by 1.
    // Bit-by-bit: 0 0 1 1 0 1 0 0
    INC_HL = 0b00110100,

    // DEC r
    // The register r is decremented by 1. Register r may be any of B, C, D, E,
    // H, L or A.
    // Bit-by-bit: 0 0 <r r r> 1 0 1
    DEC_B = 0b00000101,
    DEC_C = 0b00001101,
    DEC_D = 0b00010101,
    DEC_E = 0b00011101,
    DEC_H = 0b00100101,
    DEC_L = 0b00101101,
    DEC_A = 0b00111101,

    // DEC (HL)
    // The byte at the memory address specified in the register HL is decremented
    // by 1.
    // Bit-by-bit: 0 0 1 1 0 1 0 1
    DEC_HL = 0b00110101,

    // DAA
    // The results of the previous operation as stored in the Accumulator and flags
    // are retroactively adjusted to become a BCD (binary coded decimal) operation,
    // where the lower and upper nibbles of the bytes in the operation are treated as
    // two individual decimal digits, rather than the whole byte as one binary number.
    // It does this by adding or subtracting 6 from the Accumulator's lower nibble,
    // upper nibble or both, based on whether the last operation was a subtraction
    // (n flag), and whether a carry and/or half carry occurred (c and h flags).
    // Bit-by-bit = 0 0 1 0 0 1 1 1
    DAA = 0b00100111,

    // CPL
    // The contents of register A are inverted (one's complement).
    // Bit-by-bit: 0 0 1 0 1 1 1 1
    CPL = 0b00101111,

    // CCF
    // The carry flag is inverted.
    // Bit-by-bit: 0 0 1 1 1 1 1 1
    CCF = 0b00111111,

    // SCF
    // The carry flag is set.
    // Bit-by-bit: 0 0 1 1 0 1 1 1
    SCF = 0b00110111,

    // NOP
    // The CPU performs no operation during this cycle.
    // Bit-by-bit: 0 0 0 0 0 0 0 0
    NOP = 0b00000000,

    // TODO: HALT
    // CPU operation is suspended until an interrupt or reset is recieved. While in
    // this halted state, NOPs are executed to maintain memory refresh logic.
    // Bit-by-bit: 0 1 1 1 0 1 1 0
    HALT = 0b01110110,

    // TODO: STOP
    // CPU operation is stopped.
    // Bit-by-bit: 0 0 0 1 0 0 0 0
    STOP = 0b00010000,

    // TODO: DI

    // RLCA
    // The contents of register A are rotated left by 1 bit position, after the sign
    // bit (7) is copied into the carry flag.
    // Bit-by-bit: 0 0 0 0 0 1 1 1
    RLCA = 0b00000111,

    // RLA
    // The contents of register A are rotated left by 1 bit position through the
    // carry flag.
    // Bit-by-bit: 0 0 0 1 0 1 1 1
    RLA = 0b00010111,

    // RRCA
    // The contents of register A are rotated right by 1 bit position, after bit 0
    // is copied into the carry flag.
    // Bit-by-bit: 0 0 0 0 1 1 1 1
    RRCA = 0b00001111,

    // RRA
    // The contents of register A are rotated right by 1 bit position through the
    // carry flag.
    // Bit-by-bit: 0 0 0 1 1 1 1 1
    RRA = 0b00011111,

    // PREFIX:
    // Interpret the next byte as a prefix instruction (PrefixOpCode)
    // rather than a normal instruction (OpCode)
    PREFIX = 0b11001011
};

enum class PrefixOpCode {
    // RLC r
    // The contents of the register r are rotated left by 1 bit position, after the
    // sign bit (7) is copied into the carry flag. Register r may be any of B, C, D,
    // E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 0 0 0 <r r r>
    RLC_B = 0b00000000,
    RLC_C = 0b00000001,
    RLC_D = 0b00000010,
    RLC_E = 0b00000011,
    RLC_H = 0b00000100,
    RLC_L = 0b00000101,
    RLC_A = 0b00000111,

    // RLC (HL)
    // The byte at the memory address specified in the register pair HL is rotated
    // left by 1 bit position, after the sign bit (7) is copied into the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 0 0 0 1 1 0
    RLC_HL = 0b00000110,

    // RL r
    // The contents of the register r are rotated left by 1 bit position through the
    // carry flag. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 0 1 0 <r r r>
    RL_B = 0b00010000,
    RL_C = 0b00010001,
    RL_D = 0b00010010,
    RL_E = 0b00010011,
    RL_H = 0b00010100,
    RL_L = 0b00010101,
    RL_A = 0b00010111,

    // RL (HL)
    // The byte at the memory address specified in the register pair HL is rotated
    // left by 1 bit position through the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 0 1 0 1 1 0
    RL_HL = 0b00010110,

    // RRC r
    // The contents of the register r are rotated left by 1 bit position, after bit
    // 0 is copied into the carry flag. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 0 0 1 <r r r>
    RRC_B = 0b00001000,
    RRC_C = 0b00001001,
    RRC_D = 0b00001010,
    RRC_E = 0b00001011,
    RRC_H = 0b00001100,
    RRC_L = 0b00001101,
    RRC_A = 0b00001111,

    // RRC (HL)
    // The byte at the memory address specified in the register pair HL is rotated
    // right by 1 bit position, after bit 0 is copied into the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 0 0 1 1 1 0
    RRC_HL = 0b00001110,

    // RR r
    // The contents of the register r are rotated right by 1 bit position through the
    // carry flag. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 0 1 1 <r r r>
    RR_B = 0b00011000,
    RR_C = 0b00011001,
    RR_D = 0b00011010,
    RR_E = 0b00011011,
    RR_H = 0b00011100,
    RR_L = 0b00011101,
    RR_A = 0b00011111,

    // RR (HL)
    // The byte at the memory address specified in the register pair HL is rotated
    // right by 1 bit position through the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 0 1 1 1 1 0
    RR_HL = 0b00011110,

    // SLA r
    // The contents of the register r are shifted left by 1 bit position, after bit
    // 7 is copied to the carry flag. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 1 0 0 <r r r>
    SLA_B = 0b00100000,
    SLA_C = 0b00100001,
    SLA_D = 0b00100010,
    SLA_E = 0b00100011,
    SLA_H = 0b00100100,
    SLA_L = 0b00100101,
    SLA_A = 0b00100111,

    // SLA (HL)
    // The byte at the memory address specified in the register pair HL is shifted
    // left by 1 bit position, after bit 7 is copied to the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 1 0 0 1 1 0
    SLA_HL = 0b00100110,

    // SRA r
    // The lower 7 bits (0-6) of the register r are shifted right by 1 bit position,
    // after bit 0 is copied into the carry flag. Register r may be any of B, C, D,
    // E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 1 0 1 <r r r>
    SRA_B = 0b00101000,
    SRA_C = 0b00101001,
    SRA_D = 0b00101010,
    SRA_E = 0b00101011,
    SRA_H = 0b00101100,
    SRA_L = 0b00101101,
    SRA_A = 0b00101111,

    // SRA (HL)
    // The lower 7 bits (0-6) of the byte at the memory address specified in the
    // register pair HL are shifted right by 1 bi position, after bit 0 is copied
    // into the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 1 0 1 1 1 0
    SRA_HL = 0b00101110,

    // SRL r
    // The contents of the register r are shifted right by 1 bit position, after bit 0
    // is copied into the carry flag. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 0 1 1 1 <r r r>
    SRL_B = 0b00111000,
    SRL_C = 0b00111001,
    SRL_D = 0b00111010,
    SRL_E = 0b00111011,
    SRL_H = 0b00111100,
    SRL_L = 0b00111101,
    SRL_A = 0b00111111,

    // SRL (HL)
    // The byte at the memory address specified in the register pair HL is shifted right
    // by 1 bit position, after bit 0 is copied into the carry flag.
    // Bit-by-bit (after 0xCB): 0 0 1 1 1 1 1 0
    SRL_HL = 0b00111110,

    // BIT b, r
    // The zero flag is set if the bit at position b in the register r is 0, otherwise,
    // the flag is reset. Register r may be any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 1 <b b b> <r r r>
    BIT_0_B = 0b01000000,
    BIT_1_B = 0b01001000,
    BIT_2_B = 0b01010000,
    BIT_3_B = 0b01011000,
    BIT_4_B = 0b01100000,
    BIT_5_B = 0b01101000,
    BIT_6_B = 0b01110000,
    BIT_7_B = 0b01111000,

    BIT_0_C = 0b01000001,
    BIT_1_C = 0b01001001,
    BIT_2_C = 0b01010001,
    BIT_3_C = 0b01011001,
    BIT_4_C = 0b01100001,
    BIT_5_C = 0b01101001,
    BIT_6_C = 0b01110001,
    BIT_7_C = 0b01111001,

    BIT_0_D = 0b01000010,
    BIT_1_D = 0b01001010,
    BIT_2_D = 0b01010010,
    BIT_3_D = 0b01011010,
    BIT_4_D = 0b01100010,
    BIT_5_D = 0b01101010,
    BIT_6_D = 0b01110010,
    BIT_7_D = 0b01111010,

    BIT_0_E = 0b01000011,
    BIT_1_E = 0b01001011,
    BIT_2_E = 0b01010011,
    BIT_3_E = 0b01011011,
    BIT_4_E = 0b01100011,
    BIT_5_E = 0b01101011,
    BIT_6_E = 0b01110011,
    BIT_7_E = 0b01111011,

    BIT_0_H = 0b01000100,
    BIT_1_H = 0b01001100,
    BIT_2_H = 0b01010100,
    BIT_3_H = 0b01011100,
    BIT_4_H = 0b01100100,
    BIT_5_H = 0b01101100,
    BIT_6_H = 0b01110100,
    BIT_7_H = 0b01111100,

    BIT_0_L = 0b01000101,
    BIT_1_L = 0b01001101,
    BIT_2_L = 0b01010101,
    BIT_3_L = 0b01011101,
    BIT_4_L = 0b01100101,
    BIT_5_L = 0b01101101,
    BIT_6_L = 0b01110101,
    BIT_7_L = 0b01111101,

    BIT_0_A = 0b01000111,
    BIT_1_A = 0b01001111,
    BIT_2_A = 0b01010111,
    BIT_3_A = 0b01011111,
    BIT_4_A = 0b01100111,
    BIT_5_A = 0b01101111,
    BIT_6_A = 0b01110111,
    BIT_7_A = 0b01111111,

    // BIT b, (HL)
    // The zero flag is set if the bit at position b in the byte at the address stored
    // in the register pair HL is 0, otherwise, the flag is reset. Register r may be
    // any of B, C, D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 0 1 <b b b> 1 1 0
    BIT_0_HL = 0b01000110,
    BIT_1_HL = 0b01001110,
    BIT_2_HL = 0b01010110,
    BIT_3_HL = 0b01011110,
    BIT_4_HL = 0b01100110,
    BIT_5_HL = 0b01101110,
    BIT_6_HL = 0b01110110,
    BIT_7_HL = 0b01111110,

    // SET b, r
    // The bit at position b of the register r is set. Register r may be any of B, C, D,
    // E, H, L or A.
    // Bit-by-bit (after 0xCB): 1 1 <b b b> <r r r>
    SET_0_B = 0b11000000,
    SET_1_B = 0b11001000,
    SET_2_B = 0b11010000,
    SET_3_B = 0b11011000,
    SET_4_B = 0b11100000,
    SET_5_B = 0b11101000,
    SET_6_B = 0b11110000,
    SET_7_B = 0b11111000,

    SET_0_C = 0b11000001,
    SET_1_C = 0b11001001,
    SET_2_C = 0b11010001,
    SET_3_C = 0b11011001,
    SET_4_C = 0b11100001,
    SET_5_C = 0b11101001,
    SET_6_C = 0b11110001,
    SET_7_C = 0b11111001,

    SET_0_D = 0b11000010,
    SET_1_D = 0b11001010,
    SET_2_D = 0b11010010,
    SET_3_D = 0b11011010,
    SET_4_D = 0b11100010,
    SET_5_D = 0b11101010,
    SET_6_D = 0b11110010,
    SET_7_D = 0b11111010,

    SET_0_E = 0b11000011,
    SET_1_E = 0b11001011,
    SET_2_E = 0b11010011,
    SET_3_E = 0b11011011,
    SET_4_E = 0b11100011,
    SET_5_E = 0b11101011,
    SET_6_E = 0b11110011,
    SET_7_E = 0b11111011,

    SET_0_H = 0b11000100,
    SET_1_H = 0b11001100,
    SET_2_H = 0b11010100,
    SET_3_H = 0b11011100,
    SET_4_H = 0b11100100,
    SET_5_H = 0b11101100,
    SET_6_H = 0b11110100,
    SET_7_H = 0b11111100,

    SET_0_L = 0b11000101,
    SET_1_L = 0b11001101,
    SET_2_L = 0b11010101,
    SET_3_L = 0b11011101,
    SET_4_L = 0b11100101,
    SET_5_L = 0b11101101,
    SET_6_L = 0b11110101,
    SET_7_L = 0b11111101,

    SET_0_A = 0b11000111,
    SET_1_A = 0b11001111,
    SET_2_A = 0b11010111,
    SET_3_A = 0b11011111,
    SET_4_A = 0b11100111,
    SET_5_A = 0b11101111,
    SET_6_A = 0b11110111,
    SET_7_A = 0b11111111,

    // SET b, (HL)
    // The bit at position b of the byte at the memory address stored in the register
    // pair HL is set.
    // Bit-by-bit (after 0xCB): 1 1 <b b b> 1 1 0
    SET_0_HL = 0b11000110,
    SET_1_HL = 0b11001110,
    SET_2_HL = 0b11010110,
    SET_3_HL = 0b11011110,
    SET_4_HL = 0b11100110,
    SET_5_HL = 0b11101110,
    SET_6_HL = 0b11110110,
    SET_7_HL = 0b11111110,

    // RES b, r
    // The bit at position b of the register r is reset. Register r may be any of B, C,
    // D, E, H, L or A.
    // Bit-by-bit (after 0xCB): 1 1 <b b b> <r r r>
    RES_0_B = 0b10000000,
    RES_1_B = 0b10001000,
    RES_2_B = 0b10010000,
    RES_3_B = 0b10011000,
    RES_4_B = 0b10100000,
    RES_5_B = 0b10101000,
    RES_6_B = 0b10110000,
    RES_7_B = 0b10111000,

    RES_0_C = 0b10000001,
    RES_1_C = 0b10001001,
    RES_2_C = 0b10010001,
    RES_3_C = 0b10011001,
    RES_4_C = 0b10100001,
    RES_5_C = 0b10101001,
    RES_6_C = 0b10110001,
    RES_7_C = 0b10111001,

    RES_0_D = 0b10000010,
    RES_1_D = 0b10001010,
    RES_2_D = 0b10010010,
    RES_3_D = 0b10011010,
    RES_4_D = 0b10100010,
    RES_5_D = 0b10101010,
    RES_6_D = 0b10110010,
    RES_7_D = 0b10111010,

    RES_0_E = 0b10000011,
    RES_1_E = 0b10001011,
    RES_2_E = 0b10010011,
    RES_3_E = 0b10011011,
    RES_4_E = 0b10100011,
    RES_5_E = 0b10101011,
    RES_6_E = 0b10110011,
    RES_7_E = 0b10111011,

    RES_0_H = 0b10000100,
    RES_1_H = 0b10001100,
    RES_2_H = 0b10010100,
    RES_3_H = 0b10011100,
    RES_4_H = 0b10100100,
    RES_5_H = 0b10101100,
    RES_6_H = 0b10110100,
    RES_7_H = 0b10111100,

    RES_0_L = 0b10000101,
    RES_1_L = 0b10001101,
    RES_2_L = 0b10010101,
    RES_3_L = 0b10011101,
    RES_4_L = 0b10100101,
    RES_5_L = 0b10101101,
    RES_6_L = 0b10110101,
    RES_7_L = 0b10111101,

    RES_0_A = 0b10000111,
    RES_1_A = 0b10001111,
    RES_2_A = 0b10010111,
    RES_3_A = 0b10011111,
    RES_4_A = 0b10100111,
    RES_5_A = 0b10101111,
    RES_6_A = 0b10110111,
    RES_7_A = 0b10111111,

    // RES b, (HL)
    // The bit at position b of the byte at the memory address stored in the register
    // pair HL is reset.
    // Bit-by-bit (after 0xCB): 1 1 <b b b> 1 1 0
    RES_0_HL = 0b10000110,
    RES_1_HL = 0b10001110,
    RES_2_HL = 0b10010110,
    RES_3_HL = 0b10011110,
    RES_4_HL = 0b10100110,
    RES_5_HL = 0b10101110,
    RES_6_HL = 0b10110110,
    RES_7_HL = 0b10111110,
};

class CPU {
public:
    enum class RegisterOperand : uint8_t {
        B, // 000
        C, // 001
        D, // 010
        E, // 011
        H, // 100
        L, // 101
        A  // 111
    };

    enum class RegisterPairOperand : uint8_t {
        BC, // 00
        DE, // 01
        HL, // 10
        SP  // 11
    };

    enum class BitOperand : uint8_t {
        BIT0 = 0, // 000
        BIT1 = 1, // 001
        BIT2 = 2, // 010
        BIT3 = 3, // 011
        BIT4 = 4, // 100
        BIT5 = 5, // 101
        BIT6 = 6, // 110
        BIT7 = 7  // 111
    };

    struct Clock {
        int m;
        int t;
    };

    struct Registers {
        struct Flags {
            static constexpr uint8_t ZERO_FLAG_BYTE_POSITION = 7;
            static constexpr uint8_t SUBTRACT_FLAG_BYTE_POSITION = 6;
            static constexpr uint8_t HALF_CARRY_FLAG_BYTE_POSITION = 5;
            static constexpr uint8_t CARRY_FLAG_BYTE_POSITION = 4;

            bool zero = false;
            bool subtract = false;
            bool half_carry = false;
            bool carry = false;

            // We can treat a Flags object like a normal uint8_t
            Flags(uint8_t byte);

            operator uint8_t() const;
        };

        // Accumulator and flags
        uint8_t a = 0;
        Flags f = 0;

        // General purpose
        uint8_t c = 0;
        uint8_t b = 0;

        uint8_t e = 0;
        uint8_t d = 0;

        uint8_t l = 0;
        uint8_t h = 0;

        // Stack pointer
        uint16_t sp = 0;

        // TODO: Use this instead of switching over a RegisterOperand
        uint8_t& get(RegisterOperand target);
        uint16_t& get(RegisterPairOperand target);

        // Some instructions allow two 8 bit registers to be read as one 16 bit register
        // Referred to as BC (B & C), DE (D & E) and HL (H & L)
        uint16_t& BC();
        uint16_t BC() const;

        uint16_t& DE();
        uint16_t DE() const;

        uint16_t& HL();
        uint16_t HL() const;

        // TODO: Use the new interface above instead
        uint16_t getBC() const;
        void setBC(uint16_t value);

        uint16_t getDE() const;
        void setDE(uint16_t value);

        uint16_t getHL() const;
        void setHL(uint16_t value);
    };

    class MemoryBus {
        std::array<uint8_t, 0xFFFF> m_memory{0};

    public:
        MemoryBus() = default;

        explicit MemoryBus(std::array<uint8_t, 0xFFFF> memory);

        uint8_t& byteAt(uint16_t address);

        uint8_t readByte(uint16_t address);
        void writeByte(uint16_t address, uint8_t value);
    };

private:
    Clock m_clock;
    Registers m_registers;

    MemoryBus m_bus{};
    uint16_t m_pc{0};

    uint8_t m_halt;
    uint8_t m_stop;

    void load(uint8_t& target, uint8_t value);

    void LD_r_r(RegisterOperand target, RegisterOperand value);
    void LD_r_n(RegisterOperand target);
    void LD_r_HL(RegisterOperand target);

    void LD_HL_r(RegisterOperand value);
    void LD_HL_n();

    void LD_A_BC();
    void LD_A_DE();
    void LD_A_nn();

    void LD_BC_A();
    void LD_DE_A();
    void LD_nn_A();

    void load(uint16_t& target, uint16_t value);

    void LD_dd_nn(RegisterPairOperand target);

    void LD_HL_nn();

    void LD_nn_HL();

    void add(uint8_t value);

    void ADDA_r(RegisterOperand target);
    void ADDA_n();
    void ADDA_HL();

    void addWithCarry(uint8_t value);

    void ADCA_r(RegisterOperand target);
    void ADCA_n();
    void ADCA_HL();

    void subtract(uint8_t value);

    void SUB_r(RegisterOperand target);
    void SUB_n();
    void SUB_HL();

    void subtractWithCarry(uint8_t value);

    void SBCA_r(RegisterOperand target);
    void SBCA_n();
    void SBCA_HL();

    void bitwiseAnd(uint8_t value);

    void AND_r(RegisterOperand target);
    void AND_n();
    void AND_HL();

    void bitwiseOr(uint8_t value);

    void OR_r(RegisterOperand target);
    void OR_n();
    void OR_HL();

    void bitwiseXor(uint8_t value);

    void XOR_r(RegisterOperand target);
    void XOR_n();
    void XOR_HL();

    void compare(uint8_t value);

    void CP_r(RegisterOperand target);
    void CP_n();
    void CP_HL();

    void increment(uint8_t &target);

    void INC_r(RegisterOperand target);
    void INC_HL();

    void decrement(uint8_t &target);

    void DEC_r(RegisterOperand target);
    void DEC_HL();

    void DAA();
    void CPL();

    void CCF();
    void SCF();

    void NOP();
    void HALT();

    void rotateLeft(uint8_t &target);

    void RLCA();
    void RLC_r(RegisterOperand target);
    void RLC_HL();

    void rotateLeftThroughCarry(uint8_t &target);

    void RLA();
    void RL_r(RegisterOperand target);
    void RL_HL();

    void rotateRight(uint8_t &target);

    void RRCA();
    void RRC_r(RegisterOperand target);
    void RRC_HL();

    void rotateRightThroughCarry(uint8_t &target);

    void RRA();
    void RR_r(RegisterOperand target);
    void RR_HL();

    void shiftLeft(uint8_t &target);

    void SLA_r(RegisterOperand target);
    void SLA_HL();

    void shiftTailRight(uint8_t &target);

    void SRA_r(RegisterOperand target);
    void SRA_HL();

    void shiftRight(uint8_t &target);

    void SRL_r(RegisterOperand target);
    void SRL_HL();

    void testBit(BitOperand bit, uint8_t byte);

    void BIT_b_r(BitOperand bit, RegisterOperand reg);
    void BIT_b_HL(BitOperand bit);

    void setBit(BitOperand bit, uint8_t& target);

    void SET_b_r(BitOperand bit, RegisterOperand reg);
    void SET_b_HL(BitOperand bit);

    void resetBit(BitOperand bit, uint8_t& target);

    void RES_b_r(BitOperand bit, RegisterOperand reg);
    void RES_b_HL(BitOperand bit);

public:
    void load(const std::array<uint8_t, 0xFFFF> &memory);

    void exec();

    void step();
    void stepPrefix();

    void reset();

    Registers& registers();
    const Registers& registers() const;
};

#endif //BIGBOY_CPU_H
