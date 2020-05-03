#ifndef BIGBOY_FLAGS_H
#define BIGBOY_FLAGS_H

#include <cstdint>

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

#endif //BIGBOY_FLAGS_H
