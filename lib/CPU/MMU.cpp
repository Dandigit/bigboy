#include <bigboy/CPU/MMU.h>

MMU::MMU(std::array<uint8_t, 0xFFFF> memory) :
        m_memory{memory} {}

uint8_t& MMU::byteAt(uint16_t address) {
    return m_memory[address];
}

uint8_t MMU::byteAt(uint16_t address) const {
    return m_memory[address];
}