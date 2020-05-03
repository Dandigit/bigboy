#include <bigboy/CPU/Flags.h>

Flag::Flag(uint8_t& register_, uint8_t position) :
        m_register{register_},
        m_position{position} {}

Flag& Flag::operator=(bool set) {
    m_register = (m_register & ~(1UL << m_position)) | (m_position << set);
    return *this;
}

Flag::operator bool() {
    return ((m_register >> m_position) & 1u) == 1u;
}

Flags::Flags(uint8_t& register_) :
        m_register{register_},
        zero{register_, ZERO_FLAG_BYTE_POSITION},
        subtract{register_, SUBTRACT_FLAG_BYTE_POSITION},
        halfCarry{register_, HALF_CARRY_FLAG_BYTE_POSITION},
        carry{register_, CARRY_FLAG_BYTE_POSITION} {}