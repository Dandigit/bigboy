#ifndef BIGBOY_OPCODE_H
#define BIGBOY_OPCODE_H

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

    // LD SP, HL
    // The contents of the register pair HL are loaded to the stack pointer
    // (SP).
    // Bit-by-bit: 1 1 1 1 1 0 0 1
    LD_SP_HL = 0b11111001,

    // PUSH qq
    // The contents of the register pair qq are pushed to the stack. First,
    // the stack pointer (SP) is decremented, and the high-order byte of qq
    // is loaded to the byte at the memory address specified by SP. Then,
    // SP is decremented again and the low-order byte of qq is loaded into
    // the byte at the memory address specified by SP. The register pair qq
    // may be any of BC, DE, HL or AF.
    // Bit-by-bit: 1 1 <q q> 0 1 0 1
    PUSH_BC = 0b11000101,
    PUSH_DE = 0b11010101,
    PUSH_HL = 0b11100101,
    PUSH_AF = 0b11110101,

    // POP qq

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

#endif //BIGBOY_OPCODE_H