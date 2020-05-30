#include <bigboy/Serial.h>

#include <iostream>
#include <stdexcept>
#include <string>

std::vector<AddressSpace> Serial::addressSpaces() const {
    return {{0xFF01, 0xFF02}};
}

uint8_t Serial::readByte(uint16_t address) const {
    switch (address) {
        case 0xFF01:
            return m_data;
        case 0xFF02:
            return m_control;
        default:
            std::cerr << "SMemory device Serial does not address " << address << '\n';
            return 0xFF;
    }
}

void Serial::writeByte(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF01:
            m_data = value;
            break;
        case 0xFF02:
            m_control = value;
            // Debug logging
            if (value == 0x81) {
                std::cout << static_cast<unsigned char>(m_data);
            }
            break;
        default:
            std::cerr << "Memory device Serial does not address " << address << '\n';
    }
}
