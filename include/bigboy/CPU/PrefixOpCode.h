#ifndef BIGBOY_PREFIXOPCODE_H
#define BIGBOY_PREFIXOPCODE_H

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

#endif //BIGBOY_PREFIXOPCODE_H