#include <bigboy/MMU/InternalMemory.h>

#include <string>

std::vector<AddressSpace> InternalMemory::addressSpaces() const {
    return {{0xC000, 0xFDFF}, {0xFF80, 0xFFFF}};
}

uint8_t InternalMemory::readByte(uint16_t address) const {
    // 4KB Work RAM Bank 0
    if (address >= 0xC000 && address <= 0xCFFF) {
        return m_wram0[address - 0xC000];
    }

    // 4KB Work RAM Bank 1
    if (address >= 0xD000 && address <= 0xDFFF) {
        return m_wram1[address - 0xD000];
    }

    // ECHO of C000-CFFF
    if (address >= 0xE000 && address <= 0xEFFF) {
        return m_wram0[address - 0xE000];
    }

    // ECHO of D000 to DDFF
    if (address >= 0xF000 && address <= 0xFDFF) {
        return m_wram1[address - 0xF000];
    }

    // High RAM (HRAM)
    if (address >= 0xFF80 && address <= 0xFFFE) {
        return m_hram[address- 0xFF80];
    }

    // Interrupt Enable Register
    if (address == 0xFFFF) {
        return m_ie;
    }

    throw std::runtime_error{"The memory device InternalMemory does not own the address: " +
            std::to_string(address)};
}

void InternalMemory::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0xC000 && address <= 0xCFFF) {
        // 4KB Work RAM Bank 0
        m_wram0[0xCFFF - address] = value;
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        // 4KB Work RAM Bank 1
        m_wram1[0xDFFF - address] = value;
    } else if (address >= 0xE000 && address <= 0xEFFF) {
        // ECHO of C000-CFFF
        m_wram0[0xEFFF - address] = value;
    } else if (address >= 0xF000 && address <= 0xFDFF) {
        // ECHO of D000 to DDFF
        m_wram1[0xFDFF - address] = value;
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        // High RAM (HRAM)
        m_hram[0xFFFE - address] = value;
    } else if (address == 0xFFFF) {
        // Interrupt Enable Register
        m_ie = value;
    } else {
        throw std::runtime_error{"The memory device InternalMemory does not own the address: " +
                                 std::to_string(address)};
    }
}

void InternalMemory::reset() {
    m_ie = 0x00;
}
