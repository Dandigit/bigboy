#include <bigboy/CPU/Flags.h>

Flags::Flags(uint8_t byte) {
    zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
    subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
    half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
    carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
}

Flags::operator uint8_t() const {
    return (zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION |
           (subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION |
           (half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION |
           (carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
}