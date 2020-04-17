//
// Created by Daniel Boulton on 30/12/19.
//

#ifndef BIGBOY_CPU_H
#define BIGBOY_CPU_H

#include <cstdint>
#include <array>

class CPU {
public:
    enum class OpCode : uint8_t {
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

        // ADD A, HL:
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

        // ADC A, HL
        // The byte at the memory address specified in the virtual 16-bit
        // register HL along with the value of the carry flag are added to the
        // register A and the result is stored in register A.
        ADCA_HL = 0b10001110,

        // SUB, r
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

        // SUB, n
        // Byte n is read as an integer and subtracted from the contents of
        // register A, and the result is stored in register A.
        // Bit-by-bit: 1 1 0 1 0 1 1 0 <n n n n n n n n>
        SUB_n = 0b11010110,

        // SUB, HL
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

        // SBC A, HL
        // The byte at the memory address specified in the virtual 16-bit
        // register HL and the value of the carry flag are both subtracted from
        // the register A, and the result is stored in register A.
        // Bit-by-bit: 1 0 0 1 1 1 1 0
        SBCA_HL = 0b10011110,

        // AND, r
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

        // AND, n
        // A bitwise AND operation is performed between the byte n and the
        // contents of register A, and the result is stored in register A.
        // Bit-by-bit: 1 1 1 0 0 1 1 0 <n n n n n n n n>
        AND_n = 0b11100110,

        // AND, HL
        // A bitwise AND operation is performed between the byte at the memory
        // address specified in the virtual 16-bit register HL and the contents
        // of register A, and the result is stored in register A.
        // Bit-by-bit: 1 0 1 0 0 1 1 0
        AND_HL = 0b10100110,

        // OR, r
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

        // OR, n
        // A bitwise OR operation is performed between the byte n and the
        // contents of register A, and the result is stored in register A.
        // Bit-by-bit: 1 1 1 1 0 1 1 0 <n n n n n n n n>
        OR_n = 0b11110110,

        // OR, HL
        // A bitwise OR operation is performed between the byte at the memory
        // address specified in the virtual 16-bit register HL and the contents
        // of register A, and the result is stored in register A.
        // Bit-by-bit: 1 0 1 1 0 1 1 0
        OR_HL = 0b10110110,

        // XOR, r
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

        // XOR, n
        // A bitwise XOR operation is performed between the byte n and the
        // contents of register A, and the result is stored in register A.
        // Bit-by-bit: 1 1 1 0 1 1 1 0 <n n n n n n n n>
        XOR_n = 0b11101110,

        // XOR, HL
        // A bitwise XOR operation is performed between the byte at the memory
        // address specified in the virtual 16-bit register HL and the contents
        // of register A, and the result is stored in register A.
        // Bit-by-bit: 1 0 1 0 1 1 1 0
        XOR_HL = 0b10101110,

        // CP, r
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

        // CP, n
        // The byte n is compared with (subtracted from) the register A, setting
        // the appropriate flags but not storing the result.
        // Bit-by-bit: 1 1 1 1 1 1 1 0 <b b b b b b b b>
        CP_n = 0b11111110,

        // CP, HL
        // The byte at the memory address specified in the register HL is compared
        // with (subtracted from) the register A, setting the appropriate flags but
        // not storing the result.
        // Bit-by-bit: 1 0 1 1 1 1 1 0
        CP_HL = 0b10111110,

        // INC, r
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

        // INC, HL
        // The byte at the memory address specified in the register HL is incremented
        // by 1.
        // Bit-by-bit: 0 0 1 1 0 1 0 0
        INC_HL = 0b00110100,

        // DEC, r
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

        // DEC, HL
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

        // RLCA
        // The contents of register A are rotated left by 1 bit position, and the sign
        // bit (7) is copied into the carry flag.
        // Bit-by-bit: 0 0 0 0 0 1 1 1
        RLCA = 0b00000111,

        // RLA
        // The contents of register A are rotated left by 1 bit position through the
        // carry flag.
        // Bit-by-bit: 0 0 0 1 0 1 1 1
        RLA = 0b00010111,

        // PREFIX:
        // Interpret the next byte as a prefix instruction (PrefixOpCode)
        // rather than a normal instruction (OpCode)
        PREFIX = 0b11001011
    };

    enum class PrefixOpCode {

    };

    enum class ArithmeticTarget : uint8_t {
        B,
        C,
        D,
        E,
        H,
        L,
        A
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
            operator uint8_t();
        };

        // Accumulator and flags
        uint8_t a = 0;
        Flags f = 0;

        // General purpose
        uint8_t b, c, d, e, h, l = 0;

        // Some instructions allow two 8 bit registers to be read as one 16 bit register
        // Referred to as AF (A & F), BC (B & C), DE (D & E) and HL (H & L)
        uint16_t getAF();
        void setAF(uint16_t value);
        uint16_t getBC();
        void setBC(uint16_t value);
        uint16_t getDE();
        void setDE(uint16_t value);
        uint16_t getHL();
        void setHL(uint16_t value);
    };

    class MemoryBus {
        std::array<uint8_t, 0xFFFF> m_memory{0};

    public:
        MemoryBus() = default;
        explicit MemoryBus(std::array<uint8_t, 0xFFFF> memory);

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

    void add(uint8_t value);

    void ADDA_r(ArithmeticTarget target);
    void ADDA_n();
    void ADDA_HL();

    void addWithCarry(uint8_t value);

    void ADCA_r(ArithmeticTarget target);
    void ADCA_n();
    void ADCA_HL();

    void subtract(uint8_t value);

    void SUB_r(ArithmeticTarget target);
    void SUB_n();
    void SUB_HL();

    void subtractWithCarry(uint8_t value);

    void SBCA_r(ArithmeticTarget target);
    void SBCA_n();
    void SBCA_HL();

    void bitwiseAnd(uint8_t value);

    void AND_r(ArithmeticTarget target);
    void AND_n();
    void AND_HL();

    void bitwiseOr(uint8_t value);

    void OR_r(ArithmeticTarget target);
    void OR_n();
    void OR_HL();

    void bitwiseXor(uint8_t value);

    void XOR_r(ArithmeticTarget target);
    void XOR_n();
    void XOR_HL();

    void compare(uint8_t value);

    void CP_r(ArithmeticTarget target);
    void CP_n();
    void CP_HL();

    void increment(uint8_t& target);

    void INC_r(ArithmeticTarget target);
    void INC_HL();

    void decrement(uint8_t& target);

    void DEC_r(ArithmeticTarget target);
    void DEC_HL();

    void DAA();
    void CPL();

    void CCF();
    void SCF();

    void rotateLeftToCarry(uint8_t& target);
    void rotateLeftThroughCarry(uint8_t& target);

    void RLCA();
    void RLA();

public:
    void reset();
    void exec();
    void step();
    void stepPrefix();
};

/*
    void SUBr_b();
    void SUBr_c();
    void SUBr_d();
    void SUBr_e();
    void SUBr_h();
    void SUBr_l();
    void SUBr_a();

    void SUBHL();
    void SUBn();

    void SBCr_b();
    void SBCr_c();
    void SBCr_d();
    void SBCr_e();
    void SBCr_h();
    void SBCr_l();
    void SBCr_a();

    void SBCHL();
    void SBCn();

    void CPr_b();
    void CPr_c();
    void CPr_d();
    void CPr_e();
    void CPr_h();
    void CPr_l();
    void CPr_a();

    void CPHL();
    void CPn();

    void ANDr_b();
    void ANDr_c();
    void ANDr_d();
    void ANDr_e();
    void ANDr_h();
    void ANDr_l();
    void ANDr_a();

    void ANDHL();
    void ANDn();

    void ORr_b();
    void ORr_c();
    void ORr_d();
    void ORr_e();
    void ORr_h();
    void ORr_l();
    void ORr_a();

    void ORHL();
    void ORn();

    void XORr_b();
    void XORr_c();
    void XORr_d();
    void XORr_e();
    void XORr_h();
    void XORr_l();
    void XORr_a();

    void XORHL();
    void XORn();

    void INCr_b();
    void INCr_c();
    void INCr_d();
    void INCr_e();
    void INCr_h();
    void INCr_l();
    void INCr_a();

    void INCHLm();

    void DECr_b();
    void DECr_c();
    void DECr_d();
    void DECr_e();
    void DECr_h();
    void DECr_l();
    void DECr_a();

    void DECHLm();

    void INCBC();
    void INCDE();
    void INCHL();
    void INCSP();

    void DECBC();
    void DECDE();
    void DECHL();
    void DECSP();

    // Bit manipulation
    void BIT0b();
    void BIT0c();
    void BIT0d();
    void BIT0e();
    void BIT0h();
    void BIT0l();
    void BIT0a();

    void BIT1b();
    void BIT1c();
    void BIT1d();
    void BIT1e();
    void BIT1h();
    void BIT1l();
    void BIT1a();

    void BIT2b();
    void BIT2c();
    void BIT2d();
    void BIT2e();
    void BIT2h();
    void BIT2l();
    void BIT2a();

    void BIT3b();
    void BIT3c();
    void BIT3d();
    void BIT3e();
    void BIT3h();
    void BIT3l();
    void BIT3a();

    void BIT4b();
    void BIT4c();
    void BIT4d();
    void BIT4e();
    void BIT4h();
    void BIT4l();
    void BIT4a();

    void BIT5b();
    void BIT5c();
    void BIT5d();
    void BIT5e();
    void BIT5h();
    void BIT5l();
    void BIT5a();

    void BIT6b();
    void BIT6c();
    void BIT6d();
    void BIT6e();
    void BIT6h();
    void BIT6l();
    void BIT6a();

    void BIT7b();
    void BIT7c();
    void BIT7d();
    void BIT7e();
    void BIT7h();
    void BIT7l();
    void BIT7a();

    void RLA();
    void RLCA();
    void RRA();
    void RRCA();

    void RLr_b();
    void RLr_c();
    void RLr_d();
    void RLr_e();
    void RLr_h();
    void RLr_l();
    void RLr_a();

    void RLHL();

    void RLCr_b();
    void RLCr_c();
    void RLCr_d();
    void RLCr_e();
    void RLCr_h();
    void RLCr_l();
    void RLCr_a();

    void RLCHL();

    void RRr_b();
    void RRr_c();
    void RRr_d();
    void RRr_e();
    void RRr_h();
    void RRr_l();
    void RRr_a();

    void RRHL();

    void RRCr_b();
    void RRCr_c();
    void RRCr_d();
    void RRCr_e();
    void RRCr_h();
    void RRCr_l();
    void RRCr_a();

    void RRCHL();

    void SLAr_b();
    void SLAr_c();
    void SLAr_d();
    void SLAr_e();
    void SLAr_h();
    void SLAr_l();
    void SLAr_a();

    void SLLr_b();
    void SLLr_c();
    void SLLr_d();
    void SLLr_e();
    void SLLr_h();
    void SLLr_l();
    void SLLr_a();

    void SRAr_b();
    void SRAr_c();
    void SRAr_d();
    void SRAr_e();
    void SRAr_h();
    void SRAr_l();
    void SRAr_a();

    void SRLr_b();
    void SRLr_c();
    void SRLr_d();
    void SRLr_e();
    void SRLr_h();
    void SRLr_l();
    void SRLr_a();

    void CPL();
    void NEG();

    void CCF();
    void SCF();

    // Stack instructions
    void PUSHBC();
    void PUSHDE();
    void PUSHHL();
    void PUSHAF();

    void POPBC();
    void POPDE();
    void POPHL();
    void POPAF();

    // Jump instructions
    void JPnn();
    void JPHL();
    void JPNZnn();
    void JPZnn();
    void JPNCnn();
    void JPCnn();

    void JRn();
    void JRNZn();
    void JRZn();
    void JRNCn();
    void JRCn();

    void DJNZn();

    void CALLnn();
    void CALLNZnn();
    void CALLZnn();
    void CALLNCnn();
    void CALLCnn();

    void RET();
    void RETI();
    void RETNZ();
    void RETZ();
    void RETNC();
    void RETC();

    void RST00();
    void RST08();
    void RST10();
    void RST18();
    void RST20();
    void RST28();
    void RST30();
    void RST38();
    void RST40();
    void RST48();
    void RST50();
    void RST58();
    void RST60();

    void NOP();
    void HALT();

    void DI();
    void EI();

    // Helper functions
    void fz();
    void MAPcb();

    void XX();
     */

/*// Load/store instructions
    void LDrr_bb();
    void LDrr_bc();
    void LDrr_bd();
    void LDrr_be();

    void LDrr_bh();
    void LDrr_bl();
    void LDrr_ba();

    void LDrr_cb();
    void LDrr_cc();
    void LDrr_cd();
    void LDrr_ce();
    void LDrr_ch();
    void LDrr_cl();
    void LDrr_ca();

    void LDrr_db();
    void LDrr_dc();
    void LDrr_dd();
    void LDrr_de();
    void LDrr_dh();
    void LDrr_dl();
    void LDrr_da();

    void LDrr_eb();
    void LDrr_ec();
    void LDrr_ed();
    void LDrr_ee();
    void LDrr_eh();
    void LDrr_el();
    void LDrr_ea();

    void LDrr_hb();
    void LDrr_hc();
    void LDrr_hd();
    void LDrr_he();
    void LDrr_hh();
    void LDrr_hl();
    void LDrr_ha();

    void LDrr_lb();
    void LDrr_lc();
    void LDrr_ld();
    void LDrr_le();
    void LDrr_lh();
    void LDrr_ll();
    void LDrr_la();

    void LDrr_ab();
    void LDrr_ac();
    void LDrr_ad();
    void LDrr_ae();
    void LDrr_ah();
    void LDrr_al();
    void LDrr_aa();

    void LDrHLm_b();
    void LDrHLm_c();
    void LDrHLm_d();
    void LDrHLm_e();
    void LDrHLm_h();
    void LDrHLm_l();
    void LDrHLm_a();

    void LDHLmn();

    void LDBCmA();
    void LDDEmA();

    void LDmmA();

    void LDABCm();
    void LDADEm();

    void LDAmm();

    void LDBCnn();
    void LDDEnn();
    void LDHLnn();
    void LDSPnn();

    void LDHLmm();
    void LDmmHL();

    void LDHLIA();
    void LDAHLI();

    void LDHLDA();
    void LDAHLD();

    void LDAIOn();
    void LDIOnA();
    void LDAIOC();
    void LDIOCA();

    void LDHLSPn();

    void SWAPr_b();
    void SWAPr_c();
    void SWAPr_d();
    void SWAPr_e();
    void SWAPr_h();
    void SWAPr_l();
    void SWAPr_a();*/

#endif //BIGBOY_CPU_H
