#ifndef BIGBOY_FLAGS_H
#define BIGBOY_FLAGS_H

#include <cstdint>

class Flag {
    uint8_t& m_register;
    uint8_t m_position;

public:
    // We can treat a Flags object like a normal uint8_t
    Flag(uint8_t& register_, uint8_t position);

    Flag& operator=(bool set);

    operator bool();
};

class Flags {
private:
    static constexpr uint8_t ZERO_FLAG_BYTE_POSITION = 7;
    static constexpr uint8_t SUBTRACT_FLAG_BYTE_POSITION = 6;
    static constexpr uint8_t HALF_CARRY_FLAG_BYTE_POSITION = 5;
    static constexpr uint8_t CARRY_FLAG_BYTE_POSITION = 4;

    uint8_t& m_register;

public:
    Flag zero;
    Flag subtract;
    Flag halfCarry;
    Flag carry;

    explicit Flags(uint8_t& register_);
};

#endif //BIGBOY_FLAGS_H
