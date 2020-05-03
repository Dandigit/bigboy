#include <bigboy/CPU/MemoryBus.h>

MemoryBus::MemoryBus(std::array<uint8_t, 0xFFFF> memory) :
        m_memory{memory} {}

uint8_t& MemoryBus::byteAt(uint16_t address) {
    return m_memory[address];
}

uint8_t MemoryBus::readByte(uint16_t address) {
    return m_memory[address];
}

void MemoryBus::writeByte(uint16_t address, uint8_t value) {
    m_memory[address] = value;
}