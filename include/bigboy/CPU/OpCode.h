#ifndef BIGBOY_OPCODE_H
#define BIGBOY_OPCODE_H

#include <string>

enum class OpCode : uint8_t {
    // LD r, r'
    // The contents of the register r' are loaded to the register r.
    // Registers r and r' may be any combination of B, C, D, E, H, L or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
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
    // Clock cycles: 8
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
    // Clock cycles: 8
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
    // Clock cycles: 12
    // Bit-by-bit: 0 0 1 1 0 1 1 0 <n n n n n n n n>
    LD_HL_n = 0b00110110,

    // LD A, (BC)
    // The byte at the memory address specified in the register pair BC
    // is loaded in to the register A.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 0 0 1 0 1 0
    LD_A_BC = 0b00001010,

    // LD A, (DE)
    // The byte at the memory address specified in the register pair DE
    // is loaded into the register A.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 0 1 1 0 1 0
    LD_A_DE = 0b00011010,

    // LD A, (nn)
    // The byte at the memory address specified by the 16-bit short nn
    // is loaded into the register A.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 1 1 1 0 1 0
    LD_A_nn = 0b11111010,

    // LD (BC), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified in the register pair BC.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 0 0 0 0 1 0
    LD_BC_A = 0b00000010,

    // LD (DE), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified in the register pair BC.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 0 1 0 0 1 0
    LD_DE_A = 0b00010010,

    // LD (nn), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified by the 16-bit short nn.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 1 0 1 0 1 0
    LD_nn_A = 0b11101010,

    // LD A, (FF00+n)
    // The byte at the memory address (FF00+n) - the nth I/O port - is
    // loaded into the register A.
    // Clock cycles: 12
    // Bit-by-bit: 1 1 1 1 0 0 0 0 <n n n n n n n n>
    LD_A_FF00n = 0b11110000,

    // LD (FF00+n), A
    // The contents of the register A are loaded into the byte at the
    // memory address (FF00+n) - the nth I/O port.
    // Clock cycles: 12
    // Bit-by-bit: 1 1 1 0 0 0 0 0 <n n n n n n n n>
    LD_FF00n_A = 0b11100000,

    // LD A, (FF00+C)
    // The byte at the memory address (FF00+C) - the Cth I/O port - is
    // loaded into the register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 1 0 0 1 0
    LD_A_FF00C = 0b11110010,

    // LD (FF00+C), A
    // The contents of the register A are loaded into the byte at the
    // memory address (FF00+C) - the Cth I/O port.
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 0 0 0 1 0
    LD_FF00C_A = 0b11100010,

    // LDI (HL), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified by the register pair HL. HL is then
    // incremented by 1.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 1 0 0 0 1 0
    LDI_HL_A = 0b00100010,

    // LDI A, (HL)
    // The byte at the memory address specified by the register pair HL
    // is loaded into the register A. HL is then incremented by 1.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 1 0 1 0 1 0
    LDI_A_HL = 0b00101010,

    // LDD (HL), A
    // The contents of the register A are loaded into the byte at the
    // memory address specified by the register pair HL. HL is then
    // decremented by 1.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 1 1 0 0 1 0
    LDD_HL_A = 0b00110010,

    // LDD A, (HL)
    // The byte at the memory address specified by the register pair HL
    // is loaded into the register A. HL is then decremented by 1.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 1 1 1 0 1 0
    LDD_A_HL = 0b00111010,

    // LD dd, nn
    // The 16-bit short nn is read as an integer and loaded into the
    // register pair dd. Register pair dd may be any of BC, DE, HL or SP.
    // Clock cycles: 12
    // Bit-by-bit: 0 0 <d d> 0 0 0 1 <n n n n n n n n> <n n n n n n n n>
    LD_BC_nn = 0b00000001,
    LD_DE_nn = 0b00010001,
    LD_HL_nn = 0b00100001,
    LD_SP_nn = 0b00110001,

    // LD SP, HL
    // The contents of the register pair HL are loaded to the stack pointer
    // (SP).
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 1 1 0 0 1
    LD_SP_HL = 0b11111001,

    // PUSH qq
    // The contents of the register pair qq are pushed to the stack. First,
    // the stack pointer (SP) is decremented, and the high-order byte of qq
    // is loaded to the byte at the memory address specified by SP. Then,
    // SP is decremented again and the low-order byte of qq is loaded into
    // the byte at the memory address specified by SP. The register pair qq
    // may be any of BC, DE, HL or AF.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 <q q> 0 1 0 1
    PUSH_BC = 0b11000101,
    PUSH_DE = 0b11010101,
    PUSH_HL = 0b11100101,
    PUSH_AF = 0b11110101,

    // POP qq
    // The top two bytes of the stack are popped into the register pair qq.
    // First, the byte at the memory address specified by SP is loaded into
    // the low-order byte of qq. SP is then incremented, and the byte at the
    // memory address which it now specifies is loaded into the high-order
    // byte of qq, and SP is incremented again. The register pair qq may be
    // any of BC, DE, HL or AF.
    // Clock cycles: 12
    // Bit-by-bit: 1 1 <q q> 0 0 0 1
    POP_BC = 0b11000001,
    POP_DE = 0b11010001,
    POP_HL = 0b11100001,
    POP_AF = 0b11110001,

    // ADD A, r:
    // The contents of register r are added to the contents of register A
    // (the Accumulator) and the result is stored in register A. Register
    // r can be any of B, C, D, E, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 0 0 0 1 1 0 <n n n n n n n n>
    ADDA_n = 0b11000110,

    // ADD A, (HL):
    // The byte at the memory address specified in the virtual 16-bit
    // register HL is added to the contents of register A and the result
    // is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 0 0 0 1 1 0
    ADDA_HL = 0b10000110,

    // ADC A, r:
    // The contents of register r as well as the state of the carry flag
    // (0 or 1 for true or false respectively) are added to the contents
    // of register A and the result is stored in register A. Register r
    // may be any of B, C, D, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 0 0 1 1 1 0 <n n n n n n n n>
    ADCA_n = 0b11001110,

    // ADC A, (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL along with the value of the carry flag are added to the
    // register A and the result is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 0 0 1 1 1 0
    ADCA_HL = 0b10001110,

    // SUB r
    // The contents of the register r are subtracted from the contents of
    // register A, and the result is stored in register A. Register r may be
    // any of B, C, D, E, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 0 1 0 1 1 0 <n n n n n n n n>
    SUB_n = 0b11010110,

    // SUB (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL is subtracted from the register A and the result is
    // stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 0 1 0 1 1 0
    SUB_HL = 0b10010110,

    // SBC A, r
    // The contents of the register r along with the value of the carry
    // flag are both subtracted from the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 0 1 1 1 1 0 <n n n n n n n n>
    SBCA_n = 0b11011110,

    // SBC A, (HL)
    // The byte at the memory address specified in the virtual 16-bit
    // register HL and the value of the carry flag are both subtracted from
    // the register A, and the result is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 0 1 1 1 1 0
    SBCA_HL = 0b10011110,

    // AND r
    // A bitwise AND operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 0 0 1 1 0 <n n n n n n n n>
    AND_n = 0b11100110,

    // AND (HL)
    // A bitwise AND operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 1 0 0 1 1 0
    AND_HL = 0b10100110,

    // XOR r
    // A bitwise XOR operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 0 1 1 1 0 <n n n n n n n n>
    XOR_n = 0b11101110,

    // XOR (HL)
    // A bitwise XOR operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 1 0 1 1 1 0
    XOR_HL = 0b10101110,

    // OR r
    // A bitwise OR operation is performed between the contents of the
    // register r and the contents of the register A, and the result is
    // stored in register A. Register r may be any of B, C, D, E, H, L, or A.
    // Clock cycles: 4
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
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 1 0 1 1 0 <n n n n n n n n>
    OR_n = 0b11110110,

    // OR (HL)
    // A bitwise OR operation is performed between the byte at the memory
    // address specified in the virtual 16-bit register HL and the contents
    // of register A, and the result is stored in register A.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 1 1 0 1 1 0
    OR_HL = 0b10110110,

    // CP r
    // The contents of register R are compared with (subtracted from) the
    // register A, setting the appropriate flags but not storing the result.
    // Register r may be any of B, C, D, E, H, L or A.
    // Clock cycles: 4
    // Bit-by-bit: 1 0 1 1 1 <r r r>
    CP_B = 0b10111000,
    CP_C = 0b10111001,
    CP_D = 0b10111010,
    CP_E = 0b10111011,
    CP_H = 0b10111100,
    CP_L = 0b10111101,
    CP_A = 0b10111111,

    // CP n
    // The byte n is compared with (subtracted from) the register A, setting
    // the appropriate flags but not storing the result.
    // Clock cycles: 8
    // Bit-by-bit: 1 1 1 1 1 1 1 0 <b b b b b b b b>
    CP_n = 0b11111110,

    // CP (HL)
    // The byte at the memory address specified in the register HL is compared
    // with (subtracted from) the register A, setting the appropriate flags but
    // not storing the result.
    // Clock cycles: 8
    // Bit-by-bit: 1 0 1 1 1 1 1 0
    CP_HL = 0b10111110,

    // INC r
    // The register r is incremented by 1. Register r may be any of B, C, D, E,
    // H, L or A.
    // Clock cycles: 4
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
    // Clock cycles: 12
    // Bit-by-bit: 0 0 1 1 0 1 0 0
    INC_HL_ = 0b00110100,

    // DEC r
    // The register r is decremented by 1. Register r may be any of B, C, D, E,
    // H, L or A.
    // Clock cycles: 4
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
    // Clock cycles: 12
    // Bit-by-bit: 0 0 1 1 0 1 0 1
    DEC_HL_ = 0b00110101,

    // DAA
    // The results of the previous operation as stored in the Accumulator and flags
    // are retroactively adjusted to become a BCD (binary coded decimal) operation,
    // where the lower and upper nibbles of the bytes in the operation are treated as
    // two individual decimal digits, rather than the whole byte as one binary number.
    // It does this by adding or subtracting 6 from the Accumulator's lower nibble,
    // upper nibble or both, based on whether the last operation was a subtraction
    // (n flag), and whether a carry and/or half carry occurred (c and h flags).
    // Clock cycles: 4
    // Bit-by-bit = 0 0 1 0 0 1 1 1
    DAA = 0b00100111,

    // CPL
    // The contents of register A are inverted (one's complement).
    // Clock cycles: 4
    // Bit-by-bit: 0 0 1 0 1 1 1 1
    CPL = 0b00101111,

    // ADD HL, rr
    // The contents of the register pair rr are added to the contents of the register
    // pair HL and the result is stored in HL. Register pair rr may be any of BC, DE,
    // HL or SP.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 <r r> 1 0 0 1
    ADD_HL_BC = 0b00001001,
    ADD_HL_DE = 0b00011001,
    ADD_HL_HL = 0b00101001,
    ADD_HL_SP = 0b00111001,

    // INC rr
    // The register pair rr is incremented by 1. Register pair rr may be any of BC,
    // DE, HL or SP.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 <r r> 0 0 1 1
    INC_BC = 0b00000011,
    INC_DE = 0b00010011,
    INC_HL = 0b00100011,
    INC_SP = 0b00110011,

    // DEC rr
    // The register pair rr is decremented by 1. Register pair rr may be any of BC,
    // DE, HL or SP.
    // Clock cycles: 8
    // Bit-by-bit: 0 0 <r r> 1 0 1 1
    DEC_BC = 0b00001011,
    DEC_DE = 0b00011011,
    DEC_HL = 0b00101011,
    DEC_SP = 0b00111011,

    // ADD SP, s
    // The byte s is read as a signed integer and added to the register pair SP.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 1 0 1 0 0 0 <s s s s s s s s>
    ADD_SP_s = 0b11101000,

    // LD HL, SP+s
    // The byte s is read as a signed integer and added to the register pair SP. The
    // result is then loaded into the register pair HL.
    // Clock cycles: 12
    // Bit-by-bit: 1 1 1 1 1 0 0 0 <s s s s s s s s>
    LD_HL_SPs = 0b11111000,

    // RLCA
    // The contents of register A are rotated left by 1 bit position, after the sign
    // bit (7) is copied into the carry flag.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 0 0 0 1 1 1
    RLCA = 0b00000111,

    // RLA
    // The contents of register A are rotated left by 1 bit position through the
    // carry flag.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 0 1 0 1 1 1
    RLA = 0b00010111,

    // RRCA
    // The contents of register A are rotated right by 1 bit position, after bit 0
    // is copied into the carry flag.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 0 0 1 1 1 1
    RRCA = 0b00001111,

    // RRA
    // The contents of register A are rotated right by 1 bit position through the
    // carry flag.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 0 1 1 1 1 1
    RRA = 0b00011111,

    // CCF
    // The carry flag is inverted.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 1 1 1 1 1 1
    CCF = 0b00111111,

    // SCF
    // The carry flag is set.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 1 1 0 1 1 1
    SCF = 0b00110111,

    // NOP
    // The CPU performs no operation during this cycle.
    // Clock cycles: 4
    // Bit-by-bit: 0 0 0 0 0 0 0 0
    NOP = 0b00000000,

    // HALT
    // CPU operation is suspended until an interrupt or reset is recieved. While in
    // this halted state, NOPs are executed to maintain memory refresh logic.
    // Clock cycles: 4 (+4 for every following NOP)
    // Bit-by-bit: 0 1 1 1 0 1 1 0
    HALT = 0b01110110,

    // STOP
    // CPU operation is stopped.
    // Clock cycles: N/A
    // Bit-by-bit: 0 0 0 1 0 0 0 0
    STOP = 0b00010000,

    // DI
    // Interrupts are disabled by resetting the Interrupt Master Flag (IME).
    // Clock cycles: 4
    // Bit-by-bit: 1 1 1 1 0 0 1 1
    DI = 0b11110011,

    // EI
    // Interrupts are enabled by setting the Interrupt Master Flag (IME).
    // Clock cycles: 4
    // Bit-by-bit: 1 1 1 1 1 0 1 1
    EI = 0b11111011,

    // JP nn
    // The 16-bit word nn is loaded into the program counter, from where
    // execution continues.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 0 0 0 0 1 1 <n n n n n n n n> <n n n n n n n n>
    JP_nn = 0b11000011,

    // JP HL
    // The contents of the register pair HL are loaded into the program
    // counter, from where execution continues.
    // Clock cycles: 4
    // Bit-by-bit: 1 1 1 0 1 0 0 1
    JP_HL = 0b11101001,

    // JP f, nn
    // The 16-bit word nn is loaded into the program counter only if the
    // condition f is true. Execution will then continue from the program
    // counter. Condition f may be any of nz, z, nc or c.
    // Clock cycles: 16 if condition is met, otherwise 12
    // Bit-by-bit: 1 1 0 <f f f> 1 0 <n n n n n n n n> <n n n n n n n n>
    JP_NZ_nn = 0b11000010,
    JP_Z_nn = 0b11001010,
    JP_NC_nn = 0b11010010,
    JP_C_nn = 0b11011010,

    // JR PC+dd
    // The 8-bit signed integer dd is added to the program counter and the
    // result is stored in the program counter, from where execution
    // continues.
    // Clock cycles: 12
    // Bit-by-bit: 0 0 0 1 1 0 0 0 <dd dd dd dd dd dd dd dd>
    JR_PCdd = 0b00011000,

    // JR f, PC+dd
    // The 8-bit signed integer dd is added to the program counter and the
    // result is stored in the program counter only if the condition f is
    // true. Execution will then continue from the program counter.
    // Condition f may be any of nz, z, nc or c.
    // Clock cycles: 12 if condition is met, otherwise 8
    // Bit-by-bit: 0 0 1 <f f f> 0 0
    JR_NZ_PCdd = 0b00100000,
    JR_Z_PCdd = 0b00101000,
    JR_NC_PCdd = 0b00110000,
    JR_C_PCdd = 0b00111000,

    // CALL nn
    // The current program counter (return address) is pushed to the stack,
    // high-order byte first. The 16-bit word nn is then loaded into the
    // program counter, from where execution continues.
    // Clock cycles: 24
    // Bit-by-bit: 1 1 0 0 1 1 0 1
    CALL_nn = 0b11001101,

    // CALL f, nn
    // Only if the condition f is true is the current program counter (return
    // address) pushed to the stack, high-order byte first, and the 16-bit word
    // nn loaded into the program counter. Execution will them continue from
    // the program counter. Condition f may be any of nz, z, nc or c.
    // Clock cycles: 24 if condition is met, otherwise 12
    // Bit-by-bit: 1 1 0 <f f f> 0 0
    CALL_NZ_nn = 0b11000100,
    CALL_Z_nn = 0b11001100,
    CALL_NC_nn = 0b11010100,
    CALL_C_nn = 0b11011100,

    // RET
    // The 16-bit word on top of the stack is popped off, low-order byte first,
    // and loaded into the program counter, from where execution continues.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 0 0 1 0 0 1
    RET = 0b11001001,

    // RET f
    // Only if the condition f is true is the 16-bit word on top of the stack
    // popped off and loaded into the program counter. Execution will then
    // continue from the program counter. Condition f may be any of nz, z, nc or c.
    // Clock cycles: 20 if condition is met, otherwise 8
    // Bit-by-bit: 1 1 0 <f f f> 0 0
    RET_NZ = 0b11000000,
    RET_Z = 0b11001000,
    RET_NC = 0b11010000,
    RET_C = 0b11011000,

    // RETI
    // The 16-bit word on top of the stack is popped off, low-order byte first,
    // and loaded into the program counter. Interrupts are then enabled by setting
    // the interrupt master flag (IME), and execution then continues from the
    // program counter.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 0 1 1 0 0 1
    RETI = 0b11011001,

    // RST n
    // The current program counter is pushed onto the stack, high-order byte first.
    // The value of the operand n is then loaded into the program counter, from
    // where execution continues. Operand n may be any of 0x00, 0x08, 0x10, 0x18,
    // 0x20, 0x28, 0x30 or 0x38.
    // Clock cycles: 16
    // Bit-by-bit: 1 1 <n n n> 1 1 1
    RST_00 = 0b11000111,
    RST_08 = 0b11001111,
    RST_10 = 0b11010111,
    RST_18 = 0b11011111,
    RST_20 = 0b11100111,
    RST_28 = 0b11101111,
    RST_30 = 0b11110111,
    RST_38 = 0b11111111,

    // CB
    // Interpret the next byte as a prefix instruction (PrefixOpCode)
    // rather than a normal instruction (OpCode)
    CB = 0b11001011
};

std::string opCodeToString(OpCode opCode) {
    switch (opCode) {
        case OpCode::LD_B_B: return "LD B, B";
        case OpCode::LD_B_C: return "LD B, C";
        case OpCode::LD_B_D: return "LD B, D";
        case OpCode::LD_B_E: return "LD B, E";
        case OpCode::LD_B_H: return "LD B, H";
        case OpCode::LD_B_L: return "LD B, L";
        case OpCode::LD_B_A: return "LD B, A";
        case OpCode::LD_C_B: return "LD C, B";
        case OpCode::LD_C_C: return "LD C, C";
        case OpCode::LD_C_D: return "LD C, D";
        case OpCode::LD_C_E: return "LD C, E";
        case OpCode::LD_C_H: return "LD C, H";
        case OpCode::LD_C_L: return "LD C, L";
        case OpCode::LD_C_A: return "LD C, A";
        case OpCode::LD_D_B: return "LD D, B";
        case OpCode::LD_D_C: return "LD D, C";
        case OpCode::LD_D_D: return "LD D, D";
        case OpCode::LD_D_E: return "LD D, E";
        case OpCode::LD_D_H: return "LD D, H";
        case OpCode::LD_D_L: return "LD D, L";
        case OpCode::LD_D_A: return "LD D, A";
        case OpCode::LD_E_B: return "LD E, B";
        case OpCode::LD_E_C: return "LD E, C";
        case OpCode::LD_E_D: return "LD E, D";
        case OpCode::LD_E_E: return "LD E, E";
        case OpCode::LD_E_H: return "LD E, H";
        case OpCode::LD_E_L: return "LD E, L";
        case OpCode::LD_E_A: return "LD E, A";
        case OpCode::LD_H_B: return "LD H, B";
        case OpCode::LD_H_C: return "LD H, C";
        case OpCode::LD_H_D: return "LD H, D";
        case OpCode::LD_H_E: return "LD H, E";
        case OpCode::LD_H_H: return "LD H, H";
        case OpCode::LD_H_L: return "LD H, L";
        case OpCode::LD_H_A: return "LD H, A";
        case OpCode::LD_L_B: return "LD L, B";
        case OpCode::LD_L_C: return "LD L, C";
        case OpCode::LD_L_D: return "LD L, D";
        case OpCode::LD_L_E: return "LD L, E";
        case OpCode::LD_L_H: return "LD L, H";
        case OpCode::LD_L_L: return "LD L, L";
        case OpCode::LD_L_A: return "LD L, A";
        case OpCode::LD_A_B: return "LD A, B";
        case OpCode::LD_A_C: return "LD A, C";
        case OpCode::LD_A_D: return "LD A, D";
        case OpCode::LD_A_E: return "LD A, E";
        case OpCode::LD_A_H: return "LD A, H";
        case OpCode::LD_A_L: return "LD A, L";
        case OpCode::LD_A_A: return "LD A, A";
        case OpCode::LD_B_n: return "LD B, n";
        case OpCode::LD_C_n: return "LD C, n";
        case OpCode::LD_D_n: return "LD D, n";
        case OpCode::LD_E_n: return "LD E, n";
        case OpCode::LD_H_n: return "LD H, n";
        case OpCode::LD_L_n: return "LD L, n";
        case OpCode::LD_A_n: return "LD A, n";
        case OpCode::LD_B_HL: return "LD B, (HL)";
        case OpCode::LD_C_HL: return "LD C, (HL)";
        case OpCode::LD_D_HL: return "LD D, (HL)";
        case OpCode::LD_E_HL: return "LD E, (HL)";
        case OpCode::LD_H_HL: return "LD H, (HL)";
        case OpCode::LD_L_HL: return "LD L, (HL)";
        case OpCode::LD_A_HL: return "LD A, (HL)";
        case OpCode::LD_HL_B: return "LD (HL), B";
        case OpCode::LD_HL_C: return "LD (HL), C";
        case OpCode::LD_HL_D: return "LD (HL), D";
        case OpCode::LD_HL_E: return "LD (HL), E";
        case OpCode::LD_HL_H: return "LD (HL), H";
        case OpCode::LD_HL_L: return "LD (HL), L";
        case OpCode::LD_HL_A: return "LD (HL), A";
        case OpCode::LD_HL_n: return "LD (HL), n";
        case OpCode::LD_A_BC: return "LD A, (BC)";
        case OpCode::LD_A_DE: return "LD A, (DE)";
        case OpCode::LD_A_nn: return "LD A, (nn)";
        case OpCode::LD_BC_A: return "LD (BC), A";
        case OpCode::LD_DE_A: return "LD (DE), A";
        case OpCode::LD_nn_A: return "LD (nn), A";
        case OpCode::LD_A_FF00n: return "LD A, (FF00+n)";
        case OpCode::LD_FF00n_A: return "LD (FF00+n), A";
        case OpCode::LD_A_FF00C: return "LD A, (FF00+C)";
        case OpCode::LD_FF00C_A: return "LD (FF00+C), A";
        case OpCode::LDI_HL_A: return "LDI HL, A";
        case OpCode::LDI_A_HL: return "LDI A, HL";
        case OpCode::LDD_HL_A: return "LDD HL, A";
        case OpCode::LDD_A_HL: return "LDD A, HL";
        case OpCode::LD_BC_nn: return "LD BC, nn";
        case OpCode::LD_DE_nn: return "LD DE, nn";
        case OpCode::LD_HL_nn: return "LD HL, nn";
        case OpCode::LD_SP_nn: return "LD SP, nn";
        case OpCode::LD_SP_HL: return "LD SP, HL";
        case OpCode::PUSH_BC: return "PUSH BC";
        case OpCode::PUSH_DE: return "PUSH DE";
        case OpCode::PUSH_HL: return "PUSH HL";
        case OpCode::PUSH_AF: return "PUSH AF";
        case OpCode::POP_BC: return "POP BC";
        case OpCode::POP_DE: return "POP DE";
        case OpCode::POP_HL: return "POP HL";
        case OpCode::POP_AF: return "POP AF";
        case OpCode::ADDA_B: return "ADDA B";
        case OpCode::ADDA_C: return "ADDA C";
        case OpCode::ADDA_D: return "ADDA D";
        case OpCode::ADDA_E: return "ADDA E";
        case OpCode::ADDA_H: return "ADDA H";
        case OpCode::ADDA_L: return "ADDA L";
        case OpCode::ADDA_A: return "ADDA A";
        case OpCode::ADDA_n: return "ADDA n";
        case OpCode::ADDA_HL: return "ADDA (HL)";
        case OpCode::ADCA_B: return "ADCA B";
        case OpCode::ADCA_C: return "ADCA C";
        case OpCode::ADCA_D: return "ADCA D";
        case OpCode::ADCA_E: return "ADCA E";
        case OpCode::ADCA_H: return "ADCA H";
        case OpCode::ADCA_L: return "ADCA L";
        case OpCode::ADCA_A: return "ADCA A";
        case OpCode::ADCA_n: return "ADCA n";
        case OpCode::ADCA_HL: return "ADCA (HL)";
        case OpCode::SUB_B: return "SUB B";
        case OpCode::SUB_C: return "SUB C";
        case OpCode::SUB_D: return "SUB D";
        case OpCode::SUB_E: return "SUB E";
        case OpCode::SUB_H: return "SUB H";
        case OpCode::SUB_L: return "SUB L";
        case OpCode::SUB_A: return "SUB A";
        case OpCode::SUB_n: return "SUB n";
        case OpCode::SUB_HL: return "SUB (HL)";
        case OpCode::SBCA_B: return "SBCA B";
        case OpCode::SBCA_C: return "SBCA C";
        case OpCode::SBCA_D: return "SBCA D";
        case OpCode::SBCA_E: return "SBCA E";
        case OpCode::SBCA_H: return "SBCA H";
        case OpCode::SBCA_L: return "SBCA L";
        case OpCode::SBCA_A: return "SBCA A";
        case OpCode::SBCA_n: return "SBCA n";
        case OpCode::SBCA_HL: return "SBCA (HL)";
        case OpCode::AND_B: return "AND B";
        case OpCode::AND_C: return "AND C";
        case OpCode::AND_D: return "AND D";
        case OpCode::AND_E: return "AND E";
        case OpCode::AND_H: return "AND H";
        case OpCode::AND_L: return "AND L";
        case OpCode::AND_A: return "AND A";
        case OpCode::AND_n: return "AND n";
        case OpCode::AND_HL: return "AND (HL)";
        case OpCode::XOR_B: return "XOR B";
        case OpCode::XOR_C: return "XOR C";
        case OpCode::XOR_D: return "XOR D";
        case OpCode::XOR_E: return "XOR E";
        case OpCode::XOR_H: return "XOR H";
        case OpCode::XOR_L: return "XOR L";
        case OpCode::XOR_A: return "XOR A";
        case OpCode::XOR_n: return "XOR n";
        case OpCode::XOR_HL: return "XOR (HL)";
        case OpCode::OR_B: return "OR B";
        case OpCode::OR_C: return "OR C";
        case OpCode::OR_D: return "OR D";
        case OpCode::OR_E: return "OR E";
        case OpCode::OR_H: return "OR H";
        case OpCode::OR_L: return "OR L";
        case OpCode::OR_A: return "OR A";
        case OpCode::OR_n: return "OR n";
        case OpCode::OR_HL: return "OR (HL)";
        case OpCode::CP_B: return "CP B";
        case OpCode::CP_C: return "CP C";
        case OpCode::CP_D: return "CP D";
        case OpCode::CP_E: return "CP E";
        case OpCode::CP_H: return "CP H";
        case OpCode::CP_L: return "CP L";
        case OpCode::CP_A: return "CP A";
        case OpCode::CP_n: return "CP n";
        case OpCode::CP_HL: return "CP (HL)";
        case OpCode::INC_B: return "INC B";
        case OpCode::INC_C: return "INC C";
        case OpCode::INC_D: return "INC D";
        case OpCode::INC_E: return "INC E";
        case OpCode::INC_H: return "INC H";
        case OpCode::INC_L: return "INC L";
        case OpCode::INC_A: return "INC A";
        case OpCode::INC_HL_: return "INC (HL)";
        case OpCode::DEC_B: return "DEC B";
        case OpCode::DEC_C: return "DEC C";
        case OpCode::DEC_D: return "DEC D";
        case OpCode::DEC_E: return "DEC E";
        case OpCode::DEC_H: return "DEC H";
        case OpCode::DEC_L: return "DEC L";
        case OpCode::DEC_A: return "DEC A";
        case OpCode::DEC_HL_: return "DEC (HL)";
        case OpCode::DAA: return "DAA";
        case OpCode::CPL: return "CPL";
        case OpCode::ADD_HL_BC: return "ADD HL, BC";
        case OpCode::ADD_HL_DE: return "ADD HL, DE";
        case OpCode::ADD_HL_HL: return "ADD HL, HL";
        case OpCode::ADD_HL_SP: return "ADD HL, SP";
        case OpCode::INC_BC: return "INC BC";
        case OpCode::INC_DE: return "INC DE";
        case OpCode::INC_HL: return "INC HL";
        case OpCode::INC_SP: return "INC SP";
        case OpCode::DEC_BC: return "DEC BC";
        case OpCode::DEC_DE: return "DEC DE";
        case OpCode::DEC_HL: return "DEC HL";
        case OpCode::DEC_SP: return "DEC SP";
        case OpCode::ADD_SP_s: return "ADD SP, dd";
        case OpCode::LD_HL_SPs: return "LD HL, SP+dd";
        case OpCode::RLCA: return "RLCA";
        case OpCode::RLA: return "RLA";
        case OpCode::RRCA: return "RRCA";
        case OpCode::RRA: return "RRA";
        case OpCode::CCF: return "CCF";
        case OpCode::SCF: return "SCF";
        case OpCode::NOP: return "NOP";
        case OpCode::HALT: return "HALT";
        case OpCode::STOP: return "STOP";
        case OpCode::DI: return "DI";
        case OpCode::EI: return "EI";
        case OpCode::JP_nn: return "JP nn";
        case OpCode::JP_HL: return "JP HL";
        case OpCode::JP_NZ_nn: return "JP NZ, nn";
        case OpCode::JP_Z_nn: return "JP Z, nn";
        case OpCode::JP_NC_nn: return "JP NC, nn";
        case OpCode::JP_C_nn: return "JP C, nn";
        case OpCode::JR_PCdd: return "JR PC+dd";
        case OpCode::JR_NZ_PCdd: return "JR NZ, PC+dd";
        case OpCode::JR_Z_PCdd: return "JR Z, PC+dd";
        case OpCode::JR_NC_PCdd: return "JR NC, PC+dd";
        case OpCode::JR_C_PCdd: return "JR C, PC+dd";
        case OpCode::CALL_nn: return "CALL nn";
        case OpCode::CALL_NZ_nn: return "CALL NZ, nn";
        case OpCode::CALL_Z_nn: return "CALL Z, nn";
        case OpCode::CALL_NC_nn: return "CALL NC, nn";
        case OpCode::CALL_C_nn: return "CALL C, nn";
        case OpCode::RET: return "RET";
        case OpCode::RET_NZ: return "RET NZ";
        case OpCode::RET_Z: return "RET Z";
        case OpCode::RET_NC: return "RET NC";
        case OpCode::RET_C: return "RET C";
        case OpCode::RETI: return "RETI";
        case OpCode::RST_00: return "RST 00";
        case OpCode::RST_08: return "RST 08";
        case OpCode::RST_10: return "RST 10";
        case OpCode::RST_18: return "RST 18";
        case OpCode::RST_20: return "RST 20";
        case OpCode::RST_28: return "RST 28";
        case OpCode::RST_30: return "RST 30";
        case OpCode::RST_38: return "RST 38";
        case OpCode::CB: return "CB";
        default: return "!unreachable!";
    }
}

#endif //BIGBOY_OPCODE_H