#include <bigboy/CPU/Flags.h>

BitFlag::BitFlag(uint8_t& byte, uint8_t position) :
        m_byte{byte},
        m_position{position} {}

BitFlag& BitFlag::operator=(bool set) {
    m_byte = (m_byte & ~(1UL << m_position)) | (m_position << set);
    return *this;
}

BitFlag::operator bool() const {
    return ((m_byte >> m_position) & 1u) == 1u;
}

Flags::Flags(uint8_t& register_) :
        m_register{register_},
        zero{register_, ZERO_FLAG_BYTE_POSITION},
        subtract{register_, SUBTRACT_FLAG_BYTE_POSITION},
        halfCarry{register_, HALF_CARRY_FLAG_BYTE_POSITION},
        carry{register_, CARRY_FLAG_BYTE_POSITION} {}

bool Flags::get(ConditionOperand condition) const {
    switch (condition) {
        case ConditionOperand::NZ: return !zero;
        case ConditionOperand::Z: return zero;
        case ConditionOperand::NC: return !carry;
        case ConditionOperand::C: return carry;
    }
}