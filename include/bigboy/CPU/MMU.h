#ifndef BIGBOY_MMU_H
#define BIGBOY_MMU_H

#include <array>
#include <cstdint>

class MMU {
    std::array<uint8_t, 0xFFFF> m_memory{0};

public:
    MMU() = default;

    explicit MMU(std::array<uint8_t, 0xFFFF> memory);

    uint8_t& byteAt(uint16_t address);

    // TODO: Use byteAt instead
    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
};

#endif //BIGBOY_MMU_H