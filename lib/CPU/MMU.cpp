#include <bigboy/CPU/MMU.h>

MMU::MMU(std::array<uint8_t, 0xFFFF> memory) :
        m_memory{memory} {}

uint8_t& MMU::byteAt(uint16_t address) {
    return m_memory[address];
}

uint8_t MMU::readByte(uint16_t address) {
    return m_memory[address];
}

void MMU::writeByte(uint16_t address, uint8_t value) {
    m_memory[address] = value;
}