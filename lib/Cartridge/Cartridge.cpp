#include <bigboy/Cartridge/Cartridge.h>

#include <iostream>
#include <fstream>

Cartridge::Cartridge(std::array<uint8_t, 0x3FFF + 1> bank0, std::array<uint8_t, 0x3FFF + 1> bank1) :
        m_bank0{std::move(bank0)},
        m_bank1{std::move(bank1)} {
}

Cartridge Cartridge::fromFile(std::string fileName) {
    Cartridge cartridge{};
    cartridge.loadFile(std::move(fileName));
    return cartridge;
}

void Cartridge::loadFile(std::string fileName) {
    std::ifstream file{fileName};
    if (!file.is_open()) {
        throw std::runtime_error{"[fatal]: File '" + fileName + "' could not be opened."};
    }

    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    if (length != 32768) {
        throw std::runtime_error{"[fatal]: File '" + fileName + "' is not a 32KB GameBoy ROM."};
    }

    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(m_bank0.data()), m_bank0.size());

    file.read(reinterpret_cast<char*>(m_bank1.data()), m_bank1.size());

    file.close();
}

bool Cartridge::doesAddress(uint16_t address) const {
    return (address >= 0x0000 && address <= 0x7FFF) ||
            (address >= 0xA000 && address <= 0xBFFF);
}

uint8_t Cartridge::readByte(uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_bank0[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        return m_bank1[address];
    }
    if (address >= 0xA000 && address <= 0xBFFF) {
        return m_extRAM[address];
    }

    throw std::runtime_error{"Memory device Cartridge does not support reading the address: " +
            std::to_string(address)};
}

void Cartridge::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0xA000 && address <= 0xBFFF) {
        m_extRAM[address] = value;
    } else {
        throw std::runtime_error{"Memory device Cartridge does not support writing to the address: " +
                std::to_string(address)};
    }
}