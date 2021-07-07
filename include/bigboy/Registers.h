#ifndef BIGBOY_REGISTERS_H
#define BIGBOY_REGISTERS_H

#include <cstdint>

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

enum class RegisterPairStackOperand : uint8_t {
    BC, // 00
    DE, // 01
    HL, // 10
    AF  // 11
};

enum class ConditionOperand : uint8_t {
    NZ, // 000 or 001
    Z,  // 010 or 011
    NC, // 100 or 101
    C   // 110 or 111
};

class Registers {
public:
    // General purpose
    #ifdef BIGBOY_BIG_ENDIAN
    uint8_t b = 0;
    uint8_t c = 0;
    #else
    uint8_t c = 0;
    uint8_t b = 0;
    #endif

    #ifdef BIGBOY_BIG_ENDIAN
    uint8_t d = 0;
    uint8_t e = 0;
    #else
    uint8_t e = 0;
    uint8_t d = 0;
    #endif

    #ifdef BIGBOY_BIG_ENDIAN
    uint8_t h = 0;
    uint8_t l = 0;
    #else
    uint8_t l = 0;
    uint8_t h = 0;
    #endif

    // Accumulator and flags
    #ifdef BIGBOY_BIG_ENDIAN
    uint8_t a = 0;
    uint8_t f = 0;
    #else
    uint8_t f = 0;
    uint8_t a = 0;
    #endif

    // Stack pointer
    uint16_t sp = 0xFF - 1;

    // Some instructions allow two 8 bit registers to be read as one 16 bit register
    // Referred to as BC (B & C), DE (D & E), HL (H & L) and AF (A & F)
    uint16_t& BC();
    uint16_t BC() const;

    uint16_t& DE();
    uint16_t DE() const;

    uint16_t& HL();
    uint16_t HL() const;

    uint16_t& AF();
    uint16_t AF() const;

    uint8_t& get(RegisterOperand target);
    uint16_t& get(RegisterPairOperand target);
    uint16_t& get(RegisterPairStackOperand target);
    bool get(ConditionOperand condition) const;

    bool getZeroFlag()      const { return (f >> ZERO_FLAG_BYTE_POSITION) & 1u; }
    bool getSubtractFlag()  const { return (f >> SUBTRACT_FLAG_BYTE_POSITION) & 1u; }
    bool getHalfCarryFlag() const { return (f >> HALF_CARRY_FLAG_BYTE_POSITION) & 1u; }
    bool getCarryFlag()     const { return (f >> CARRY_FLAG_BYTE_POSITION) & 1u; }

    void setZeroFlag()      { f |= (1u << ZERO_FLAG_BYTE_POSITION); }
    void setSubtractFlag()  { f |= (1u << SUBTRACT_FLAG_BYTE_POSITION); }
    void setHalfCarryFlag() { f |= (1u << HALF_CARRY_FLAG_BYTE_POSITION); }
    void setCarryFlag()     { f |= (1u << CARRY_FLAG_BYTE_POSITION); }

    void clearZeroFlag()      { f &= ~(1u << ZERO_FLAG_BYTE_POSITION); }
    void clearSubtractFlag()  { f &= ~(1u << SUBTRACT_FLAG_BYTE_POSITION); }
    void clearHalfCarryFlag() { f &= ~(1u << HALF_CARRY_FLAG_BYTE_POSITION); }
    void clearCarryFlag()     { f &= ~(1u << CARRY_FLAG_BYTE_POSITION); }

    void reset();

private:
    static constexpr uint8_t ZERO_FLAG_BYTE_POSITION = 7;
    static constexpr uint8_t SUBTRACT_FLAG_BYTE_POSITION = 6;
    static constexpr uint8_t HALF_CARRY_FLAG_BYTE_POSITION = 5;
    static constexpr uint8_t CARRY_FLAG_BYTE_POSITION = 4;
};

#endif //BIGBOY_REGISTERS_H
