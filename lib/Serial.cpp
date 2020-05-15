#include <bigboy/Serial.h>

#include <iostream>
#include <stdexcept>
#include <string>

bool Serial::doesAddress(uint16_t address) const {
    return address == 0xFF01 || address == 0xFF02;
}

uint8_t Serial::readByte(uint16_t address) const {
    switch (address) {
        case 0xFF01:
            return m_data;
        case 0xFF02:
            throw std::runtime_error{"Serial SC reading is not implemented!"};
        default:
            throw std::runtime_error{"Memory device Serial does not address " + std::to_string(address)};
    }
}

void Serial::writeByte(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF01:
            m_data = value;
            break;
        case 0xFF02:
            // Debug logging
            if (value == 0x81) {
                std::cout << static_cast<unsigned char>(value);
            } else {
                throw std::runtime_error{"Serial SC writing is not implemented!"};
            }

            break;
        default:
            throw std::runtime_error{"Memory device Serial does not address " + std::to_string(address)};
    }
}
