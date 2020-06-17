#include <bigboy/Cartridge/Cartridge.h>

#include <algorithm>
#include <fstream>
#include <iostream>

Cartridge::Cartridge(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < m_bank0.size()) {
        throw std::runtime_error{"Tried to load illegally small ROM."};
    }

    for (int i = 0; i < m_bank0.size(); i++) {
        m_bank0[i] = bytes[i];
    }

    std::copy(bytes.begin() + m_bank0.size(), bytes.end(),
            std::back_inserter(m_bank1Onwards));

    switch (getMBCType()) {
        case MBCType::ROM_RAM_BATTERY: {
            const std::string saveFileName = getGameTitle() + ".gbsave";
            std::ifstream saveFile{saveFileName};
            if (saveFile.is_open()) {
                saveFile.seekg(0, std::ios::end);
                const size_t length = saveFile.tellg();
                saveFile.seekg(0, std::ios::beg);

                if (length != 0x2000) {
                    throw std::runtime_error{"Save file '" + saveFileName + "' is not the right size."};
                }

                m_extRAM.reserve(0x2000);
                saveFile.read(reinterpret_cast<char*>(m_extRAM.data()), 0x2000);
            }
        }
        // Fallthrough
        case MBCType::ROM_RAM:
        case MBCType::ROM:
            if (m_bank1Onwards.size() != 0x4000) {
                throw std::runtime_error{"Tried to load MBC0 ROM that was not 32KB in size."};
            }
            break;
        default:
            throw std::runtime_error{"Unimplemented MBC type: " + std::to_string(static_cast<int>(getMBCType()))};
    }
}

Cartridge::~Cartridge() {
    switch (getMBCType()) {
        case MBCType::ROM_RAM_BATTERY: {
            const std::string saveFileName = getGameTitle() + ".gbsave";
            std::ofstream saveFile{saveFileName};
            if (!saveFile.is_open()) {
                throw std::runtime_error{"Could not write save file '" + saveFileName + "'."};
            }

            saveFile.write(reinterpret_cast<char*>(m_extRAM.data()), 0x2000);
            break;
        }
        default:
            break;
    }
}

Cartridge Cartridge::fromFile(std::string fileName) {
    std::ifstream file{fileName};
    if (!file.is_open()) {
        throw std::runtime_error{"[fatal]: File '" + fileName + "' could not be opened."};
    }

    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> bytes{};
    bytes.resize(length);
    file.read(reinterpret_cast<char*>(bytes.data()), length);
    file.close();

    return Cartridge{bytes};
}

std::vector<AddressSpace> Cartridge::addressSpaces() const {
    return {{0x0000, 0x7FFF}, {0xA000, 0xBFFF}};
}

uint8_t Cartridge::readByte(uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_bank0[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        switch (getMBCType()) {
            case MBCType::ROM:
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                return m_bank1Onwards[address - 0x4000];
            default:
                break;
        }
    }
    if (address >= 0xA000 && address <= 0xBFFF) {
        switch (getMBCType()) {
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                return m_extRAM[address - 0xA000];
            default:
                break;
        }
    }

    std::cerr << "Memory device Cartridge does not support reading the address: " << address << '\n';
    return 0xFF;
}

void Cartridge::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0xA000 && address <= 0xBFFF) {
        switch (getMBCType()) {
            case MBCType::ROM:
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                m_extRAM[address - 0xA000] = value;
                return;
            default:
                break;
        }
    }

    std::cerr << "Memory device Cartridge does not support writing to the address: " << address << '\n';
}

MBCType Cartridge::getMBCType() const {
    return static_cast<MBCType>(m_bank0[0x0147]);
}

std::string Cartridge::getGameTitle() const {
    std::string name{};
    for (int i = 0x0134; i < 0x0143; i++) {
        char c = m_bank0[i];
        if (c == '\0') break;

        name.push_back(c);
    }
    return name;
}