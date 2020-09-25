#ifndef BIGBOY_INTERNALMEMORY_H
#define BIGBOY_INTERNALMEMORY_H

#include <array>
#include <cstdint>

#include <bigboy/MMU/MemoryDevice.h>

class InternalMemory : public MemoryDevice {
public:
    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

    void reset();

private:
    // 2x4KB work RAM banks: C000-CFFF and D000-DFFF
    // Also addressable through E000-FDFF
    std::array<uint8_t, 0xFFF + 1> m_wram0{0};
    std::array<uint8_t, 0xFFF + 1> m_wram1{0};

    // High RAM (HRAM): FF80-FFFE
    std::array<uint8_t, 0x7F + 1> m_hram{0};

    // Interrupt enable register: FFFF
    uint8_t m_ie = 0;

    // Interrupt flag (request) register: FF0F
    uint8_t m_if = 0;
};

#endif //BIGBOY_INTERNALMEMORY_H
